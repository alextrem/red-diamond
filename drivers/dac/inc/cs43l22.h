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
 * @file    cs43l22.h
 * @brief   CS43L22 ADC with I2S interface controlled through I2C
 *
 * @addtogroup pcm1792a
 * @{
 */

#ifndef _CS43L22_H_
#define _CS43L22_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct Dac {
  I2CDriver *i2cp;                      /** */
  uint32_t deviceID;                    /** chip device id */
} DAC_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t cs43l22ReadRegister(I2CDriver *i2cp, uint8_t reg);
  void cs43l22WriteRegister(I2CDriver *i2cp, uint8_t reg, uint8_t value);
#ifdef __cplusplus
}
#endif

#endif /* _PCM1792A_H_ */

/** @} */
