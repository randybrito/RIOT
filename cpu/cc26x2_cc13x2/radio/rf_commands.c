/*
 * Copyright (C) 2020 Locha Inc
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup         cpu_cc26x2_cc13x2
 * @{
 *
 * @file
 * @brief           CC13x2 IEEE 802.15.4 netdev driver
 *
 * @author          Jean Pierre Dudey <jeandudey@hotmail.com>
 * @}
 */

#include "cc26x2_cc13x2_rf.h"
#include "cc26xx_cc13xx_rfc_mailbox.h"

/* Overrides for CMD_PROP_RADIO_DIV_SETUP */
static uint32_t rf_prop_overrides[] =
{
    /* override_prop_common.xml
     * DC/DC regulator: In Tx, use DCDCCTL5[3:0]=0x7 (DITHER_EN=0 and IPEAK=7).
     */
    (uint32_t)0x00F788D3,
    /* override_tc146.xml
     * Tx: Configure PA ramp time, PACTL2.RC=0x3 (in ADI0, set PACTL2[4:3]=0x3)
     */
    RFC_ADI_2HALFREG_OVERRIDE(0, 16, 0x8, 0x8, 17, 0x1, 0x1),
    /* Tx: Configure PA ramping, set wait time before turning off (0x1A ticks
     of 16/24 us = 17.3 us).
     */
    RFC_HW_REG_OVERRIDE(0x6028, 0x001A),
    /* Rx: Set AGC reference level to 0x16 (default: 0x2E) */
    RFC_HW_REG_OVERRIDE(0x609C, 0x0016),
    /* Rx: Set RSSI offset to adjust reported RSSI by -1 dB (default: -2),
     * trimmed for external bias and differential configuration */
    (uint32_t)0x000188A3,
    /* Rx: Set anti-aliasing filter bandwidth to 0x8 (in ADI0, set
     * IFAMPCTL3[7:4]=0x8) */
    RFC_ADI_HALFREG_OVERRIDE(0, 61, 0xF, 0x8),
    /* TX power override
     * Tx: Set PA trim to max to maximize its output power (in ADI0,
     * set PACTL0=0xF8) */
    RFC_ADI_REG_OVERRIDE(0, 12, 0xF8),
    RFC_END_OVERRIDE
};

rfc_cmd_prop_radio_div_setup_t rf_cmd_prop_radio_div_setup =
{
    .op = {
        .command_no = RFC_CMD_PROP_RADIO_DIV_SETUP,
        .status = RFC_IDLE,
        .next_op = NULL, /* set by us */
        .start_time = 0,
        .start_trigger = {
            .type = RFC_TRIG_NOW,
            .ena_cmd = 0,
            .trigger_no = 0,
            .past_trig = 0
        },
        .condition = {
            .rule = RFC_COND_ALWAYS,
            .skip_no = 0
        }
    },
    .modulation = {
        .mod_type = 0x1,
        .deviation = 0xC8,
        .deviation_step_sz = 0
    },
    .symbol_rate = {
        .prescale = 0xF,
        .rate_word = 0x20000,
        .decim_mode = 0x0
    },
    .rx_bw = 0x59,
    .pream_conf = {
        .pream_bytes = 7,
        .pream_mode = 0,
    },
    .format_conf = {
        .sw_bits = 18,
        .bit_reversal = 0,
        .msb_first = 1,
        .fec_mode = 0,
        .whiten_mode = 7
    },
    .config = {
        .front_end_mode = 0,
        .bias_mode = 1,
        .analog_cfg_mode = 0,
        .no_fs_powerup = 0
    },
    .tx_power = 0x04C0,
    .reg_override = rf_prop_overrides,
    .center_freq = 0x0393,
    .int_freq = 0x0999,
    .lo_divider = 0x05
};

