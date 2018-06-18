/*
    red-diamond - Copyright (C) 2015 Alexander Geissler

    Licensed under the GPL License, Version 3.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.gnu.org/licenses/gpl-3.0.txt

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    cs43l22.c
 * @brief   CS43L22 Audio DAC with I2S interface and controlled through I2C
 *
 * @addtogroup cs43l22
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "cs43l22.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static void Codec_Reset(void);
static msg_t Codec_ReadRegister(I2CDriver *i2cp, cs43l22_sad_t sad,
                               uint8_t reg, uint8_t *rxbuf, size_t n);
static msg_t Codec_WriteRegister(I2CDriver *i2cp, cs43l22_sad_t sad,
                                 uint8_t *txbuf, size_t n);

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static uint8_t rxbuf[2];
static uint8_t txbuf[2];
static const DAC_CS43L22_t factory_default = {
  .master_volume = {0,0},
  .headphone_volume = {0,0},
  .speaker_volume = {0,0},
  .beep = {.frequency = BEEP_F1000,
           .ontime = beep_on_5200ms,
           .offtime = beep_off_1230ms,
           .config = off,
           .volume = 0x00}
};
static DAC_CS43L22_t dac;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Resets the device
 */
static void Codec_Reset(void) {
  /* Power down the codec */
  palClearPad(GPIOD, 4);
  /*  */
  chThdSleepMilliseconds(10);
  /* Power on codec */
  palSetPad(GPIOD, 4);
}

/**
 * @brief   Reads a value from the Codec
 * @pre     The I2C interface must be initialized and the driver started
 *
 * @param[in] i2cp      Pointer to the I2C interface
 * @param[in] sad       Slave address without R bit
 * @param[in] reg       first sub-register address
 * @param[out] rxbuf    pointer to an output buffer
 * @param[in] n         number of consecutive registers to read
 * @return              the operation status
 * @notapi
 *
 */
static msg_t Codec_ReadRegister(I2CDriver *i2cp, cs43l22_sad_t sad, uint8_t reg,
                                  uint8_t *rxbuf, size_t n) {
  if (n > 1)
      reg |= CODEC_I2C_AUTOINCR;
  uint8_t buf = reg;

  return i2cMasterTransmitTimeout(i2cp, sad, &buf, 1, rxbuf, 1, MS2ST(4));
}

/**
 * @brief   Writes a value into a register.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp      pointer to the I2C interface
 * @param[in] sad       slave address without R bit
 * @param[in] txbuf     buffer containing the sub-address and the data
 *                      to write
 * @param[in] n         Number of bytes to write not considering the first
 *                      element
 * @return              the operation status
 * @notapi
 */
