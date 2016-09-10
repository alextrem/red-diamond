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

#define CODEC_STANDARD 0x04
#define CODEC_ADDRESS 0x94

#define CODEC_ID   0x01
#define CODEC_HPC  0x04

#define CODEC_HPA  0x22
#define CODEC_HPB  0x23
#define CODEC_SPKA 0x24
#define CODEC_SPKB 0x25

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef enum output {
  all = 0,                          /** All outputs */
  master = 1,                       /** Master Output */
  headphone = 2,                    /** Headphone output */
  speaker = 3,                      /** Speaker output */
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
void Codec_Configure(void);
void Codec_GetID(void);
void Codec_VolumeCtl(OUTPUT_t set, uint8_t volume);
void Codec_Balance(const OUTPUT_t set, int8_t balance);
void Codec_Mute(const OUTPUT_t set);
void Codec_BeepGenerator(DAC_CS43L22_t *dac);
void Codec_BeepSetFrequency(DAC_CS43L22_t *dac);
void Codec_FactoryDefault(DAC_CS43L22_t *dac);

#ifdef __cplusplus
}
#endif

#endif /* _CS43L22_H_ */

/** @} */
