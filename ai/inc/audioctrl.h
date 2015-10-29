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
 * @file    audioctrl.h
 * @brief   Audioctrl of the amplifier
 *
 * @addtogroup pcm1792a
 * @{
 */

#ifndef _AUDIOCTRL_H_
#define _AUDIOCTRL_H_

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

typedef enum source {
  AUX  = 1,
  CD   = 2,
  HDMI = 3,
  FM = 4,
  AM = 5,
  DAB = 6
} Source_t;

typedef struct audioctrl {
  uint8_t volume;
  int8_t balance;
  Source_t source;
} AudioCtrl_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void set_balance(AudioCtrl_t* const me);
void set_volume(AudioCtrl_t* const me);
void set_filter(AudioCtrl_t * const me);
void choose_source(AudioCtrl_t* const me);
#ifdef __cplusplus
}
#endif

#endif /* _AUDIOCTRL_H_ */

/** @} */
