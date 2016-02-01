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
 * @file    adv7611.c
 * @brief   ADV7611 HDMI receiver through I2C interface
 *
 * @addtogroup adv7611
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "adv7611.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static uint8_t adv7611ReadRegister(const HDMI_t *hdmi_cfg,
                                   uint8_t device_address,
                                   uint8_t reg);
static void adv7611WriteRegister(const HDMI_t *hdmi_cfg,
                                 uint8_t device_address,
                                 uint8_t reg,
                                 const uint8_t *value);
static void set_slave_address(HDMI_t const *hdmi_cfg);

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static uint8_t rx_buf[2];

static const uint8_t IO_Map = 0x98;
static const uint8_t CP_Map = 0xFD;
static const uint8_t HDMI_Map = 0x13;
static const uint8_t Repeater_Map = 0x14;
static const uint8_t EDID_Map = 0x15;
static const uint8_t InfoFrame_Map = 0x16;
static const uint8_t CEC_Map = 0x17;
static const uint8_t DPLL_Map = 0x18;

/*===========================================================================*/
/* Driver local functions.                                                   */
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
static uint8_t adv7611ReadRegister(const HDMI_t *hdmi_cfg,
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
static void adv7611WriteRegister(const HDMI_t *hdmi_cfg,
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

static void set_slave_address(HDMI_t const *hdmi_cfg) {
  adv7611WriteRegister(hdmi_cfg, IO_Map, 0xFD, &CP_Map);
  adv7611WriteRegister(hdmi_cfg, IO_Map, 0xFB, &HDMI_Map);
  adv7611WriteRegister(hdmi_cfg, IO_Map, 0xF9, &Repeater_Map);
  adv7611WriteRegister(hdmi_cfg, IO_Map, 0xFA, &EDID_Map);
  adv7611WriteRegister(hdmi_cfg, IO_Map, 0xF5, &InfoFrame_Map);
  adv7611WriteRegister(hdmi_cfg, IO_Map, 0xF4, &CEC_Map);
  adv7611WriteRegister(hdmi_cfg, IO_Map, 0xF8, &DPLL_Map);
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Configure audio interface.
 * @pre     The I2C interface must be initialized and the driver started.
 */
void configureAudioInterface(HDMI_t const *hdmi_cfg) {
  //TODO: Registers which need to be wriiten need to be done
  // adv7611WriteRegister(const HDMI_t *hdmi_cfg, IO_Map);
}

/**
 * @brief   Initialize chip.
 * @pre     The I2C interface must be initialized and the driver started.
 */
void init_chip(HDMI_t const *hdmi_cfg) {
  /* Configure I2C interface addresses */
  set_slave_address(hdmi_cfg);
  //TODO: Registers which need to be wriiten need to be done
  // adv7611WriteRegister(const HDMI_t *hdmi_cfg, IO_Map);
}

/** @} */
