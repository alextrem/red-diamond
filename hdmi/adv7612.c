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

static uint8_t rx_buf[2];

static const uint8_t IO_Map = 0x98;
static const uint8_t CP_Map = 0x12;
static const uint8_t HDMI_Map = 0x13;
static const uint8_t Repeater_Map = 0x14;
static const uint8_t EDID_Map = 0x15;
static const uint8_t InfoFrame_Map = 0x16;
static const uint8_t CEC_Map = 0x17;
static const uint8_t DPLL_Map = 0x18;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_slave_address(const HDMI_t *hdmi_cfg);

static void set_slave_address(const HDMI_t *hdmi_cfg) {
  adv7612WriteRegister(hdmi_cfg, IO_Map, 0xFD, &CP_Map);
  adv7612WriteRegister(hdmi_cfg, IO_Map, 0xFB, &HDMI_Map);
  adv7612WriteRegister(hdmi_cfg, IO_Map, 0xF9, &Repeater_Map);
  adv7612WriteRegister(hdmi_cfg, IO_Map, 0xFA, &EDID_Map);
  adv7612WriteRegister(hdmi_cfg, IO_Map, 0xF5, &InfoFrame_Map);
  adv7612WriteRegister(hdmi_cfg, IO_Map, 0xF4, &CEC_Map);
  adv7612WriteRegister(hdmi_cfg, IO_Map, 0xF8, &DPLL_Map);
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Reads a register value.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] hdmi_cfg  pointer to the I2C initerface
 * @param[in] device_address
 * @param[in] reg       register number
 * @return              The register value.
 */
uint8_t adv7612ReadRegister(const HDMI_t *hdmi_cfg,
                            uint8_t device_address,
                            uint8_t reg) {
  msg_t status = MSG_RESET;

#if I2C_USE_MUTUAL_EXCLUSION
  i2cAcquireBus(hdmi_cfg->i2cp);
#endif

  status = i2cMasterTransmitTimeout(hdmi_cfg->i2cp,
                                    device_address,
                                    &reg, 1,
                                    rx_buf, 1,
                                    MS2ST(1000));

#if I2C_USE_MUTUAL_EXCLUSION
  i2cReleaseBus(hdmi_cfg->i2cp);
#endif

  return status;
}

/**
 * @brief   Writes a value into a register.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] hdmi_cfg  pointer to the I2C initerface
 * @param[in] device_address
 * @param[in] reg       register number
 * @param[in] value     the value to be written
 */
void adv7612WriteRegister(const HDMI_t *hdmi_cfg,
                          uint8_t device_address,
                          uint8_t reg,
                          const uint8_t *value) {
  msg_t status = MSG_RESET;

  uint8_t request[2];
  request[0] = reg;
  request[1] = *value;

#if I2C_USE_MUTUAL_EXCLUSION
  i2cAcquireBus(hdmi_cfg->i2cp);
#endif

  status = i2cMasterTransmitTimeout(hdmi_cfg->i2cp,
                                    device_address,
                                    request, 2,
                                    NULL, 0,
                                    MS2ST(1000));

#if I2C_USE_MUTUAL_EXCLUSION
  i2cReleaseBus(hdmi_cfg->i2cp);
#endif
}

/** @} */
