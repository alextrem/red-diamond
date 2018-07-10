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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"
#include "common.h"

#include "usbcfg.h"

#include "cs43l22.h"
//#include "pcm1792a.h"
#include "adv7612.h"

#include "ff.h"
#include "mad.h"

#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP1_BUF_DEPTH      8
#define ADC_GRP2_NUM_CHANNELS   2
#define ADC_GRP2_BUF_DEPTH      16
static uint32_t temperature = 0;

static THD_WORKING_AREA(ledThreadWorkingArea, 64);
static THD_WORKING_AREA(pwmThreadWorkingArea, 32);
//static THD_WORKING_AREA(controlThreadWorkingArea, 256);
//static THD_WORKING_AREA(audioThreadWorkingArea, 1024);

MMCDriver MMCD1;

/*
 * SD-Card event sources
 */
static event_source_t inserted_event, removed_event;

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
    pwmEnableChannel(&PWMD4, 0, (pwmcnt_t)100);
    pwmEnableChannel(&PWMD4, 1, (pwmcnt_t)100);
    chThdSleepMilliseconds(250);
  }
}

/*===========================================================================*/
/* FatFs related.                                                            */
/*===========================================================================*/

/*
 * @brief FS object
 */
static FATFS MMC_FS;

/* FS ready and mounted */
static bool fs_ready = FALSE;

/* Generic large buffer.*/
static uint8_t fbuff[1024];

static FRESULT scan_files(BaseSequentialStream *chp, char *path) {
  FRESULT res;
  FILINFO fno;
  DIR dir;
  int i;
  char *fn;

  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    i = strlen(path);
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
        break;
      if (fno.fname[0] == '.')
        continue;
      fn = fno.fname;
      if (fno.fattrib & AM_DIR) {
        path[i++] = '/';
        strcpy(&path[i], fn);
        res = scan_files(chp, path);
        if (res != FR_OK)
          break;
        path[--i] = 0;
      }
      else {
        chprintf(chp, "%s/%s\r\n", path, fn);
      }
    }
  }
  return res;
}

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static void cmd_dir(BaseSequentialStream *chp, int argc, char *argv[]) {
  FRESULT err;
  uint32_t clusters;
  FATFS *fsp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: tree\r\n");
    return;
  }
  if (!fs_ready) {
    chprintf(chp, "File System not mounted\r\n");
    return;
  }
  err = f_getfree("/", &clusters, &fsp);
  if (err != FR_OK) {
    chprintf(chp, "FS: f_getfree() failed\r\n");
    return;
  }
  chprintf(chp,
           "FS: %lu free clusters, %lu sectors per cluster, %lu bytes free\r\n",
           clusters, (uint32_t)MMC_FS.csize,
           clusters * (uint32_t)MMC_FS.csize * (uint32_t)MMCSD_BLOCK_SIZE);
  fbuff[0] = 0;
  scan_files(chp, (char *)fbuff);
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

static void cmd_mad(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mad\r\n");
    return;
  }
  chprintf(chp, "Libmad: %s, %s, %s\r\n", __FILE__, __DATE__, __TIME__);
  chprintf(chp, "%s\r\n", mad_version);
  chprintf(chp, "%s\r\n", mad_copyright);
  chprintf(chp, "%s\r\n", mad_author);
  chprintf(chp, "%s\r\n", mad_build);
}

static void cmd_adc(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  chprintf(chp, "Temperature: %d \r\n", temperature );
  if (argc > 0) {
    chprintf(chp, "Usage: adc\r\n");
    return;
  }
}

static void cmd_codec(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  if (argc > 2) {
    chprintf(chp, "Usage: codec [command]\r\n");
    return;
  }

  if (strcmp("all", argv[0]) == 0) {
    uint8_t id = Codec_GetID(&I2CD1);
    chprintf(chp, "ID: 0x%x\r\n", id);
  }
  else if (strcmp("mute", argv[0]) == 0) {
    Codec_Mute(&I2CD1, all);
    chprintf(chp, "Muting outputs\r\n");
  }
  else if (strcmp("beep", argv[0]) == 0) {
    Codec_BeepGenerator(&I2CD1);
    chprintf(chp, "Setting Beep frequency to 1kHz\r\n");
  }
  else if (strcmp("vol", argv[0]) == 0) {
    uint32_t volume = atoi(argv[1]);
    if (volume < 256) {
      Codec_VolumeCtrl(&I2CD1, all, volume);
      chprintf(chp, "Set volume to %d\r\n", volume);
    }
  }
}

