/*
    ChibiOS/RT - Copyright (C) 2015 ALexander Geissler

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"
#include "ff.h"
#include "common.h"

#include "usbcfg.h"

#include "cs43l22.h"
//#include "pcm1792a.h"
#include "adv7612.h"

#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP1_BUF_DEPTH      8
#define ADC_GRP2_NUM_CHANNELS   4
#define ADC_GRP2_BUF_DEPTH      16

MMCDriver MMCD1;

static THD_WORKING_AREA(ledThreadWorkingArea, 64);
static THD_WORKING_AREA(pwmThreadWorkingArea, 32);
//static THD_WORKING_AREA(controlThreadWorkingArea, 256);
//static THD_WORKING_AREA(audioThreadWorkingArea, 1024);

static THD_FUNCTION(ledThread, arg) {

  (void)arg;
  chRegSetThreadName("LED");
  while (TRUE) {
    palTogglePad(GPIOD, 15);
    chThdSleepMilliseconds(500);
    palTogglePad(GPIOD, 15);
    chThdSleepMilliseconds(500);
  }
}

THD_FUNCTION(pwmThread, arg) {
  (void)arg;

  chRegSetThreadName("PWM");
  while (TRUE) {
    pwmEnableChannel(&PWMD4, 0, (pwmcnt_t)5);
    pwmEnableChannel(&PWMD4, 1, (pwmcnt_t)10);
    chThdSleepMilliseconds(250);
  }
}

/*===========================================================================*/
/* FatFs related.                                                            */
/*===========================================================================*/

/*
 * @brief FS object
 */
static FATFS SDC_FS;

/* FS ready and mounted */
static bool fs_ready = FALSE;
/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static void cmd_boot(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: boot\r\n");
    return;
  }
}

static void cmd_led(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if(argc > 0) {
    chprintf(chp, "Usage: led\r\n");
    return;
  }
  chprintf(chp, "toggling leds\r\n");
  palTogglePad(GPIOD, 12);
  palTogglePad(GPIOD, 13);
  palTogglePad(GPIOD, 14);
  palTogglePad(GPIOD, 15);
}

static const ShellCommand commands[] = {
  {"boot", cmd_boot},
  {"led", cmd_led},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

/*===========================================================================*/
/* Accelerometer related.                                                    */
/*===========================================================================*/

/*
 * PWM configuration structure.
 * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
 * the active state is a logic one.
 */
static const PWMConfig pwmcfg = {
  100000,                                   /* 100kHz PWM clock frequency.  */
  512,                                      /* PWM period is 128 cycles.    */
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
  },
  /* HW dependent part.*/
  0,
  0,
#if STM32_PWM_USE_ADVANCED
  0
#endif
};

/*
 * SPI1 configuration structure.
 * Speed 5.25MHz, CPHA=1, CPOL=1, 8bits frames, MSb transmitted first.
 * The slave select line is the pin GPIOE_CS_SPI on the port GPIOE.
 */
static const SPIConfig spi1cfg = {
  NULL,
  /* HW dependent part.*/
  GPIOC,
  GPIOE_CS_SPI,
  SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
};

/*
 * SPI2 configuration structure.
 * Speed 21MHz, CPHA=0, CPOL=0, 8bits frames, MSb transmitted first.
 * The slave select line is the pin 12 on the port GPIOA.
 */
static const SPIConfig spi2cfg = {
  NULL,
  /* HW dependent part.*/
  GPIOB,
  12,
  0
};

/*
 * I2S configuration structure
 * This Interface is attached to an FPGA
 * TODO: enable DMA channels
 */
static uint32_t audio_tx_buf[1024];
static uint32_t audio_rx_buf[1024];
static const I2SConfig i2s3cfg = {
  &audio_tx_buf,
  &audio_rx_buf,
  sizeof(audio_rx_buf),
  NULL,
  0,
  16
};

/*
 * I2C configuration structure for HDMI and other chips
 */
static const I2CConfig i2cfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2
};

/*
 * ADC configuration structure.
 */

static void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
  (void)adcp;

}

static void adcerrcb(ADCDriver *adcp, adcerror_t err) {
  (void)adcp;
  (void)err;
}

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static adcsample_t samples2[ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH];

/*
 * ADC conversion group
 * Mode:        Linear buffer, 8 samples of one channel, SW triggered
 * Channels:    IN10
 */
static const ADCConversionGroup adcgrpcfg1 = {
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  NULL,
  adcerrcb,
  0,                                      /* CR1 */
  ADC_CR2_SWSTART,                        /* CR2 */
  ADC_SMPR1_SMP_AN10(ADC_SAMPLE_3),       /* SMPR1 */
  0,                                      /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS), /* SQR1 */
  0,                                      /* SQR2 */
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)
};

/*
 * ADC Conversion group
 * Mode:        Continous, 16 samples of 4 channels, SW triggered
 * Channels:    IN12, IN13, Sensor, Vref
 */
