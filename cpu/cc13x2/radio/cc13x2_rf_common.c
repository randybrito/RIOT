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
#include <driverlib/osc.h>
#include <driverlib/prcm.h>
#include <driverlib/rfc.h>
#include <driverlib/rf_common_cmd.h>

uint_fast8_t cc13x2_rf_power_on(void)
{
    bool interruptsWereDisabled;
    bool oscSourceSwitch = false;

#if defined(CPU_VARIANT_X2)
    /* Request the HF XOSC as the source for the HF clock. Needed before we can
     * use the FS. This will only request, it will _not_ perform the switch.
     */
    if (OSCClockSourceGet(OSC_SRC_CLK_HF) != OSC_XOSC_HF)
    {
        /* Request to switch to the crystal to enable radio operation. It takes a
         * while for the XTAL to be ready so instead of performing the actual
         * switch, we do other stuff while the XOSC is getting ready.
         */
        OSCClockSourceSet(OSC_SRC_CLK_HF, OSC_XOSC_HF);
        oscSourceSwitch = true;
    }

    /*
     * Trigger a switch to the XOSC, so that we can subsequently use the RF FS
     * This will block until the XOSC is actually ready, but give how we
     * requested it early on, this won't be too long a wait.
     * This should be done before starting the RAT.
     */
    if (oscSourceSwitch)
    {
        /* Block until the high frequency clock source is ready */
        while (!OSCHfSourceReady())
            ;

        /* Switch the HF clock source (cc26xxware executes this from ROM) */
        OSCHfSourceSwitch();
    }
#endif

    interruptsWereDisabled = IntMasterDisable();

    /* Enable RF Core power domain */
    PRCMPowerDomainOn(PRCM_DOMAIN_RFCORE);

    while (PRCMPowerDomainStatus(PRCM_DOMAIN_RFCORE) != PRCM_DOMAIN_POWER_ON)
        ;

    PRCMDomainEnable(PRCM_DOMAIN_RFCORE);
    PRCMLoadSet();

    while (!PRCMLoadGet())
        ;

    cc13x2_rf_setup_int();

    if (!interruptsWereDisabled)
    {
        IntMasterEnable();
    }

    /* Let CPE boot */
    RFCClockEnable();

    /* Send ping (to verify RFCore is ready and alive) */
    return cc13x2_rf_prop_execute_ping_cmd();
}

/**
 * Turns off the radio core.
 *
 * Switches off the power and resources for the radio core.
 * - disables the interrupts
 * - disables the radio core power domain
 * - powers off the radio core power domain
 * - switches the high frequency clock to the rcosc to save power
 */
void cc13x2_rf_power_off(void)
{
    cc13x2_rf_stop_int();

    PRCMDomainDisable(PRCM_DOMAIN_RFCORE);
    PRCMLoadSet();

    while (!PRCMLoadGet())
        ;

    PRCMPowerDomainOff(PRCM_DOMAIN_RFCORE);

    while (PRCMPowerDomainStatus(PRCM_DOMAIN_RFCORE) != PRCM_DOMAIN_POWER_OFF)
        ;

#if defined(CPU_VARIANT_X2)
    if (OSCClockSourceGet(OSC_SRC_CLK_HF) != OSC_RCOSC_HF)
    {
        /* Request to switch to the RC osc for low power mode. */
        OSCClockSourceSet(OSC_SRC_CLK_HF, OSC_RCOSC_HF);
        /* Switch the HF clock source (cc26xxware executes this from ROM) */
        OSCHfSourceSwitch();
    }
#endif
}

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