static void cmd_dac(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argv;
  if (argc > 0) {
    chprintf(chp, "Usage: dac [command]\r\n");
    return;
  }
}

static const ShellCommand commands[] = {
  {"led", cmd_led},
  {"mad", cmd_mad},
  {"adc", cmd_adc},
  {"codec", cmd_codec},
  {"dac", cmd_dac},
  {"dir", cmd_dir},
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
  10000,                                    /* 10kHz PWM clock frequency.  */
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
  false,
  NULL,
  /* HW dependent part.*/
  GPIOC,
  GPIOC_PIN4,
  SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA,
  0
};

static const MMCConfig mmc1cfg = {
  &SPID1,
  &spi1cfg,
  &spi1cfg
};

/*
 * SPI2 configuration structure.
 * Speed 21MHz, CPHA=0, CPOL=0, 8bits frames, MSb transmitted first.
 * The slave select line is the pin 12 on the port GPIOA.
 */
static const SPIConfig spi2cfg = {
  false,
  NULL,
  /* HW dependent part.*/
  GPIOB,
  GPIOB_PIN12,
  SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA,
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
  sizeof(audio_rx_buf) / 4,
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

/*===========================================================================*/
/* ADC related.                                                              */
/*===========================================================================*/

/*
 * ADC configuration structure.
 */
//static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static adcsample_t samples2[ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH];

static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
  (void)adcp;
  (void)buffer;
  (void)n;

  int32_t temp_avg = 0;
  //samples2 = buffer;

  /* Compute avarage of temperature sensor raw data */
  for (int i = 0; i < (ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH); i += 2 ) {
    temp_avg += samples2[i];
  }
  temp_avg /= ADC_GRP2_BUF_DEPTH;
  temperature = temp_avg;
}

static void adcerrcb(ADCDriver *adcp, adcerror_t err) {
  (void)adcp;
  (void)err;
}

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
  0,
  0,
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS), /* SQR1 */
  0,                                      /* SQR2 */
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)        /* SQR3 */
};

/*
 * ADC Conversion group
 * Mode:        Continous, 16 samples of 2 channels, SW triggered
 * Channels:    Sensor, Vref
 */
static const ADCConversionGroup adcgrpcfg2 = {
  TRUE,            /* circular buffer */
  ADC_GRP2_NUM_CHANNELS,
  adccallback,
  adcerrcb,
  0,               /* CR1 */
  ADC_CR2_SWSTART, /* CR2 */
  ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_480) | ADC_SMPR1_SMP_VREF(ADC_SAMPLE_480), /* SMPR1 */
  0,               /* SMPR2 */
  0,
  0,
  ADC_SQR1_NUM_CH(ADC_GRP2_NUM_CHANNELS), /* SQR1 */
  ADC_SQR2_SQ8_N(ADC_CHANNEL_SENSOR) | ADC_SQR2_SQ7_N(ADC_CHANNEL_VREFINT), /* SQR2 */
  0 /* SQR3 */
};

/*==========================================================================*/
/* DAC Conversion Group                                                     */
/*==========================================================================*/
static const DACConfig dac1cfg = {
  .init = 0U,
  DAC_DHRM_12BIT_RIGHT,
  0
};

static const DACConversionGroup dacgrpcfg = {
  .num_channels = 1U,
  .end_cb = NULL,
  .error_cb = NULL,
  .trigger = DAC_TRG_SW
};

/*===========================================================================*/
/* Main and generic code                                                     */
/*===========================================================================*/

static thread_t *shelltp = NULL;

static void InserHandler(eventid_t id) {
  FRESULT err;

  (void) id;

  if (mmcConnect(&MMCD1))
    return;

  /* On SD insertion do file system mount */
  err = f_mount(&MMC_FS, "/", 1);
  if (err != FR_OK) {
    mmcDisconnect(&MMCD1);
    return;
  }
  fs_ready = TRUE;
}

static void RemoveHandler(eventid_t id) {
  (void) id;
  mmcDisconnect(&MMCD1);
  fs_ready = FALSE;
}

