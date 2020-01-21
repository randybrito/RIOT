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
 * @brief           CC13x2 radio
 *
 * @author          Jean Pierre Dudey <jeandudey@hotmail.com>
 * @}
 */

#define ENABLE_DEBUG (1)
#include "debug.h"

#include "cpu.h"
#include "cc13x2_rf_internal.h"

enum {
    CORE_STATUS_IDLE = 0, /**< RF Core is off */
    CORE_STATUS_ACTIVE = 1, /**< RF Core is active */
};

typedef struct {
    int status; /**< status of the RF Core */
} rf_core_t;

static rf_core_t _rf_core = {
    .status = CORE_STATUS_IDLE,
};

cc13x2_rf_cmd_set_tx_power_t cmd_set_tx_power = {
    .command_no = CMD_SET_TX_POWER,
    .tx_power = 0,
};

cc13x2_rf_cmd_prop_tx_t cmd_prop_tx = {
    .command_no = CMD_PROP_TX,
    .status = 0,
    .next_op = NULL,
    .start_time = 0,
    .start_trigger = {0},
    .condition = {0},
    .packet_config = {0},
    .packet_len = 0,
    .sync_word = 0,
    .packet = NULL,
};

int cc13x2_run_immediate_command(void *command, uint32_t* rawstatus)
{
    assert(command != NULL);

    if (_rf_core.status != CORE_STATUS_ACTIVE) {
        return -1;
    }

    /* wait until dbell becomes available and reset RFACKIFG */
    while (RFC_DBELL->CMDR != 0) {}
    RFC_DBELL->RFACKIFG = 0;

    /* send the command to the dbell */
    RFC_DBELL->CMDR = (uint32_t)command;

    /* wait until the RF Core parses the commnand and reset RFACKIFG */
    while (!RFC_DBELL->RFACKIFG) {}
    RFC_DBELL->RFACKIFG = 0;

    if (rawstatus != NULL) {
        /* return the command status */
        *rawstatus = (uint32_t)(RFC_DBELL->CMDSTA);
    }

    return 0;
}

void isr_rfc_cpe1(void)
{
    DEBUG_PUTS("ISR_RFC_CPE1");
    cortexm_isr_end();
}

void isr_rfc_cpe0(void)
{
    DEBUG_PUTS("ISR_RFC_CPE0");
    cortexm_isr_end();
}

void isr_rfc_hw(void)
{
    DEBUG_PUTS("ISR_RFC_HW");
    cortexm_isr_end();
}

void isr_rfc_cmd_ack(void)
{
    DEBUG_PUTS("ISR_RFC_CMD_ACK");
    cortexm_isr_end();
}
