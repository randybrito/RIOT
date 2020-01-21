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

cc13x2_rf_cmd_nop_t cmd_nop = {
    .command_no = CMD_NOP,
    .status = 0,
    .next_op = NULL,
    .start_time = 0,
    .start_trigger = {0},
    .condition = {0},
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