static void ShellHandler(eventid_t id) {
  (void) id;
  if (chThdTerminatedX(shelltp)) {
    /* Return memory to heap*/
    chThdWait(shelltp);
    shelltp = NULL;
  }
}

/*===========================================================================*/
/* Initialization and main thread.                                           */
/*===========================================================================*/

/*
 * Application entry point.
 */
int main(void) {
  static const evhandler_t evhndl[] = {
    InserHandler,
    RemoveHandler,
    ShellHandler
  };
  event_listener_t el0, el1, el2;

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
   * Set I2S PLL
   */
  Config_I2S(&I2SD3, SR_48kHz, BIT_32);

  /*
   * Initializes a sd-card driver
   */

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  /*
   * Activates the serial driver
   */
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

  /*
   * Activates the I2C interface
   * The HDMI, DAC, and maybe the ADC are using this interface
   */
  i2cStart(&I2CD1, &i2cfg);
  palSetGroupMode(GPIOB, PAL_PORT_BIT(6) | PAL_PORT_BIT(9), 0,
                  PAL_STM32_OTYPE_OPENDRAIN | PAL_MODE_ALTERNATE(4));

  /* Assign driver to Codec interface */
  Codec_Init(&I2CD1);
  /* Configure codecs with defined settings */
  Codec_BeepGenerator(&I2CD1);
  i2sStartExchange(&I2SD3);

  /*
   * Initializes the SPI driver 1 in order to access the MEMS. The signals
   * are already initialized in the board file.
   */
  mmcObjectInit(&MMCD1);
  mmcStart(&MMCD1, &mmc1cfg);

  /*
   * Initializes the SPI driver 2. The SPI2 signals are routed as follow:
   * PB12 - NSS.
   * PB13 - SCK.
   * PB14 - MISO.
   * PB15 - MOSI.
   */

  spiStart(&SPID2, &spi2cfg);
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
  //dmaStreamAllocate(I2SD3.txdma, 0, NULL, NULL);
  i2sStart(&I2SD3, &i2s3cfg);
  palSetGroupMode(GPIOC, PAL_PORT_BIT(7) |
                         PAL_PORT_BIT(10) |
                         PAL_PORT_BIT(12),
                         0,
                         PAL_STM32_OSPEED_MID2 |
                         PAL_MODE_ALTERNATE(6));
  palSetPadMode(GPIOA, 4, PAL_MODE_ALTERNATE(6) | PAL_STM32_OSPEED_MID2); /* WS  */

  /*
   * Initializes PWM driver 4
   */
  pwmStart(&PWMD4, &pwmcfg);
  palSetGroupMode(GPIOC, PAL_PORT_BIT(12) |
                         PAL_PORT_BIT(13) |
                         PAL_PORT_BIT(14),
                         0,
                         PAL_MODE_ALTERNATE(2));
  //palSetPadMode(GPIOD, 15, PAL_MODE_ALTERNATE(2));

  chThdCreateStatic(ledThreadWorkingArea, sizeof(ledThreadWorkingArea),
                    NORMALPRIO+1, ledThread, NULL);

  chThdCreateStatic(pwmThreadWorkingArea, sizeof(pwmThreadWorkingArea),
                    NORMALPRIO+2, pwmThread, NULL);

  /*
   * Configure I/Os for ADC1
   */
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0) |
                         PAL_PORT_BIT(2) |
                         PAL_PORT_BIT(3),
                         0,
                         PAL_MODE_INPUT_ANALOG);

  /*
   * Configure and activate ADC1
   */
  adcStart(&ADCD1, NULL);
  adcSTM32EnableTSVREFE();

  /* Begin asynchronous ADC conversion */
  adcStartConversion(&ADCD1, &adcgrpcfg2, samples2, ADC_GRP2_BUF_DEPTH);

  chEvtRegister(&inserted_event, &el0, 0);
  chEvtRegister(&removed_event, &el1, 1);
  chEvtRegister(&shell_terminated, &el2, 2);
   while (true) {
    if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE)) {
        /* Spawns a new shell.*/
        shelltp = chThdCreateFromHeap( NULL, SHELL_WA_SIZE,
                                       "shell", NORMALPRIO + 1,
                                       shellThread, (void *)&shell_cfg1);
    }
    chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));
  }
}
