/*
    red-diamond - Copyright (C) 2015 Alexander Geißler

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
 * @file    adv7611.h
 * @brief   ADV7611 HDMI audio interface via I2C
 *
 * @addtogroup adv7611
 * @{
 */

#ifndef _ADV7611_H_
#define _ADV7611_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    adv7611 register names
 * @{
 */
#define ADV7611_IO_REG_04               0x04
#define ADV7611_IO_REG_0B               0x0B
#define ADV7611_IO_REG_0C               0x0C


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
  void configureAudioInterface(HDMI_t const *hdmi_cfg);
  void initChip(HDMI_t const *hdmi_cfg);
#ifdef __cplusplus
}
#endif

#endif /* _ADV7611_H_ */

/** @} */
