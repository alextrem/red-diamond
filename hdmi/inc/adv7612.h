/*
    ChibiOS/RT - Copyright (C) 2015 Alexander Geißler

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
 * @file    adv7612.h
 * @brief   ADV7612 HDMI audio interface via I2C
 *
 * @addtogroup adv7612
 * @{
 */

#ifndef _ADV7612_H_
#define _ADV7612_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    adv7612 register names
 * @{
 */
#define ADV7612_IO_REG_04               0x04
#define ADV7612_IO_REG_0B               0x0B
#define ADV7612_IO_REG_0C               0x0C


/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct hdmi {
  I2CDriver *i2cp;                      /** interface driver */
  uint8_t IO_Map;                       /** address for IO Map  */
  uint8_t CP_MAP;                       /** address for CP Map  */
  uint8_t HDMI_Map;                     /** address for HDMI Map  */
  uint8_t Repeater_Map;                 /** address for Repeater Map */
  uint8_t EDID_Map;                     /** address for EDID Map  */
  uint8_t InfoFrame_Map;                /** address for InfoFrame Map */
  uint8_t CEC_Map;                      /** address for CEC Map  */
  uint8_t DPLL_Map;                     /** address for DPLL Map  */
  uint32_t reset;                       /** */
  uint16_t deviceID;                    /** chip device id */
} HDMI_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define CORE_PDN(x)         (x<<1)
#define XTAL_PDN(x)         (x)
#define POWER_DOWN(x)       (x<<6)
#define POWER_SAVE_MODE(x)  (x<<3)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t adv7612ReadRegister(SPIDriver *spip, uint8_t reg);
  void adv7612WriteRegister(SPIDriver *spip, uint8_t reg, uint8_t value);
#ifdef __cplusplus
}
#endif

#endif /* _ADV7612_H_ */

/** @} */
