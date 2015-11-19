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

#define CODEC_ADDRESS 0x4A

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

typedef enum volume {
  all = 0,
  master = 1,
  headphone = 2,
  speaker = 3,
} VOLUME_t;

typedef struct Dac {
  I2CDriver *i2cp;                      /** */
  uint32_t deviceID;                    /** chip device id */
  uint8_t master_volume[2];             /** master volume control for l+r*/
  uint8_t headphone_volume[2];          /** headphone volume control for l+r */
  uint8_t speaker_volume[2];            /** speaker volume control for l+r*/
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

void Codec_Init(I2CDriver *i2cp);
void Codec_Configure(void);
void Codec_GetID(void);
void Codec_VolumeCtl(const VOLUME_t set, uint8_t volume);
void Codec_Mute(const VOLUME_t set);

#ifdef __cplusplus
}
#endif

#endif /* _CS43L22_H_ */

/** @} */
