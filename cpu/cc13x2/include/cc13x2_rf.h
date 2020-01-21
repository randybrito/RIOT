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
 */

#ifndef CC13X2_RF_H
#define CC13X2_RF_H

#include "net/netdev.h"
#include "net/netdev/ieee802154.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief           CC13x2 radio device structure
 */
typedef struct {
    netdev_ieee802154_t netdev; /**< IEEE 802.15.4 network device */
} cc13x2_rf_t;

/**
 * @brief           CC13x2 radio netdev setup
 * @param           dev: the network device structure.
 */
void cc13x2_rf_setup(cc13x2_rf_t* dev);

/**
 * @brief           Get IEEE 802.15.4 short address
 *
 * @return          The short address
 */
uint16_t cc13x2_rf_get_addr_short(void);

/**
 * @brief           Get IEEE 802.15.4 long address
 *
 * @return          The long address
 */
uint64_t cc13x2_rf_get_addr_long(void);

#ifdef __cplusplus
}
#endif

#endif /* CC13X2_RF_H */

/*@}*/
