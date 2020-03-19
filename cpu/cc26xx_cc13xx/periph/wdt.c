/*
 * Copyright (C) 2020 Locha Inc
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_cc26xx_cc13xx
 * @ingroup     drivers_periph_wdt
 *
 * @brief
 *
 * @{
 * @file        wdt.c
 * @brief       Independent Watchdog Timer for CC26xx/CC13xx platforms
 *
 * @author      Jean Pierre Dudey <jeandudey@hotmail.com>
 * @}
 */

#include <stdlib.h>
#include <assert.h>

#include "cpu.h"
#include "timex.h"

#include "periph_cpu.h"
#include "periph_conf.h"
#include "periph/wdt.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

#define MS_RATIO (1000)

/**
 * @brief   WDT IRQ context
 */
typedef struct {
    uint32_t max_ticks; /**< Load value */
} wdt_ctx_t;

static wdt_ctx_t _wdt_ctx;

/**
 * @brief   Convert milliseconds to clock ticks.
 *
 * @param[in] ms The milliseconds.
 *
 * @return Clock ticks
 */
static uint32_t _wdt_ms_to_ticks(uint32_t ms)
{
    DEBUG("_wdt_ms_to_ticks(%lu)\n", ms);

    uint32_t convert_ratio = (CLOCK_CORECLOCK / WDT_DIV_RATIO) / MS_RATIO;
    uint32_t max_convert_ms = WDT_MAX_RELOAD / convert_ratio;

    uint32_t tick_value;
    /* Convert ms to watchdog timer ticks */
    /* Check if value exceeds maximum */
    if (ms > max_convert_ms) {
        DEBUG("_wdt_ms_to_ticks: overflow!\n");
        /* return zero to indicate overflow */
        tick_value = 0;
    }
    else {
        tick_value = ms * convert_ratio;
    }

    return tick_value;
}

/**
 * @brief   Unlock WDT registers
 */
static void _wdt_unlock(void)
{
    DEBUG("_wdt_unlock()\n");
    WDT->LOCK = WDT_LOCK_UNLOCK;
    while (WDT->LOCK == WDT_LOCK_LOCKED) {}
}

/**
 * @brief   Lock WDT again registers
 */
static void _wdt_lock(void)
{
    DEBUG("_wdt_lock()\n");
    WDT->LOCK = WDT_LOCK_LOCKED;
}

void wdt_init(void)
{
    DEBUG("wdt_init()\n");
    _wdt_ctx.max_ticks = 0;
}

void wdt_start(void)
{
    DEBUG("wdt_start()\n");
    unsigned key = irq_disable();
    _wdt_unlock();
    /* Enable interrupts on the WDT, when INTEN bit is set it can't be cleared,
     * only with a hardware reset */
    WDT->CTL |= (WDT_CTL_INTEN | WDT_CTL_RESEN);
    _wdt_lock();
    irq_restore(key);
}

void wdt_kick(void)
{
    DEBUG("wdt_kick()\n");
    unsigned key = irq_disable();
    _wdt_unlock();
    WDT->LOAD = _wdt_ctx.max_ticks;
    _wdt_lock();
    irq_restore(key);
}

void wdt_setup_reboot(uint32_t min_time, uint32_t max_time)
{
    DEBUG("wdt_setup_reboot(%lu, %lu)\n", min_time, max_time);

    /* Windowed mode not supported */
    assert(min_time == 0);

    /* Convert max time to ms */
    uint32_t ticks = _wdt_ms_to_ticks(max_time / 2);
    _wdt_ctx.max_ticks = ticks;

    unsigned key = irq_disable();

    _wdt_unlock();

    /* Set the timer interval */
    WDT->LOAD = ticks;

    /* Disable NMI interrupt */
    WDT->CTL &= ~WDT_CTL_INTTYPE;

    /* Stall WDT if a debugger stops the execution */
    WDT->TEST |= WDT_TEST_STALL;

    _wdt_lock();

    irq_restore(key);
}

void isr_watchdog(void)
{
    DEBUG("wdt called\n");
}