rfc_cmd_fs_t rf_cmd_fs =
{
    .op = {
        .command_no = RFC_CMD_FS,
        .status = RFC_IDLE,
        .next_op = NULL, /* set by us */
        .start_time = 0,
        .start_trigger = {
            .type = RFC_TRIG_NOW,
            .ena_cmd = 0,
            .trigger_no = 0,
            .past_trig = 0
        },
        .condition = {
            .rule = RFC_COND_ALWAYS,
            .skip_no = 0
        }
    },
    .frequency = 0, /* set by us */
    .fract_freq = 0, /* set by us */
    .synth_conf = {
        .txmode = 0,
        .ref_freq = 0
    },
    .__dummy0 = 0,
    .__dummy1 = 0,
    .__dummy2 = 0,
    .__dummy3 = 0
};

rfc_cmd_prop_tx_adv_t rf_cmd_prop_tx_adv =
{
    .op = {
        .command_no = RFC_CMD_PROP_TX_ADV,
        .status = RFC_IDLE,
        .next_op = NULL, /* set by us */
        .start_time = 0,
        .start_trigger = {
            .type = RFC_TRIG_NOW,
            .ena_cmd = 0,
            .trigger_no = 0,
            .past_trig = 0
        },
        .condition = {
            .rule = RFC_COND_ALWAYS,
            .skip_no = 0
        },
    },
    .pkt_conf = {
        .fs_off = 0,
        .use_crc = 1,
        .crc_inc_sw = 0,
        .crc_inc_hdr = 0
    },
    .num_hdr_bits = IEEE802154_PHY_MR_FSK_PHR_LEN * 8,
    .pkt_len = 0, /*< set by us */
    .start_conf = {
        .ext_tx_trig = 0,
        .input_mode = 0,
        .source = 0
    },
    .pre_trigger = {
        .type = RFC_TRIG_NOW,
        .ena_cmd = 0,
        .trigger_no = 0,
        .past_trig = 0
    },
    .pre_time = 0,
    .sync_word = 0x0055904E,
    .pkt = 0 /* set by us */
};

rfc_cmd_prop_rx_adv_t rf_cmd_prop_rx_adv =
{
    .op = {
        .command_no = RFC_CMD_PROP_RX_ADV,
        .status = RFC_IDLE,
        .next_op = NULL, /* set by us */
        .start_time = 0,
        .start_trigger = {
            .type = RFC_TRIG_NOW,
            .ena_cmd = 0,
            .trigger_no = 0,
            .past_trig = 0
        },
        .condition = {
            .rule = RFC_COND_ALWAYS,
            .skip_no = 0
        }
    },
    .pkt_conf = {
        .fs_off = 0,
        .repeat_ok = 1,
        .repeat_nok = 1,
        .use_crc = 1,
        .crc_inc_sw = 0,
        .crc_inc_hdr = 0,
        .end_type = 0,
        .filter_op = 1
    },
    .rx_conf = {
        .auto_flush_ignored = 1,
        .auto_flush_crc_err = 1,
        .include_hdr = 0,
        .include_crc = 0,
        .append_rssi = 1,
        .append_timestamp = 0,
        .append_status = 1
    },
    .sync_word0 = 0x0055904E,
    .sync_word1 = 0,
    .max_pkt_len = IEEE802154G_FRAME_LEN_MAX,
    .hdr_conf = {
        .num_hdr_bits = IEEE802154_PHY_MR_FSK_PHR_LEN * 8,
        .len_pos = 0,
        .num_len_bits = 11
    },
    .addr_conf = {
        .addr_type = 0,
        .addr_size = 0,
        .addr_pos = 0,
        .num_addr = 0
    },
    .len_offset = -4,
    .end_trigger = {
        .type = RFC_TRIG_NEVER,
        .ena_cmd = 0,
        .trigger_no = 0,
        .past_trig = 0
    },
    .end_time = 0,
    .addr = 0, /* unused */
    .queue = 0, /* set by us */
    .output = 0 /* set by us */
};