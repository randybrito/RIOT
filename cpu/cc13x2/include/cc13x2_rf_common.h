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

/**
 * @brief   Turns on the radio core.
 *
 *          Sets up the power and resources for the radio core.
 *          - switches the high frequency clock to the xosc crystal on
 *            cc26x2/cc13x2.
 *          - initializes the rx buffers and command
 *          - powers on the radio core power domain
 *          - enables the radio core power domain
 *          - sets up the interrupts
 *          - sends the ping command to the radio core to make sure it is
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
 *          - disables the interrupts
 *          - disables the radio core power domain
 *          - powers off the radio core power domain
 *          - on cc13x2/cc26x2 switches the high frequency clock to the rcosc
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
