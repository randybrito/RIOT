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

#include "cc13x2_rf.h"
#include "cc13x2_rf_netdev.h"
#include "cpu.h"

#define RF_BOOT0          (0xE0000011) /**< RF Core boot parameters */
#define RF_IRQ_PRIO  (~0) /**< RF IRQs priority */

void cc13x2_rf_init(void)
{
    /* Enable RTC_UPD */
    AON_RTC->CTL |= CTL_RTC_UPD_EN;

    /* Set RF Core boot parameters */
    PRCM->RFCBITS = RF_BOOT0;

    NVIC_SetPriority(RF_CPE0_IRQN, RF_IRQ_PRIO);
    NVIC_EnableIRQ(RF_CPE0_IRQN);

    NVIC_SetPriority(RF_HW_IRQN, RF_IRQ_PRIO);
    NVIC_EnableIRQ(RF_HW_IRQN);
}

void cc13x2_rf_setup(cc13x2_rf_t *dev)
{
    netdev_t *netdev = (netdev_t *)dev;

    netdev->driver = &cc13x2_rf_driver;

    cc13x2_rf_init();
}