static const ADCConversionGroup adcgrpcfg2 = {
  TRUE,
  ADC_GRP2_NUM_CHANNELS,
  adccb,
  adcerrcb,
  0,               /* CR1 */
  ADC_CR2_SWSTART, /* CR2 */
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_56) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_56) |
  ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) | ADC_SMPR1_SMP_VREF(ADC_SAMPLE_144),
  0,               /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP2_NUM_CHANNELS), /* SQR1 */
  0,
  ADC_SQR2_SQ8_N(ADC_CHANNEL_SENSOR) | ADC_SQR2_SQ7_N(ADC_CHANNEL_VREFINT) |
  ADC_SQR3_SQ6_N(ADC_CHANNEL_IN12) | ADC_SQR3_SQ5_N(ADC_CHANNEL_IN13)
};

/*===========================================================================*/
/* Initialization and main thread.                                           */
/*===========================================================================*/

/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Initializes a serial-over-USB CDC driver.
   */
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * St I2S PLL
   */
  Config_I2S(&I2SD3, SR_48kHz, 1);

  /*
   * Initializes a sd-card driver
   */
  mmcObjectInit(&MMCD1);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1000);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  /*
   * Activates an analog interface
   */

  /*
   * Activates the ADCD1 driver and temperature sensor
   */
  adcStart(&ADCD1, NULL);
  adcSTM32EnableTSVREFE();

  /*
   * Activates the I2C interface
   * The HDMI, DAC, and maybe the ADC are using this interface
   */
  i2cStart(&I2CD1, &i2cfg);
  palSetPadMode(GPIOB, 6, PAL_STM32_OTYPE_OPENDRAIN | PAL_MODE_ALTERNATE(4)); /* SCL */
  palSetPadMode(GPIOB, 9, PAL_STM32_OTYPE_OPENDRAIN | PAL_MODE_ALTERNATE(4)); /* SDA */

  /* Assign driver to Codec interface */
  Codec_Init(&I2CD1);
  /* Configure codecs with defined settings */
  // TODO

  /*
   * Initializes the SPI driver 1 in order to access the MEMS. The signals
   * are already initialized in the board file.
   */
  spiStart(&SPID1, &spi1cfg);

  /*
   * Initializes the SPI driver 2. The SPI2 signals are routed as follow:
   * PB12 - NSS.
   * PB13 - SCK.
   * PB14 - MISO.
   * PB15 - MOSI.
   */

  spiStart(&SPID2, &spi2cfg);
  palSetPad(GPIOB, 12);
  palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL |
                           PAL_STM32_OSPEED_HIGHEST);           /* NSS.     */
  palSetPadMode(GPIOB, 13, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);           /* SCK.     */
  palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(5));              /* MISO.    */
  palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);           /* MOSI.    */

  /*
   * Initializes the I2S driver 3. The I2S signals are routed as follow:
   * PA4  - I2S3_WS.
   * PC7  - I2S3_MCK.
   * PC10 - I2S3_SCK.
   * PC12 - I2S3_SD.
   */
  i2sStart(&I2SD3, &i2s3cfg);
  palSetPadMode(GPIOA, 4, PAL_MODE_OUTPUT_PUSHPULL | PAL_MODE_ALTERNATE(6) |
                PAL_STM32_OSPEED_MID2); /* WS  */
  palSetPadMode(GPIOC, 7, PAL_MODE_OUTPUT_PUSHPULL | PAL_MODE_ALTERNATE(6) |
                PAL_STM32_OSPEED_MID2); /* MCK */
  palSetPadMode(GPIOC, 10, PAL_MODE_OUTPUT_PUSHPULL | PAL_MODE_ALTERNATE(6) |
                PAL_STM32_OSPEED_MID2); /* SCK */
  palSetPadMode(GPIOC, 12, PAL_MODE_OUTPUT_PUSHPULL | PAL_MODE_ALTERNATE(6) |
                PAL_STM32_OSPEED_MID2); /* SD */

  /*
   * Initializes PWM driver 4
   */
  pwmStart(&PWMD4, &pwmcfg);
  palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, 13, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOD, 14, PAL_MODE_ALTERNATE(2));
  //palSetPadMode(GPIOD, 15, PAL_MODE_ALTERNATE(2));

  chThdCreateStatic(ledThreadWorkingArea, sizeof(ledThreadWorkingArea),
                    NORMALPRIO+1, ledThread, NULL);

  chThdCreateStatic(pwmThreadWorkingArea, sizeof(pwmThreadWorkingArea),
                    NORMALPRIO+2, pwmThread, NULL);

  while (TRUE) {

    if (SDU1.config->usbp->state == USB_ACTIVE) {
       thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                               "shell", NORMALPRIO+3,
                                               shellThread, (void *)&shell_cfg1);
       chThdWait(shelltp);
    }
    chThdSleepMilliseconds(500);
  }
}
