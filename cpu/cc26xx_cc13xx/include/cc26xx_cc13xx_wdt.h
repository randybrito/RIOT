/*
 * Copyright (C) 2016 Leon George
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup         cpu_cc26xx_cc13xx_definitions
 * @{
 *
 * @file
 * @brief           CC26xx/CC13xx WDT register definitions
 */

#ifndef CC26XX_CC13XX_WDT_H
#define CC26XX_CC13XX_WDT_H

#include <cc26xx_cc13xx.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * WDT registers
 */
typedef struct {
    reg32_t LOAD; /**< config */
    reg32_t VALUE; /**< current count value */
    reg32_t CTL; /**< control */
    reg32_t ICR; /**< interrupt clear */
    reg32_t RIS; /**< raw interrupt status */
    reg32_t MIS; /**< masked interrupt status */
    reg32_t __reserved1[0x100]; /**< reserved */
    reg32_t TEST; /**< test mode */
    reg32_t INT_CAUS; /**< interrupt cause test mode */
    reg32_t __reserved2[0x1f9]; /**< reserved */
    reg32_t LOCK; /**< lock */
} wdt_regs_t;

/** @ingroup cpu_specific_peripheral_memory_map
  * @{
  */
#define WDT_BASE      (PERIPH_BASE + 0x80000) /**< WDT base address */
/*@}*/

#define WDT           ((wdt_regs_t *) (WDT_BASE)) /**< WDT register bank */

/**
 * @brief   WDT register values
 * @{
 */
/**
 * @brief   Interrupt enable (start WDT)
 * @details 0h = Disable
 *          1h = Enable
 */
#define WDT_CTL_INTEN     0x1

/**
 * @brief   Reset enable
 * @details 0h = Disable
 *          1h = Enable
 */
#define WDT_CTL_RESEN     0x2

/**
 * @brief   Interrupt type
 * @details 0h = Maskable interrupt
 *          1h = Non-maskable interrupt
 */
#define WDT_CTL_INTTYPE   0x4

/**
 * @brief   Stall enable
 * @details 0h = Disable
 *          1h = Enable
 */
#define WDT_TEST_STALL    0x100

#define WDT_LOCK_UNLOCKED 0x0 /**< Unlocked */
#define WDT_LOCK_LOCKED   0x1 /**< Locked */
#define WDT_LOCK_UNLOCK   0x1ACCE551  /**< Unlocks the Watchdog Timer */
/** @} */

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif /* CC26XX_CC13XX_WDT_H */

/*@}*/
