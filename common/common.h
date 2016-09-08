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

/**
 * @brief   samplerate enum
 */
typedef enum samplerate {
  SR_8kHz = 0,      /**< @brief 8 kHz samplerate */
  SR_16kHz = 1,     /**< @brief 16 kHz samplerate */
  SR_32kHz = 2,     /**< @brief 32 kHz samplerate  */
  SR_48kHz = 3,     /**< @brief 48 kHz samplerate */
  SR_96kHz = 4,     /**< @brief 96 kHz samplerate */
  SR_22k05Hz = 5,   /**< @brief 22,05 kHz samplerate */
  SR_44k1Hz = 6,    /**< @brief 44,1 kHz samplerate */
  SR_192kHz = 7     /**< @brief 192 kHz samplerate */
} SAMPLERATE_t;

typedef enum nbits {
  BIT_16 = 0,
  BIT_32 = 1
} BITS_t;

/**
 * @brief   I2S pll structure
 */
typedef struct plli2s {
  uint32_t plli2sn;     /**< @brief I2S PLL N multiplier value */
  uint32_t plli2sr;     /**< @brief I2S PLL R divider value */
  uint8_t i2sdiv;       /**< @brief linear prescaler */
  uint8_t i2sodd;       /**< @brief  */
} PLLI2S_t;

static const PLLI2S_t pll_settings_16bit[8] = {
  {192, 2, 187, 1},
  {192, 3, 62, 1},
  {256, 3, 62, 1},
  {192, 5, 12, 1},
  {384, 5, 12, 1},
  {290, 3, 68, 1},
  {302, 2, 53, 1},
  {424, 3, 11, 1}
};

static const PLLI2S_t pll_settings_32bit[8] = {
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

void Config_I2S(I2SDriver *i2sp, const SAMPLERATE_t samplerate, const BITS_t nbits);

#ifdef __cplusplus
}
#endif

#endif  /* _COMMON_H_ */

/** @} */