static msg_t Codec_WriteRegister(I2CDriver *i2cp, cs43l22_sad_t sad,
                                 uint8_t *txbuf, size_t n) {
  if (n > 1)
      (*txbuf) |= CODEC_I2C_AUTOINCR;

  /* Check if driver is assigned to a structure */
  return i2cMasterTransmitTimeout(i2cp, sad, txbuf, n , NULL, 0, MS2ST(4));
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the codec data structure
 * @pre     The I2C interface must be initialized and the driver started
 * @note    To keep the driver assignment flexible the assignment of the
 *          interface will be done in this function
 *
 * @param[in] i2cp  pointer to the I2C driver structure
 */
void Codec_Init(I2CDriver *i2cp) {
  //TODO: Check for previous settings

  Codec_GetID(i2cp);

  /* Recommended initialization sequence from datasheet 4.11*/
  txbuf[0] = 0x00; txbuf[1] = 0x99;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x47; txbuf[1] = 0x80;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x32; txbuf[1] = 0x80;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x32; txbuf[1] = 0x00;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x00; txbuf[1] = 0x00;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Set master volume */
  dac.master_volume[0] = 0x0A;
  dac.master_volume[1] = 0x0A;

  /* Set headphone volume */
  dac.headphone_volume[0] = 0x00;
  dac.headphone_volume[1] = 0x00;

  /* Set speaker volume */
  dac.speaker_volume[0] = 0x01;
  dac.speaker_volume[1] = 0x01;
}

/**
 * @brief   Recommended power down procedure
 */
void Codec_PowerDown(I2CDriver *i2cp) {
  /*TODO: Mute the DAC and PWM output */
  //Codec_WriteRegister();
  /* Disable soft ramp and zero cross volume transitions */
  txbuf[0] = 0x0A; txbuf[1] = 0x00;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  /* Power down the codec */
  txbuf[0] = 0x02; txbuf[1] = 0x9F;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
}

/**
 * @brief   Configures the codec for playout
 */
void Codec_Configure(I2CDriver *i2cp) {
  /* Reset Codec register */
  Codec_Reset();

  /* Keep Codec powered OFF */
  txbuf[0] = CODEC_POWER_CTL1; txbuf[1] = 0x01;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Recommended initialization sequence from datasheet 4.11 */
  txbuf[0] = 0x00; txbuf[1] = 0x99;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x47; txbuf[1] = 0x80;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x32; txbuf[1] = 0x80;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x32; txbuf[1] = 0x00;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
  txbuf[0] = 0x00; txbuf[1] = 0x00;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Speaker always OFF, Headphone always ON */
  txbuf[0] = CODEC_POWER_CTL2; txbuf[1] = 0xAF;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Clock configuration */
  txbuf[0] = CODEC_CLOCK_CTL; txbuf[1] = 0x81;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Set slave mode and the audio interface standard */
  txbuf[0] = CODEC_INTERFACE_CTL1; txbuf[1] = CODEC_STANDARD;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Set the volume on all outputs */
  //Codec_SetVolume(all);

  /* Power on codec */
  txbuf[0] = CODEC_POWER_CTL1; txbuf[1] = 0x9E;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Disable the analog soft ramp */
  txbuf[0] = CODEC_SOFT_RAMP; txbuf[1] = 0x00;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Disable the limiter attack level */
  txbuf[0] = CODEC_LIMITER_CTRL1; txbuf[1] = 0x00;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Adjust base and treble levels */
  txbuf[0] = CODEC_TONE_CTRL; txbuf[1] = 0x0F;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
}

/**
 * @brief   Get the ID and Revision of device
 */
uint8_t Codec_GetID(I2CDriver *i2cp) {
  msg_t status = MSG_OK;

  status = Codec_ReadRegister(i2cp, CS43L22_SAD_GND, CODEC_ID, &rxbuf[0], 1);
//  if (status != MSG_OK)
//    return status;
//  else
  return rxbuf[0];
}

/**
 * @brief   Sets volume of the codec for all, headphone or speaker
 * @note    The I2C interface must be configured
 *
 * @param[in]   i2cp    pointer to the I2C driver
 * @param[in]   set     set volume for headphone, speaker or both
 * @param[in]   volume  volume value
 */
void Codec_VolumeCtrl(I2CDriver *i2cp, OUTPUT_t set, uint8_t volume) {
  switch(set) {
    case all:
      txbuf[0] = CODEC_SPKA; txbuf[1] = volume;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_SPKB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_HPA;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_HPB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      break;
    case master:
      break;
    case headphone:
      txbuf[0] = CODEC_HPA; txbuf[1] = volume;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_HPB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      break;
    case speaker:
      txbuf[0] = CODEC_SPKA; txbuf[1] = volume;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_SPKB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      break;
  }
}

/**
 * @brief   Sets balance of the outputs. For a negative balance value right
 *          channel will be attenuated. For a positive otherwise
 *
 * @param[in]   i2cp    pointer to the I2C driver
 * @param[in]   set     set volume for headphone, speaker or both
 * @param[in]   balance value to set balance
 */
void Codec_Balance(I2CDriver *i2cp, const OUTPUT_t set, int8_t balance) {

  switch(set) {
    case all:
      break;
    case master:
      break;
    case headphone:
      if ( balance < 0 ) {
        txbuf[0] = CODEC_HPA; txbuf[1] = balance;
        Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      }
      else {
        txbuf[0] = CODEC_HPB; txbuf[1] = balance;
        Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      }
      break;
    case speaker:
      break;
  }
}

/**
 * @brief   Mute outputs
 *
 * @param[in]   i2cp    pointer to the I2C driver
 * @param[in]   set     mute output for headphone speaker or both
 */
void Codec_Mute(I2CDriver *i2cp, const OUTPUT_t set) {
  switch(set) {
    case all:
      txbuf[0] = CODEC_SPKA; txbuf[1] = 0x01;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_SPKB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_HPA;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_HPB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      break;
    case master:
      break;
    case headphone:
      txbuf[0] = CODEC_HPA; txbuf[1] = 0x01;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_HPB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      break;
    case speaker:
      txbuf[0] = CODEC_SPKA; txbuf[1] = 0x01;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      txbuf[0] = CODEC_SPKB;
      Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
      break;
  }
}

/**
 * @brief   Configure beep generator
 *
 * @param[in]   i2cp    pointer to the I2C driver
 */
void Codec_BeepGenerator(I2CDriver *i2cp) {
  //beep.frequency = BEEP_F1000;
  //beep.ontime = beep_on_1200ms;
  //beep.offtime = beep_off_2580ms;
  //beep.config = continous;

  /* Set beep frequency */
  txbuf[0] = CODEC_BEEP_FREQ_ON_TIME; txbuf[1] = 0x73;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));

  /* Continous Beep mode */
  txbuf[0] = CODEC_BEEP_TONE_CFG; txbuf[1] = 0xC0;
  Codec_WriteRegister(i2cp, CS43L22_SAD_GND, &txbuf[0], sizeof(txbuf));
}

/**
 * @brief   Set Codec to default system settings
 *
 * @param[in]   dac
 */
void Codec_FactoryDefault(void) {
  dac = factory_default;
}
/** @} */
