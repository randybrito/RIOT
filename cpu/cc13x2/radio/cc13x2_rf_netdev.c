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
#include "cc13x2_rf_netdev.h"

static int _send(netdev_t *dev, const iolist_t *iolist)
{
    (void)dev;
    (void)iolist;

    return -1;
}

static int _recv(netdev_t *dev, void *buf, size_t len, void *info)
{
    (void)dev;
    (void)buf;
    (void)len;
    (void)info;

    return -1;
}

static int _init(netdev_t *netdev)
{
    cc13x2_rf_t *dev = (cc13x2_rf_t *)netdev;

    uint16_t addr_short = cc13x2_rf_get_addr_short();
    uint64_t addr_long = cc13x2_rf_get_addr_long();

    /* Initialise netdev_ieee802154_t struct */
    netdev_ieee802154_reset(&dev->netdev);

    netdev_ieee802154_set(&dev->netdev, NETOPT_ADDRESS,
                          &addr_short, sizeof(addr_short));
    netdev_ieee802154_set(&dev->netdev, NETOPT_ADDRESS_LONG,
                          &addr_long, sizeof(addr_long));

    return 0;
}

static void _isr(netdev_t *dev)
{
    (void)dev;
}

static int _get(netdev_t *netdev, netopt_t opt, void *value, size_t max_len)
{
    cc13x2_rf_t *dev = (cc13x2_rf_t *)netdev;

    if (dev == NULL) {
        return -ENODEV;
    }

    switch (opt) {
        case NETOPT_ADDRESS:
            if (max_len < sizeof(uint16_t)) {
                return -EOVERFLOW;
            }
            else {
                *(uint16_t *)value = cc13x2_rf_get_addr_short();
            }
            return sizeof(uint16_t);
        
        case NETOPT_ADDRESS_LONG:
            if (max_len < sizeof(uint64_t)) {
                return -EOVERFLOW;
            }
            else {
                *(uint64_t *)value = cc13x2_rf_get_addr_long();
            }
            return sizeof(uint64_t);

        default:
            break;
    }

    int res = netdev_ieee802154_get((netdev_ieee802154_t *)netdev, opt, value,
                                    max_len);

    if ((res >= 0) || (res != -ENOTSUP)) {
        return res;
    }

    return -ENOTSUP;
}

static int _set(netdev_t *dev, netopt_t opt, const void *value, size_t value_len)
{
    (void)dev;
    (void)opt;
    (void)value;
    (void)value_len;

    return -1;
}

const netdev_driver_t cc13x2_rf_driver = {
    .send = _send,
    .recv = _recv,
    .init = _init,
    .isr = _isr,
    .get = _get,
    .set = _set,
};
