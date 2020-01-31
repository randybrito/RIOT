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

#include "cc13x2_rf_common.h"

#include <assert.h>

#include <driverlib/interrupt.h>
#include <driverlib/prcm.h>
#include <driverlib/rfc.h>
#include <driverlib/rf_common_cmd.h>

uint_fast8_t cc13x2_rf_execute_abort_cmd(void)
{
    return (RFCDoorbellSendTo(CMDR_DIR_CMD(CMD_ABORT)) & 0xFF);
}

uint_fast8_t cc13x2_rf_prop_execute_ping_cmd(void)
{
    return (RFCDoorbellSendTo(CMDR_DIR_CMD(CMD_PING)) & 0xFF);
}

void cc13x2_rf_setup_int(void)
{
    bool interrupts_disabled;

    assert(PRCMRfReady());

    interrupts_disabled = IntMasterDisable();

    /* Set all interrupt channels to CPE0 channel, error to CPE1 */
    HWREG(RFC_DBELL_NONBUF_BASE + RFC_DBELL_O_RFCPEISL) = IRQ_INTERNAL_ERROR;
    HWREG(RFC_DBELL_NONBUF_BASE + RFC_DBELL_O_RFCPEIEN) = IRQ_LAST_COMMAND_DONE | IRQ_LAST_FG_COMMAND_DONE;

    IntPendClear(INT_RFC_CPE_0);
    IntPendClear(INT_RFC_CPE_1);
    IntEnable(INT_RFC_CPE_0);
    IntEnable(INT_RFC_CPE_1);

    if (!interrupts_disabled)
    {
        IntMasterEnable();
    }
}

void cc13x2_rf_stop_int(void)
{
    bool interrupts_disabled;

    interrupts_disabled = IntMasterDisable();

    /* clear and disable interrupts */
    HWREG(RFC_DBELL_NONBUF_BASE + RFC_DBELL_O_RFCPEIFG) = 0x0;
    HWREG(RFC_DBELL_NONBUF_BASE + RFC_DBELL_O_RFCPEIEN) = 0x0;

    IntUnregister(INT_RFC_CPE_0);
    IntUnregister(INT_RFC_CPE_1);
    IntPendClear(INT_RFC_CPE_0);
    IntPendClear(INT_RFC_CPE_1);
    IntDisable(INT_RFC_CPE_0);
    IntDisable(INT_RFC_CPE_1);

    if (!interrupts_disabled)
    {
        IntMasterEnable();
    }
}
