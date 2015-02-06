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
#define PCM1792A_ATTENUATION_LEFT       0x10
#define PCM1792A_ATTENUATION_RIGHT      0x11
#define PCM1792A_ATTENUATION_CTRL       0x12
#define PCM1792A_AUDIO_INTERFACE        0x13
#define PCM1792A_DEEMPHASIS             0x14
#define PCM1792A_DEVICEID               0x17


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

typedef struct DacProxy {
    uint32_t sampling;    /* ! chosen samplingrate for the dac */
    uint32_t attenuation; /* ! attenuation steps in 0.5 dB */
    uint32_t audio_format /* ! */
    uint32_t deemphasis;  /* ! */
    uint32_t mute;        /* ! soft mute */
    uint32_t dac_enable;  /* ! */
    uint32_t rolloff;     /* ! */
    uint32_t reset;
    uint32_t deviceID;    /* ! */
} DAC_t;

typedef enum AudioFormat {
    RIGHT_JUSTIFIED_16BIT,  /* ! Right justified 16 bit data */
    RIGHT_JUSTIFIED_20BIT,  /* ! Right justified 20 bit data */
    RIGHT_JUSTIFIED_24BIT,  /* ! Right justified 24 bit data */
    LEFT_JUSTIFIED_24BIT,   /* ! Left justified 24 bit data MSB */
    I2S_16BIT,              /* ! I2S with 16 bit data */
    I2S_24BIT               /* ! I2S with 24 bit data */
} AudioFormat_t;

typedef enum AttenuationRate {
    LRCK1,
    LRCK2,
    LRCK4,
    LRCK8
} AttenuationRate_t;

typedef enum OversamplingRate {
    FS_64,  /* ! Oversampling 64 times the sampling rate */
    FS_32,  /* ! Oversampling 32 times the sampling rate */
    FS_128  /* ! Oversampling 128 times the sampling rate */
} Oversampling_t;

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
  void DAC_initialize(DAC_t* const me);
  void DAC_configure(DAC_t* const me);
  void DAC_attenuate(DAC_t* const me);
  void DAC_outputEnable(DAC_t* const me);
  void DAC_deviceID(DAC_t* const me);
#ifdef __cplusplus
}
#endif

#endif /* _PCM1792A_H_ */

/** @} */
