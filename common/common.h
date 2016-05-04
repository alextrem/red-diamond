/*
    ChibiOS/RT - Copyright (C) 2016 Alexander Geißler

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.gnu.org/licenses/gpl-3.0.txt

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file common.h
 * @brief Common things for configuration and setup end up in this file
 *
 * @addtogroup common
 * @{
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

typedef enum samplerate {
  SR_8kHz = 0,
  SR_16kHz = 1,
  SR_32kHz = 2,
  SR_48kHz = 3,
  SR_96kHz = 4,
  SR_22k05Hz = 5,
  SR_44k1Hz = 6,
  SR_192kHz = 7
} SAMPLERATE_t;

typedef struct plli2s {
  uint32_t plli2sn;
  uint32_t plli2sr;
  uint8_t i2sdiv;
  uint8_t i2sodd;
} PLLI2S_t;

static PLLI2S_t pll_settings_16bit[8] = {
  {192, 2, 187, 1},
  {192, 3, 62, 1},
  {256, 3, 62, 1},
  {192, 5, 12, 1},
  {384, 5, 12, 1},
  {290, 3, 68, 1},
  {302, 2, 53, 1},
  {424, 3, 11, 1}
};

static PLLI2S_t pll_settings_32bit[8] = {
  {192, 3, 62, 1},
  {256, 2, 62, 1},
  {256, 5, 12, 1},
  {384, 5, 12, 1},
  {424, 3, 11, 1},
  {302, 2, 53, 1},
  {429, 4, 19, 0},
  {258, 3, 3, 1}
};

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define I2SCFGR_CLEAR_MASK  ((uint16_t)0xF040)
#define I2SPR_CLEAR_MASK    ((uint16_t)0xFC02)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

void Config_I2S(SPIDriver *spip, const SAMPLERATE_t samplerate, const uint8_t nbits);

#ifdef __cplusplus
}
#endif

#endif  /* _COMMON_H_ */

/** @} */
