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
 * @file    max30101.c
 * @brief   MAX30101 Pulse Oximeter controlled through I2C
 *
 * @addtogroup max30101
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "max30101.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static uint8_t Sensor_ReadRegister(uint8_t address);
static msg_t Sensor_WriteRegister(uint8_t address, uint8_t value);

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static uint8_t rxbuf[2];
static uint8_t txbuf[2];
static OXI_MAX30101_t oxi;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Reads a value from the Sensor
 * @pre     The I2C interface must be initialized and the driver started
 *
 * @param[in] address
 *
 */
static uint8_t Sensor_ReadRegister(uint8_t address) {
  /* Check if driver is assigned to a structure */
  txbuf[0] = address;

  msg_t msg = i2cMasterTransmitTimeout(oxi.i2cp,
                                       SENSOR_ADDRESS,
                                       txbuf,
                                       1,
                                       NULL,
                                       0,
                                       MS2ST(4));

  if (msg != MSG_OK) {
  }

  msg = i2cMasterReceiveTimeout(oxi.i2cp,
                                SENSOR_ADDRESS,
                                rxbuf,
                                1,
                                MS2ST(4));

  if (msg != MSG_OK) {
  }

  uint8_t data = rxbuf[0];

  return data;
}

/**
 * @brief   Writes a value into a register.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] address   register which should be written
 * @param[in] value     the value to be written
 */
static msg_t Sensor_WriteRegister(uint8_t address, uint8_t value) {
  txbuf[0] = address;
  txbuf[1] = value;

  /* Check if driver is assigned to a structure */
  msg_t msg = i2cMasterTransmitTimeout(oxi.i2cp,
                                       SENSOR_ADDRESS,
                                       txbuf,
                                       sizeof(txbuf),
                                       NULL,
                                       0,
                                       MS2ST(4));

  if (msg != MSG_OK) {
  }

  return msg;
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
void MAX30101_Init(I2CDriver *i2cp) {
  /* Assign used driver to structure*/
  oxi.i2cp = i2cp;

  MAX30101_GetID();
}


/**
 * @brief   Configures the codec for playout
 */
void MAX30101_Configure(void) {

}

/**
 * @brief   Get the ID and Revision of device
 */
void MAX30101_GetID(void) {
  oxi.revisionID = Sensor_ReadRegister(REG_REVISION_ID);
  oxi.partID = Sensor_ReadRegister(REG_PART_ID);
}


/** @} */
