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
 * @file    cs43l22.h
 * @brief   CS43L22 ADC with I2S interface controlled through I2C
 *
 * @addtogroup cs43l22
 * @{
 */

#ifndef _CS43L22_H_
#define _CS43L22_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
/**
 * @name    Version identification
 * @{
 */
/**
 * @brief   CS43L22 driver version string.
 */
#define EX_CS43L22_VERSION                 "0.1.0"

/**
 * @brief   LIS302DL driver version major number.
 */
#define EX_CS43L22_MAJOR                   0

/**
 * @brief   LIS302DL driver version minor number.
 */
#define EX_CS43L22_MINOR                   1

/**
 * @brief   LIS302DL driver version patch number.
 */
#define EX_CS43L22_PATCH                   0
/** @} */

#define CODEC_STANDARD              0x04
#define CODEC_ADDRESS               0x94
#define CODEC_I2C_AUTOINCR          (1 << 7)

#define CODEC_ID                    0x01
#define CODEC_POWER_CTL1            0x02
#define CODEC_POWER_CTL2            0x04
#define CODEC_CLOCK_CTL             0x05
#define CODEC_INTERFACE_CTL1        0x06
#define CODEC_INTERFACE_CTL2        0x07
#define CODEC_PASS_A                0x08
#define CODEC_PASS_B                0x09
#define CODEC_SOFT_RAMP             0x0A
#define CODEC_PASS_GANG_CTRL        0x0C
#define CODEC_PLAYBACK_CTL1         0x0D
#define CODEC_MISC_CTL              0x0E
#define CODEC_PLAYBACK_CTL2         0x0F
#define CODEC_PASS_A_VOL            0x14
#define CODEC_PASS_B_VOL            0x15
#define CODEC_PCM_A                 0x1A
#define CODEC_PCM_B                 0x1B
#define CODEC_BEEP_FREQ_ON_TIME     0x1C
#define CODEC_BEEP_VOL_OFF_TIME     0x1D
#define CODEC_BEEP_TONE_CFG         0x1E
#define CODEC_TONE_CTRL             0x1F
#define CODEC_MASTER_VOLUME_CTRL_A  0x20
#define CODEC_MASTER_VOLUME_CTRL_B  0x21
#define CODEC_HPA                   0x22
#define CODEC_HPB                   0x23
#define CODEC_SPKA                  0x24
#define CODEC_SPKB                  0x25
#define CODEC_PCM_CHANNEL_SWAP      0x26
#define CODEC_LIMITER_CTRL1         0x27
#define CODEC_LIMITER_CTRL2         0x28
#define CODEC_LIMITER_ATTACK        0x29
#define CODEC_STATUS                0x2E
#define CODEC_BATTERY_COMPENSATION  0x2F
#define CODEC_VP_BATTERY_LEVEL      0x30
#define CODEC_SPEAKER_STATUS        0x31
#define CODEC_CHARGE_PUMP_FREQ      0x34

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/
/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   CS43L22 I2C interface switch.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p TRUE.
 */
#if !defined(CS43L22_USE_I2C) || defined(__DOXYGEN__)
#define CS43L22_USE_I2C                    TRUE
#endif

/**
 * @brief   CS43L22 shared I2C switch.
 * @details If set to @p TRUE the device acquires I2C bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires I2C_USE_MUTUAL_EXCLUSION.
 */
#if !defined(CS43L22_SHARED_SPI) || defined(__DOXYGEN__)
#define CS43L22_SHARED_I2C                 FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/
#if CS43L22_USE_I2C && !HAL_USE_I2C
#error "CS43L22_USE_I2C requires HAL_USE_I2C"
#endif

#if CS43L22_SHARED_I2C && !I2C_USE_MUTUAL_EXCLUSION
#error "CS43L22_SHARED_I2C requires I2C_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/
typedef enum {
  CS43L22_SAD_GND = 0x94,           /**< Slave Address when SA0 is to GND    */
  CS43L22_SAD_VCC = 0x96            /**< Slave Address when SA0 is to VCC    */
}cs43l22_sad_t;

typedef enum output {
  all = 0,                          /**< All outputs */
  master = 1,                       /**< Master Output */
  headphone = 2,                    /**< Headphone output */
  speaker = 3,                      /**< Speaker output */
} OUTPUT_t;

typedef enum beep_config {
  off = 0,                          /** Deactivate beep */
  single = 1,                       /** Just on shot beep */
  multiple = 2,                     /** A bunch of beeps */
  continous = 3,                    /** Beep all the time */
} BEEP_CONFIG_t;

typedef enum beep_ontime {
  beep_on_83ms = 0x00,              /** 83ms beep */
  beep_on_430ms = 0x01,             /** 430ms beep */
  beep_on_780ms = 0x02,             /** 780ms beep */
  beep_on_1200ms = 0x03,            /** 1,2s beep */
  beep_on_1500ms = 0x04,            /** 1,5s beep */
  beep_on_1800ms = 0x05,            /** 1,8s beep */
  beep_on_2200ms = 0x06,            /** 2,2s beep */
  beep_on_2500ms = 0x07,            /** 2,5s beep */
  beep_on_2800ms = 0x08,            /** 2,8s beep */
  bepp_on_3200ms = 0x09,            /** 3,2s beep */
  beep_on_3500ms = 0x0A,            /** 3,5s beep */
  beep_on_3800ms = 0x0B,            /** 3,8s beep */
  beep_on_4200ms = 0x0C,            /** 4,2s beep */
  beep_on_4500ms = 0x0D,            /** 4,5s beep */
  beep_on_4800ms = 0x0E,            /** 4,8s beep */
  beep_on_5200ms = 0x0F             /** 5,2s beep */
} ONTIME_t;

typedef enum beep_offtime {
  beep_off_1230ms = 0,              /** 1,23s beep off */
  beep_off_2580ms = 1,              /** 2,58s beep off */
  beep_off_3900ms = 2,              /** 3,9s beep off */
  beep_off_5200ms = 3,              /** 5,2s beep off */
  beep_off_6600ms = 4,              /** 6,6s beep off */
  beep_off_8050ms = 5,              /** 8,05s beep off */
  beep_off_9350ms = 6,              /** 9,35s beep off */
  beep_off_10800ms = 7              /** 10,80s beep off */
} OFFTIME_t;

typedef struct beep {
  uint8_t frequency;                /** Choose e a frequency from the macros*/
  ONTIME_t ontime;                  /** Beep ontime setting */
  OFFTIME_t offtime;                /** Beep offtime setting */
  uint8_t volume;                   /** Beep volume */
  BEEP_CONFIG_t configuration;      /** Beep configuration structure */
  uint8_t mix;                      /** Activate or deactivate beep mix */
} BEEP_t;

typedef struct Dac {
  I2CDriver *i2cp;                  /** Driver attachement*/
  uint8_t deviceID;                 /** chip device id */
  uint8_t master_volume[2];         /** Master volume control for l+r */
  uint8_t headphone_volume[2];      /** Headphone volume control for l+r */
  uint8_t speaker_volume[2];        /** Speaker volume control for l+r */
  BEEP_t beep;                      /** Beep structure */
} DAC_CS43L22_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define BEEP_F260 0x00      /** Beep frequency 260 Hz*/
#define BEEP_F521 0x10      /** Beep frequency 521 Hz*/
#define BEEP_F585 0x20      /** Beep frequency 585 Hz*/
#define BEEP_F666 0x30      /** Beep frequency from hell */
#define BEEP_F705 0x40      /** Beep frequency 705 Hz*/
#define BEEP_F774 0x50      /** Beep frequency 774 Hz*/
#define BEEP_F888 0x60      /** Beep frequency 888 Hz*/
#define BEEP_F1000 0x70     /** Beep frequency 1 kHz*/
#define BEEP_F1043 0x80     /** Beep frequency 1043 Hz */
#define BEEP_F1200 0x90     /** Beep frequency 1200 Hz*/
#define BEEP_F1333 0xA0     /** Beep frequency 1333 Hz*/
#define BEEP_F1411 0xB0     /** Beep frequency 1411 Hz*/
#define BEEP_F1600 0xC0     /** Beep frequency 1600 Hz*/
#define BEEP_F1714 0xD0     /** Beep frequency 1714 Hz */
#define BEEP_F2000 0xE0     /** Beep frequency 2 kHz Hz*/
#define BEEP_F2181 0xF0     /** Beep frequency 2181 Hz*/

#define ID()
#define REVISION()

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

void Codec_Init(I2CDriver *i2cp);
void Codec_Configure(I2CDriver *i2cp);
void Codec_GetID(I2CDriver *i2cp);
void Codec_VolumeCtl(I2CDriver *i2cp, OUTPUT_t set, uint8_t volume);
void Codec_Balance(I2CDriver *i2cp, const OUTPUT_t set, int8_t balance);
void Codec_Mute(I2CDriver *i2cp, const OUTPUT_t set);
void Codec_BeepGenerator(DAC_CS43L22_t *dac);
void Codec_BeepSetFrequency(DAC_CS43L22_t *dac);
void Codec_FactoryDefault(void);

#ifdef __cplusplus
}
#endif

#endif /* _CS43L22_H_ */

/** @} */
