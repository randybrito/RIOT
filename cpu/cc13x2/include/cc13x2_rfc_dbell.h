/*
 * Copyright (C) 2020 Locha Inc
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup         cpu_cc13x2_definitions
 * @{
 *
 * @file
 * @brief           CC13x2 RF Core definitions
 *
 * @author          Jean Pierre Dudey <jeandudey@hotmail.com>
 */

#ifndef CC13X2_RFC_DBELL_H
#define CC13X2_RFC_DBELL_H

#include "cc26xx_cc13xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * RF Core Doorbell register
 */
typedef struct {
    reg32_t CMDR; /**< Doorbell Command Register */
    reg32_t CMDSTA; /**< Doorbell Command Status Register */
    reg32_t RFHWIFG; /**< Interrupt Flags From RF Hardware Modules */
    reg32_t RFHWIEN; /**< Interrupt Enable For RF Hardware Modules */
    reg32_t RFCPEIFG; /**< Interrupt Flags For Command and Packet Engine Generated Interrupts */
    reg32_t RFCPEIEN; /**< Interrupt Enable For Command and Packet Engine Generated Interrupts */
    reg32_t RFCPEISL; /**< Interrupt Vector Selection For Command and Packet Engine Generated Interrupts  */
    reg32_t RFACKIFG; /**< Doorbell Command Acknowledgement Interrupt Flag */
    reg32_t SYSGPOCTL; /**< RF Core General Purpose Output Control */
} rfc_dbell_regs_t;

/** @ingroup cpu_specific_peripheral_memory_map
  * @{
  */
#define RFC_DBELL_BASE      (PERIPH_BASE + 0x41000) /**< RFC_DBELL base address */
/** @} */

#define RFC_DBELL           ((rfc_dbell_regs_t *) (RFC_DBELL_BASE)) /**< RFC_DBELL register bank */

#ifdef __cplusplus
}
#endif

#endif /* CC13X2_RFC_DBELL_H */

/*@}*/
