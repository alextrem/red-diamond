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
static uint8_t Codec_ReadRegister(uint8_t address);
static uint32_t Codec_WriteRegister(uint8_t address, uint8_t value);

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static uint8_t rxbuf[2];
static uint8_t txbuf[2];
static DAC_t me;

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
 * @param[in] address
 *
 * @return  The value read
 */
static uint8_t Codec_ReadRegister(uint8_t address) {
  rxbuf[0] = address;
}

/**
 * @brief   Writes a value into a register.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] address   register number
 * @param[in] value     the value to be written
 */
static uint32_t Codec_WriteRegister(uint8_t address, uint8_t value) {

  txbuf[0] = address;
  txbuf[1] = value;

  /* Check if driver is assigned to a structure */

  i2cMasterTransmitTimeout(me.i2cp, CODEC_ADDRESS, txbuf, 2, NULL, 0, 1000);
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
  /* Assign used driver to structure*/
  me.i2cp = i2cp;

  Codec_GetID();

  /* Recommended initialization sequence from datasheet 4.11*/
  Codec_WriteRegister(0x00, 0x99);
  Codec_WriteRegister(0x47, 0x80);
  Codec_WriteRegister(0x32, 0x80);
  Codec_WriteRegister(0x32, 0x00);
  Codec_WriteRegister(0x00, 0x00);

  /* Set master volume */
  me.master_volume[0] = 0x00;
  me.master_volume[1] = 0x00;

  /* Set headphone volume */
  me.headphone_volume[0] = 0x00;
  me.headphone_volume[1] = 0x00;

  /* Set speaker volume */
  me.speaker_volume[0] = 0x01;
  me.speaker_volume[1] = 0x01;
}

/**
 * @brief   Recommended power down procedure
 */
void Codec_PowerDown(void) {
  /*TODO: Mute the DAC and PWM output */
  //Codec_WriteRegister();
  /* Disable soft ramp and zero cross volume transitions */
  Codec_WriteRegister(0x0A, 0x00);
  /* Power down the codec */
  Codec_WriteRegister(0x02, 0x9F);
}

/**
 * @brief   Configures the codec for playout
 */
void Codec_Configure(void) {
  /* Reset Codec register */
  Codec_Reset();

  /* Keep Codec powered OFF */
  Codec_WriteRegister(0x02, 0x01);

  /* Recommended initialization sequence from datasheet 4.11 */
  Codec_WriteRegister(0x00, 0x99);
  Codec_WriteRegister(0x47, 0x80);
  Codec_WriteRegister(0x32, 0x80);
  Codec_WriteRegister(0x32, 0x00);
  Codec_WriteRegister(0x00, 0x00);

  /* Speaker always OFF, Headphone always ON */
  Codec_WriteRegister(0x04, 0xAF);

  /* Clock configuration */
  Codec_WriteRegister(0x05, 0x81);

  /* Set slave mode and the audio interface standard */
  //Codec_WriteRegister();

  Codec_SetVolume(all);

  /* Disable the analog soft ramp */
}

/**
 * @brief   Get the ID of device
 */

void Codec_GetID(void) {
  me.deviceID = Codec_ReadRegister(0x00);
}

/**
 * @brief  Sets volume
 *
 * @param[in]   set
 * @param[in]   value
 */
void Codec_VolumeCtrl(const VOLUME_t set, uint8_t volume) {

}

/**
 * @brief   Mute outputs
 *
 * @param[in]   set
 */
void Codec_Mute(const VOLUME_t set) {
  switch(set) {
    case all:
      Codec_WriteRegister(CODEC_SPKA, 0x01);
      Codec_WriteRegister(CODEC_SPKB, 0x01);
      Codec_WriteRegister(CODEC_HPA, 0x01);
      Codec_WriteRegister(CODEC_HPB, 0x01);
      break;
    case master:
      break;
    case headphone:
      Codec_WriteRegister(CODEC_HPA, 0x01);
      Codec_WriteRegister(CODEC_HPB, 0x01);
      break;
    case speaker:
      Codec_WriteRegister(CODEC_SPKA, 0x01);
      Codec_WriteRegister(CODEC_SPKB, 0x01);
      break;
  }
}
/** @} */
