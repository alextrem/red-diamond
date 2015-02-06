/*
    ChibiOS/RT - Copyright (C) 2006-2015 Alexander Geißler

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
 * @file    pcm1792a.h
 * @brief   PCM1792A ADC interface through SPI header.
 *
 * @addtogroup pcm1792a
 * @{
 */

#ifndef _PCM1792A_H_
#define _PCM1792A_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    PCM1792A register names
 * @{
 */
#define LIS302DL_WHO_AM_I               0x0F
#define LIS302DL_CTRL_REG1              0x20
#define LIS302DL_CTRL_REG2              0x21
#define LIS302DL_CTRL_REG3              0x22
#define LIS302DL_HP_FILTER_RESET        0x23
#define LIS302DL_STATUS_REG             0x27
#define LIS302DL_OUTX                   0x29
#define LIS302DL_OUTY                   0x2B
#define LIS302DL_OUTZ                   0x2D
#define LIS302DL_FF_WU_CFG1             0x30
#define LIS302DL_FF_WU_SRC1             0x31
#define LIS302DL_FF_WU_THS1             0x32
#define LIS302DL_FF_WU_DURATION1        0x33
#define LIS302DL_FF_WU_CFG2             0x34
#define LIS302DL_FF_WU_SRC2             0x35
#define LIS302DL_FF_WU_THS2             0x36
#define LIS302DL_FF_WU_DURATION2        0x37
#define LIS302DL_CLICK_CFG              0x38
#define LIS302DL_CLICK_SRC              0x39
#define LIS302DL_CLICK_THSY_X           0x3B
#define LIS302DL_CLICK_THSZ             0x3C
#define LIS302DL_CLICK_TIMELIMIT        0x3D
#define LIS302DL_CLICK_LATENCY          0x3E
#define LIS302DL_CLICK_WINDOW           0x3F
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

typedef struct AdcProxy ADC_t;
struct AdcProxy {
    uint32_t sampling;
    uint32_t attenuation;
    uint32_t deemphasis;
    uint32_t mute;
    uint32_t dac_enable;
    uint32_t rolloff;
    uint32_t reset;
}

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t pcm1792aReadRegister(SPIDriver *spip, uint8_t reg);
  void pcm1792aWriteRegister(SPIDriver *spip, uint8_t reg, uint8_t value);
  void Adc_initialize(ADC_t* const me);
  void Adc_configure(ADC_t* const me);
#ifdef __cplusplus
}
#endif

#endif /* _PCM1792A_H_ */

/** @} */
