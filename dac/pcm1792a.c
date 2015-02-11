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
 * @file    pcm1792a.c
 * @brief   PCM1792A Audio DAC interface through SPI code.
 *
 * @addtogroup pcm1792a
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "pcm1792a.h"

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
uint8_t pcm1792aReadRegister(SPIDriver *spip, uint8_t reg) {

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
void pcm1792aWriteRegister(SPIDriver *spip, uint8_t reg, uint8_t value) {

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

/**
 * @brief Initilize DAC with system default values
 *
 * @param[in] me    pointer tp the DAC instance
 */
void DAC_initialize(DAC_t* const me) {

}

/**
 * @brief Configures DAC
 */
void DAC_configure(DAC_t* const me) {

}

/**
 * @brief Attenuation is performed symetricaly on left and right channels
 *
 * @param[in] me            pointer to the DAC instance
 * @param[in] attenuation   attenuation value in 0.5 dB steps
 */
void DAC_attenuate(DAC_t* const me, const uint8_t attenuation) {
  me->attenuation = attenuation;

  pcm1792aWriteRegister(&SPID1, PCM1792A_ATTENUATION_LEFT, me->attenuation);
  pcm1792aWriteRegister(&SPID1, PCM1792A_ATTENUATION_RIGHT, me->attenuation);

  pcm1792aWriteRegister(&SPID1, PCM1792A_ATTENUATION_LOAD_CTRL, PCM1792A_ATLD(1));
  pcm1792aWriteRegister(&SPID1, PCM1792A_ATTENUATION_LOAD_CTRL, PCM1792A_ATLD(0));
}

/**
 * @brief mutes the output
 *
 * @param[in] me
 */

void DAC_mute(DAC_t* const me) {

}

void DAC_SetInterface(DAC_t* const me, AudioFormat_t format) {

}

/**
 * @brief Get the DACs device ID
 *
 * @param[in] me    pointer to DAC instance
 */
void DAC_deviceID(DAC_t* const me) {
    me->deviceID = pcm1792aReadRegister(&SPID1, PCM1792A_DEVICEID);
}

/** @} */
