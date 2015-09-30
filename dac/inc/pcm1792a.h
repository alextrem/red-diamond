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
#define PCM1792A_ATTENUATION_LEFT       16
#define PCM1792A_ATTENUATION_RIGHT      17
#define PCM1792A_ATTENUATION_LOAD_CTRL  18
#define PCM1792A_AUDIO_INTERFACE        19
#define PCM1792A_DEEMPHASIS             20
#define PCM1792A_DEVICEID               21
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

typedef enum AudioFormat {
  RIGHT_JUSTIFIED_16BIT,  /** Right justified 16 bit data */
  RIGHT_JUSTIFIED_20BIT,  /** Right justified 20 bit data */
  RIGHT_JUSTIFIED_24BIT,  /** Right justified 24 bit data */
  LEFT_JUSTIFIED_24BIT,   /** Left justified 24 bit data MSB */
  I2S_16BIT,              /** I2S with 16 bit data */
  I2S_24BIT               /** I2S with 24 bit data */
} AudioFormat_t;

typedef enum AttenuationRate {
  LRCK1, /**  default */
  LRCK2, /**  */
  LRCK4, /**  */
  LRCK8  /**  */
} AttenuationRate_t;

typedef enum OversamplingRate {
  FS_64,  /** Oversampling 64 times the sampling rate */
  FS_32,  /** Oversampling 32 times the sampling rate */
  FS_128  /** Oversampling 128 times the sampling rate */
} Oversampling_t;

typedef struct Dac {
  SPIDriver *spip;                      /** */
  uint32_t sampling;                    /** chosen samplingrate for the dac */
  uint8_t attenuation;                  /** attenuation steps in 0.5 dB */
  AttenuationRate_t attenuation_rate;   /** */
  AudioFormat_t audio_format;           /** I2S, right or left justified */
  uint32_t deemphasis;                  /** */
  uint32_t mute;                        /** soft mute */
  uint32_t dac_enable;                  /** activate dac output */
  uint32_t rolloff;                     /** set filter roll off */
  uint32_t reset;                       /** */
  Oversampling_t oversampling;          /** choose from three oversampling rates */
  uint32_t deviceID;                    /** chip device id */
} DAC_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define PCM1792A_ATLD(x)                (x & 0x80)
#define PCM1792A_FMT(x)                 (x<<4 & 0x70)
#define PCM1792A_DFM(x)                 (x<<2 & 0x0C)
#define PCM1792A_OS(x)                  (x & 0x03)

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
  void DAC_attenuate(DAC_t* const me, uint8_t attenuation);
  void DAC_SetOversampling(DAC_t* const me);
  void DAC_SetBalance(DAC_t* const me);
  void DAC_SetAttenuationRate(DAC_t* const me, AttenuationRate_t rate);
  void DAC_SetInterface(DAC_t* const me, AudioFormat_t format);
  void DAC_outputEnable(DAC_t* const me);
  void DAC_deviceID(DAC_t* const me);
#ifdef __cplusplus
}
#endif

#endif /* _PCM1792A_H_ */

/** @} */
