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

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Sets up I2S interface according to samplerate and databits
 * @pre     I2S must be enabled
 *
 * @param[in]   i2sp
 * @param[in]   samplerate
 * @param[in]   nbits
 *
 * @note    The default I2S pll values are defined in mcuconf.h
 *          The default samplerate with this setup is 48kHz. Take a closer look
 *          at page 826 of DM00031020.pdf
 */
void Config_I2S(I2SDriver* i2sp, const SAMPLERATE_t samplerate, const uint8_t nbits) {
  /* Clear I2SCFGR & I2SPR register */
  i2sp->spi->I2SCFGR &= I2SCFGR_CLEAR_MASK;
  i2sp->spi->I2SPR = I2SPR_CLEAR_MASK;

  /* Disable interface while clock for I2S is set up */
  rccEnableSPI3(false);

  /* Set PLLI2S as clock source */
  if ((RCC->CFGR & RCC_CFGR_I2SSRC) != 0)
  {
    RCC->CFGR &= ~RCC_CFGR_I2SSRC;
  }

  /* TODO: clean up. We don't need to get the PLL values */
  /* Get the PLLI2SN value */
  uint32_t plln = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6;
  /* Get the PLLI2SR value */
  uint32_t pllr = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> 27;

  /* Get the PLLM value */
  //uint32_t pllm = (RCC->PLLCFGR & RCC_PLLCFGR_PLLM);

  uint8_t i2sdiv, i2sodd = 0;

  /* Get PLL values according to samplerate */
  if (nbits == 0) {
    plln = pll_settings_16bit[samplerate].plli2sn;
    pllr = pll_settings_16bit[samplerate].plli2sr;
    i2sdiv = pll_settings_16bit[samplerate].i2sdiv;
    i2sodd = pll_settings_16bit[samplerate].i2sodd;
  }
  else {
    plln = pll_settings_32bit[samplerate].plli2sn;
    pllr = pll_settings_32bit[samplerate].plli2sr;
    i2sdiv = pll_settings_32bit[samplerate].i2sdiv;
    i2sodd = pll_settings_32bit[samplerate].i2sodd;
  }

  /* Set I2S PLL values */
  RCC->PLLI2SCFGR = ((plln << 6) & RCC_PLLI2SCFGR_PLLI2SN) | \
                    ((pllr << 27) & RCC_PLLI2SCFGR_PLLI2SR);

  /* Set prescaler register according samplerate and clock*/
  i2sp->spi->I2SPR = ((i2sodd << 8) | i2sdiv);

  /* Set the I2S configuration register 24bit, slave transmit, I2S phillips */
  i2sp->spi->I2SCFGR = (SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_DATLEN_0) \
                       & (~SPI_I2SCFGR_I2SCFG & ~SPI_I2SCFGR_I2SSTD) \
                       & (~SPI_I2SCFGR_CKPOL);

  /* Last but not least -> Activate I2S */
  i2sp->spi->I2SCFGR |= SPI_I2SCFGR_I2SE;
}

/** @}  */
