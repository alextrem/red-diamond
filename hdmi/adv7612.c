/*
    ChibiOS/RT - Copyright (C) 2015 Alexander Geissler

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

/**
 * @file    adv7612.c
 * @brief   ADV7612 HDMI receiver through i2c interface
 *
 * @addtogroup adv7612
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "adv7612.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static uint8_t txbuf[2];
static uint8_t rxbuf[2];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Reads a register value.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI initerface
 * @param[in] reg       register number
 * @return              The register value.
 */
uint8_t adv7612ReadRegister(SPIDriver *spip, uint8_t reg) {

  spiSelect(spip);
  txbuf[0] = 0x80 | reg;
  txbuf[1] = 0xff;
  spiExchange(spip, 2, txbuf, rxbuf);
  spiUnselect(spip);
  return rxbuf[1];
}

/**
 * @brief   Writes a value into a register.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI initerface
 * @param[in] reg       register number
 * @param[in] value     the value to be written
 */
void adv7612WriteRegister(SPIDriver *spip, uint8_t reg, uint8_t value) {

  switch (reg) {
  default:
    /* Reserved register must not be written, according to the datasheet
       this could permanently damage the device.*/
    chDbgAssert(FALSE, "pcm1792aWriteRegister(), #1", "reserved register");

    /* Read only registers cannot be written, the command is ignored.*/
    spiSelect(spip);
    txbuf[0] = reg;
    txbuf[1] = value;
    spiSend(spip, 2, txbuf);
    spiUnselect(spip);
  }
}

/** @} */
