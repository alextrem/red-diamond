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
 * @brief   ADV7612 HDMI receiver through I2C interface
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
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp      pointer to the I2C initerface
 * @param[in] addr      address of the device
 * @param[in] reg       register number
 * @return              The register value.
 */
uint8_t adv7612ReadRegister(const HDMI_t *hdmi_cfg, uint8_t reg) {
#if I2C_USE_MUTUAL_EXCLUSION
  i2cAcquireBus(hdmi_cfg->i2cp);
#endif

#if I2C_USE_MUTUAL_EXCLUSION
  i2cReleaseBus(hdmi_cfg->i2cp);
#endif

}

/**
 * @brief   Writes a value into a register.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp      pointer to the I2C initerface
 * @param[in] addr      address of the device
 * @param[in] reg       register number
 * @param[in] value     the value to be written
 */
void adv7612WriteRegister(const HDMI_t *hdmi_cfg, uint8_t reg, uint8_t value) {
#if I2C_USE_MUTUAL_EXCLUSION
  i2cAcquireBus(hdmi_cfg->i2cp);
#endif

#if I2C_USE_MUTUAL_EXCLUSION
  i2cReleaseBus(hdmi_cfg->i2cp);
#endif
}

/** @} */
