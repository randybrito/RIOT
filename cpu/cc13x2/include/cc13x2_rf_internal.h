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
 */

#ifndef CC13X2_RF_INTERNAL_H
#define CC13X2_RF_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t ratmr_t; /**< RAT timer */

/**
 * @name            Definitions for trigger types
 * @{
 */
#define TRIG_NOW (0) /**< Triggers immediately */
#define TRIG_NEVER (1) /**< Never trigs */
#define TRIG_ABSTIME (2) /**< Trigs at an absolute time */
#define TRIG_REL_SUBMIT (3) /**< Trigs at a time relative to the command was submitted */
#define TRIG_REL_START (4) /**< Trigs at a time relative to the command started */
#define TRIG_REL_PREVSTART (5) /**< Trigs at a time relative to the previous command in the chain
                                    started */
#define TRIG_REL_FIRSTSTART (6) /**< Trigs at a time relative to the first command in the chain
                                     started */
#define TRIG_REL_PREVEND (7) /**< Trigs at a time relative to the previous command in the chain
                                  ended */
#define TRIG_REL_EVT1 (8) /**< Trigs at a time relative to the context defined "Event 1" */
#define TRIG_REL_EVT2 (9) /**< Trigs at a time relative to the context defined "Event 2" */
#define TRIG_EXTERNAL (10) /**< Trigs at an external event to the radio timer */
#define TRIG_PAST_BM (0x80) /**< Bitmask for setting pastTrig bit in order to trig immediately if
                                 trigger happened in the past */
/** @} */

typedef struct cc13x2_rf_op_s cc13x2_rf_op_t; /**< Radio operation command format */

/**
 * @brief           Radio operation command format
 */
struct __attribute__ ((__packed__,aligned (4))) cc13x2_rf_op_s {
    uint16_t command_no; /**< the command ID number 0x3801 */
    uint16_t status; /**< status of the command */
    cc13x2_rf_op_t *next_op; /**< next operation to run after this operation */
    ratmr_t start_time; /**< absolute or relative start time */
    struct {
        uint8_t trigger_type:4; /**< the type of trigger */
        uint8_t ena_cmd:1; /**< 0: no alternative trigger command, 1:
                                CMD_TRIGGER can be used as an alternative
                                trigger */
        uint8_t trigger_no:2; /**< the trigger number of the CMD_TRIGGER
                                   command that triggers this action */
        uint8_t past_trig:1; /**< 0: a trigger in the past is never triggered,
                                  or for start of commands, give an error.
                                  1: a trigger in the past is triggered as
                                  soon as possible */
    } start_trigger; /**< identification of the trigger that starts the
                          operation */
    struct {
        uint8_t rule:4; /**< condition for running next command */
        uint8_t skip:4; /**< number of skips + 1 if the rule involves skipping. */
    } condition; /**< condition for next command */
};

/**
 * @name CMD_SET_TX_POWER
 * @{
 */

#define CMD_SET_TX_POWER (0x0010) /**< Set TX power */

/**
 * @brief           Set TX Power
 */
typedef struct __attribute__((aligned(4))) {
    uint16_t command_no; /**< the command ID number */
    uint16_t tx_power; /**< new TX power setting */
} cc13x2_rf_cmd_set_tx_power_t;
/** @} */

/**
 * @name CMD_PROP_TX
 *
 * @{
 */
#define CMD_PROP_TX (0x3801) /**< CMD_PROP_TX command ID */

/**
 * @brief           Proprietary Mode Transmit Command
 */
typedef struct __attribute__((aligned(4))) {
    uint16_t command_no; /**< the command ID number 0x3801 */
    uint16_t status; /**< status of the command */
    cc13x2_rf_op_t *next_op; /**< next operation to run after this operation */
    ratmr_t start_time; /**< absolute or relative start time */
    struct {
        uint8_t trigger_type:4; /**< the type of trigger */
        uint8_t ena_cmd:1; /**< 0: no alternative trigger command, 1:
                                CMD_TRIGGER can be used as an alternative
                                trigger */
        uint8_t trigger_no:2; /**< the trigger number of the CMD_TRIGGER
                                   command that triggers this action */
        uint8_t past_trig:1; /**< 0: a trigger in the past is never triggered,
                                  or for start of commands, give an error.
                                  1: a trigger in the past is triggered as
                                  soon as possible */
    } start_trigger; /**< identification of the trigger that starts the
                          operation */
    struct {
        uint8_t rule:4; /**< condition for running next command */
        uint8_t skip:4; /**< number of skips + 1 if the rule involves skipping. */
    } condition; /**< condition for next command */
    struct {
        uint8_t fs_off:1; /** frequency synth on/off after command */
        uint8_t :2;
        uint8_t use_crc:1; /**< append CRC? */
        uint8_t var_len:1; /**< fixed length */
    } packet_config; /**< packet configuration */
    uint8_t packet_len; /**< packet length */
    uint32_t sync_word; /**< sync word to transmit */
    uint8_t* packet; /**< pointer to packet */
} cc13x2_rf_cmd_prop_tx_t;

extern cc13x2_rf_cmd_prop_tx_t cmd_prop_tx;
/** @} */

/**
 * @brief           Execute an immediate command in the RF Core.
 */
int cc13x2_run_immediate_command(void *command, uint32_t* rawstatus);

#ifdef __cplusplus
}
#endif

#endif /* CC13X2_RF_INTERNAL_H */

/*@}*/
