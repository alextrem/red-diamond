/*
    ChibiOS/RT - Copyright (C) 2016 Alexander Geissler

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
 * @file common.c
 * @brief Common things for configuration and setup end up in this file
 *
 * @addtogroup common
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "common.h"

/**
 * @brief   Sets up I2S interface according to samplerate and databits
 * @pre     I2S must be enabled
 *
 * @param[in]   samplerate
 * @param[in]   nbits
 */
void Config_I2S(const uint16_t samplerate, const uint8_t nbits) {

}

/** @}  */
