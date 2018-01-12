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
 * @file    max30101.h
 * @brief   MAX30101 Pulse Oximeter controlled through I2C
 *
 * @addtogroup max30101
 * @{
 */

#ifndef _MAX30101_H_
#define _MAX30101_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define SENSOR_ADDRESS 0x57

#define REG_INT_STAT1 0x00
#define REG_INT_STAT2 0x01
#define REG_INT_ENA1  0x02
#define REG_INT_ENA2  0x03
#define REG_FIFO_WRP  0x04
#define REG_OVERFLOW_COUNT 0x05
#define REG_FIFO_RDP 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_DIE_TEMP_INT 0x1F
#define REG_DIE_TEMP_FRAC 0x20
#define REG_DIE_TEMP_CONFIG 0x21
#define REG_REVISION_ID 0xFE
#define REG_PART_ID 0xFF

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct oxi {
  I2CDriver *i2cp;                  /** Driver attachement*/
  uint8_t revisionID;               /** chip revision id */
  uint8_t partID;                   /** chip part id */
} OXI_MAX30101_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

void MAX30101_Init(I2CDriver *i2cp);
void MAX30101_Configure(void);
void MAX30101_GetID(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAX30101_H_ */

/** @} */
