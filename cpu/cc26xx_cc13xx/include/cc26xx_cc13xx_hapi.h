/*
 * Copyright (C) 2020 Locha Inc
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
 * @brief           CC26x2, CC13x2 ROM Hardware API definitions
 * @author          Jean Pierre Dudey <jeandudey@hotmail.com>
 */

#ifndef CC26XX_CC13XX_HAPI_H
#define CC26XX_CC13XX_HAPI_H

#include <cc26xx_cc13xx.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   ROM Hard-API function interface types
 * @{
 */
typedef uint32_t (* hapi_crc32_t)(uint8_t *, uint32_t, uint32_t);
typedef uint32_t (* hapi_getflsize_t)(void);
typedef uint32_t (* hapi_getchipid_t)(void);
typedef uint32_t (* hapi_reserved1_t)(uint32_t);
typedef uint32_t (* hapi_reserved2_t)(void);
typedef uint32_t (* hapi_reserved3_t)(uint8_t *, uint32_t, uint32_t);
typedef void (* hapi_resetdev_t)(void);
typedef uint32_t (* hapi_fletcher32_t)(uint16_t *, uint16_t, uint16_t);
typedef uint32_t (* hapi_minval_t)(uint32_t *, uint32_t);
typedef uint32_t (* hapi_maxval_t)(uint32_t *, uint32_t);
typedef uint32_t (* hapi_meanval_t)(uint32_t *, uint32_t);
typedef uint32_t (* hapi_stddval_t)(uint32_t *, uint32_t);
typedef void (* hapi_hfsourcesafeswitch_t)(void);
typedef void (* hapi_reserved4_t)(uint32_t);
typedef void (* hapi_reserved5_t)(uint32_t);
typedef void (* hapi_compain_t)(uint8_t);
typedef void (* hapi_comparef_t)(uint8_t);
typedef void (* hapi_adccompbin_t)(uint8_t);
#ifdef CPU_VARIANT_X2
typedef void (* hapi_dacvref_t)(uint8_t);
#else
typedef void (* hapi_compbref_t)(uint8_t);
#endif
/** @} */

/**
 * @brief   On ROM hardware API
 */
typedef struct {
    hapi_crc32_t Crc32; /**< CRC32 */
    hapi_getflsize_t FlashGetSize; /**< Get flash size */
    hapi_getchipid_t GetChipId; /**< Get chip ID */
    hapi_reserved1_t ReservedLocation1; /**< Reserved */
    hapi_reserved2_t ReservedLocation2; /**< Reserved */
    hapi_reserved3_t ReservedLocation3; /**< Reserved */
    hapi_resetdev_t ResetDevice; /**< Reset device */
    hapi_fletcher32_t Fletcher32; /**< Fletcher 32 checksum */
    hapi_minval_t MinValue; /**< Minimum value */
    hapi_maxval_t MaxValue; /**< Maximum value */
    hapi_meanval_t MeanValue; /**< Mean value */
    hapi_stddval_t StandDeviationValue; /**< Standard deviation value */
    hapi_reserved4_t ReservedLocation4; /**< Reserved */
    hapi_reserved5_t ReservedLocation5; /**< Reserved */
    hapi_hfsourcesafeswitch_t HFSourceSafeSwitch; /**< Switch SCLK_HF source */
    hapi_compain_t SelectCompAInput; /**< Select CompA input */
    hapi_comparef_t SelectCompARef; /**< Select CompA ref */
    hapi_adccompbin_t SelectADCCompBInput; /**< Select ADC CompB input */
#ifdef CPU_VARIANT_X2
    hapi_dacvref_t SelectDACVref; /**< Select DAC Vref */
#else
    hapi_compbref_t SelectCompBRef; /**< Select CompB ref */
#endif
} rom_hapi_t;

#define ROM_HAPI_BASE (0x10000048) /**< Base address por ROM hardware API */
#define ROM_HAPI ((hapi_t *) (ROM_HAPI_TABLE_BASE)) /**< HAPI functions */

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif /* CC26XX_CC13XX_HAPI_H */

/*@}*/
