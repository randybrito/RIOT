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

#include "cc13x2_rf.h"
#include "cpu.h"

#define PRIMARY_ADDRESS_0 (FCFG->MAC_15_4_0) /**< IEEE 802.15.4 primary address part 0 */
#define PRIMARY_ADDRESS_1 (FCFG->MAC_15_4_1) /**< IEEE 802.15.4 primary address part 1 */

#define SECONDARY_ADDRESS_0 (CCFG->IEEE_MAC_0) /**< IEEE 802.15.4 secondary address part 0 */
#define SECONDARY_ADDRESS_1 (CCFG->IEEE_MAC_1) /**< IEEE 802.15.4 secondary address part 1 */

static bool is_set_secondary_addr(void)
{
    return (SECONDARY_ADDRESS_0 != 0xFFFFFFFF) &&
           (SECONDARY_ADDRESS_1 != 0xFFFFFFFF);
}

uint16_t cc13x2_rf_get_addr_short(void)
{
    uint16_t ret;
    uint8_t *addr = (uint8_t *)&PRIMARY_ADDRESS_0;

    /* Use secondary address if set */
    if (is_set_secondary_addr()) {
        addr = (uint8_t *)&SECONDARY_ADDRESS_0;
    }

    ret = ((uint16_t)addr[0] << 8) |
          ((uint16_t)addr[1] << 0);

    return ret;
}

uint64_t cc13x2_rf_get_addr_long(void)
{
    uint64_t ret;

    /* Use secondary address if set */
    if (is_set_secondary_addr()) {
        ret = ((uint64_t)SECONDARY_ADDRESS_0 << 32) |
              ((uint64_t)SECONDARY_ADDRESS_1 << 0);
    }
    else {
        ret = ((uint64_t)PRIMARY_ADDRESS_0 << 32) |
              ((uint64_t)PRIMARY_ADDRESS_1 << 0);
    }

    return ret;
}
