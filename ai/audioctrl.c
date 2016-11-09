/*
    ChibiOS/RT - Copyright (C) 2015 Alexander Geissler

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
 * @file    audioctrl.c
 * @brief   Audiocontrol for amplifier
 *
 * @addtogroup control
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "audioctrl.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const AudioCtrl_t factory_default = {
  .balance = 0,
  .volume = 20,
  .source = CD,
}; //! If restore to default will be used

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Sets the balance of the audio output
 *
 * @param[in]   ctrl
 */
void set_balance(AudioCtrl_t* const ctrl) {
 /* TODO: make those user settings store in flash */
}

/**
 * @brief Sets the volume of the audio output
 *
 * @param[in]   ctrl
 */
void set_volume(AudioCtrl_t* const ctrl) {
  /* TODO: make those user settings store in flash */
}

void set_source(AudioCtrl_t* const ctrl) {
 /* TODO: make those usersettings store in flash */
}

/**
 * @brief restore factory default settings
 *
 * @param[in]   ctrl
 */
void set_factory_default (AudioCtrl_t* ctrl) {
  ctrl->balance = factory_default.balance;
  ctrl->volume = factory_default.volume;
  ctrl->source = factory_default.source;
}
/** @} */
