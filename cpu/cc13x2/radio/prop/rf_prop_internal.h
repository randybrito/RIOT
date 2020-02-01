/*
 *  Copyright (c) 2018, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

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
 * @brief           CC13x2 IEEE 802.15.4 netdev driver
 *
 * @author          Jean Pierre Dudey <jeandudey@hotmail.com>
 */

#ifndef CC13X2_RF_PROP_INTERNAL_H
#define CC13X2_RF_PROP_INTERNAL_H

#include <stdbool.h>

#include "iolist.h"
#include "net/netdev.h"
#include "net/ieee802154.h"
#include "net/netdev/ieee802154.h"

/**
 * @brief   If the EUI64 read from the ccfg is all ones then the customer did
 *          not set the address
 */
#define CCFG_UNKNOWN_EUI64 0xFF

/**
 * @brief TX output power
 * @{
 */

/**
 * @brief   Creates a TX power entry for the default PA.
 *
 *          The values for @a bias, @a gain, @a boost and @a coefficient are
 *          usually measured by Texas Instruments for a specific front-end
 *          configuration. They can then be obtained from SmartRF Studio.
 */
#define DEFAULT_PA_ENTRY(bias, gain, boost, coefficient) \
        ((bias) << 0) | ((gain) << 6) | ((boost) << 8) | ((coefficient) << 9)

/**
 * TX Power dBm lookup table
 */
typedef struct output_config
{
    int      dbm;
    uint16_t value;
} output_config_t;


#if defined(CPU_MODEL_CC1312R1F3)
/**
 * @brief     TX Power dBm lookup table.
 *
 *            This table is auto generated from Smart RF Studio.
 */
static const output_config_t rgOutputPower[] = {
  // The original PA value (12.5 dBm) has been rounded to an integer value.
  {13, DEFAULT_PA_ENTRY(36, 0, 0, 89) },
  {12, DEFAULT_PA_ENTRY(16, 0, 0, 82) },
  {11, DEFAULT_PA_ENTRY(26, 2, 0, 51) },
  {10, DEFAULT_PA_ENTRY(18, 2, 0, 31) },
  {9, DEFAULT_PA_ENTRY(28, 3, 0, 31) },
  {8, DEFAULT_PA_ENTRY(24, 3, 0, 22) },
  {7, DEFAULT_PA_ENTRY(20, 3, 0, 19) },
  {6, DEFAULT_PA_ENTRY(17, 3, 0, 16) },
  {5, DEFAULT_PA_ENTRY(14, 3, 0, 14) },
  {4, DEFAULT_PA_ENTRY(13, 3, 0, 11) },
  {3, DEFAULT_PA_ENTRY(11, 3, 0, 10) },
  {2, DEFAULT_PA_ENTRY(10, 3, 0, 9) },
  {1, DEFAULT_PA_ENTRY(9, 3, 0, 9) },
  {0, DEFAULT_PA_ENTRY(8, 3, 0, 8) },
  {-5, DEFAULT_PA_ENTRY(4, 3, 0, 5) },
  {-10, DEFAULT_PA_ENTRY(2, 3, 0, 5) },
  {-15, DEFAULT_PA_ENTRY(1, 3, 0, 3) },
  {-20, DEFAULT_PA_ENTRY(0, 3, 0, 2) },
};
#else
#error "Unknown CPU variant"
#endif

#define OUTPUT_CONFIG_COUNT (sizeof(rgOutputPower) / sizeof(rgOutputPower[0]))

/**
 * @brief   Minimum output power.
 */
#define OUTPUT_POWER_MIN (rgOutputPower[OUTPUT_CONFIG_COUNT - 1].dbm)

/**
 * @brief   Maximum output power.
 */
#define OUTPUT_POWER_MAX (rgOutputPower[0].dbm)
/**
 * @brief   Unknwon (default) output power.
 */
#define OUTPUT_POWER_UNKNOWN 0xFFFF
/** @} */

/**
 * @brief   RF buffer fields size
 * @{
 */
#define RX_BUF_LEN_FIELD_LEN (sizeof(uint16_t)) /**< Length in bytes of the length field */

#define RX_BUF_CRC_LEN       (2U) /**< Length in bytes of the CRC field. */
#define RX_BUF_RSSI_LEN      (1U) /**< Length in bytes of the RSSI field. */
#define RX_BUF_CORR_LEN      (1U) /**< Length in bytes of the CORR field. */
#define RX_BUF_TIMESTAMP_LEN (4)  /**< Length in bytes of the Timestamp field. */

/**
 * @brief    Metadata length.
 */
#define RX_BUF_METADATA_LEN \
  (RX_BUF_CRC_LEN + RX_BUF_RSSI_LEN + RX_BUF_CORR_LEN + RX_BUF_TIMESTAMP_LEN)
/** @} */

/**
 * size of length field in receive struct
 *
 * defined in Table 23-10 of the cc26xx TRM
 */
#define DATA_ENTRY_LENSZ_BYTE 1

#endif /* CC13X2_RF_PROP_INTERNAL_H */
/** @} */
