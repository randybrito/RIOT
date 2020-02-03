/*
 * Copyright (C) 2020 Locha Inc
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup         cpu_cc13x2
 * @{
 *
 * @file
 * @brief           CC26x2/CC13x2 RF Core common functions
 *
 * @author          Jean Pierre Dudey <jeandudey@hotmail.com>
 * @}
 */

#ifndef CC13X2_RF_COMMON_H
#define CC13X2_RF_COMMON_H

#include <stdint.h>
#include <assert.h>

#include "net/ieee802154.h"

/**
 * @brief   Channel ranges
 * @{
 */
#define CC13X2_CHANNEL_MIN_SUB_GHZ  (0U)   /**< Minimum channel for Sub-GHz band */
#define CC13X2_CHANNEL_MAX_SUB_GHZ  (198U) /**< Maximum channel for Sub-GHz band */
/** @} */

#define CC13X2_FREQ_SPACING_SUB_GHZ (200U)    /**< Frequency spacing between channels */
#define CC13X2_BASE_FREQ_SUB_GHZ    (902200U) /**< Base frequency for the Sub-GHz band */

#define CC13X2_CENTER_FREQ_SUB_GHZ  (0x0393U) /**< Center frequency for the Sub-GHz band */
#define CC13X2_LO_DIVIDER_SUB_GHZ   (0x05U)   /**< LO divider for the Sub-GHz band */

/**
 * @brief   Calculate the channel frequency for a given channel number.
 *
 * @pre (@p chan >= CC13X2_CHANNEL_MIN_SUB_GHZ) && (@p chan <= CC13X2_CHANNEL_MAX_SUB_GHZ)
 *
 * @param[in] chan Channel number.
 *
 * @return Channel frequency.
 */
static inline uint32_t cc13x2_rf_channel_freq_sub_ghz(const uint16_t chan)
{
    assert(chan <= CC13X2_CHANNEL_MAX_SUB_GHZ);

    const uint32_t chan0 = CC13X2_BASE_FREQ_SUB_GHZ;
    const uint32_t spacing = CC13X2_FREQ_SPACING_SUB_GHZ;
    const uint32_t chan_min = CC13X2_CHANNEL_MIN_SUB_GHZ;
    return chan0 + spacing * ((uint32_t)chan - chan_min);
}

/**
 * @brief   Get decimal and fractionary parts of a frequency
 *
 * @pre (@p dec != NULL) && (@p frac != NULL)
 *
 * @param[in] freq     The frequency.
 * @param[out] dec     The decimal part.
 * @param[out] frac    The fractionary part.
 */
static inline void cc13x2_rf_freq_parts(const uint32_t freq, uint16_t *dec, uint16_t *frac)
{
    assert(dec != NULL);
    assert(frac != NULL);

    *dec = (uint16_t)(freq / 1000);
    *frac = (uint16_t)(((freq - ((*dec) * 1000)) * 0x10000) / 1000);
}

/**
 * @brief   Turns on the radio core.
 *
 *          Sets up the power and resources for the radio core.
 *          - Switches the high frequency clock to the xosc crystal on
 *            CC26X2/CC13X2.
 *          - Initializes the rx buffers and command
 *          - Powers on the radio core power domain
 *          - Enables the radio core power domain
 *          - Sets up the interrupts
 *          - Sends the ping command to the radio core to make sure it is
 *            running
 *
 * @return  The value from the command status register.
 * @retval  CMDSTA_Done The command was received.
 */
uint_fast8_t cc13x2_rf_power_on(void);

/**
 * @brief   Turns off the radio core.
 *
 *          Switches off the power and resources for the radio core.
 *          - Disables the interrupts
 *          - Disables the radio core power domain
 *          - Powers off the radio core power domain
 *          - On CC13X2/CC26X2 switches the high frequency clock to the rcosc
 *            to save power
 */
void cc13x2_rf_power_off(void);

/**
 * @brief   Sends the direct abort command to the radio core.
 *
 * @return  The value from the command status register.
 * @retval  CMDSTA_Done The command completed correctly.
 */
uint_fast8_t cc13x2_rf_execute_abort_cmd(void);

/**
 * @brief   Sends the direct ping command to the radio core.
 *
 *          Check that the Radio core is alive and able to respond to commands.
 *
 * @return  The value from the command status register.
 * @retval  CMDSTA_Done The command completed correctly.
 */
uint_fast8_t cc13x2_rf_prop_execute_ping_cmd(void);

/**
 * @brief   Enables the CPE0 and CPE1 radio interrupts.
 *
 *          Enables the @ref IRQ_LAST_COMMAND_DONE and
 *          @ref IRQ_LAST_FG_COMMAND_DONE to be handled by the
 *          @ref isr_rfc_cpe0 interrupt handler.
 */
void cc13x2_rf_setup_int(void);

/**
 *@brief    Disables and clears the CPE0 and CPE1 radio interrupts.
 */
void cc13x2_rf_stop_int(void);

#endif /* CC13X2_RF_COMMON_H */
/** @} */
