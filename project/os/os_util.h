/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup app_util Utility Functions and Definitions
 * @{
 * @ingroup app_common
 *
 * @brief Various types and definitions available to all applications.
 */

#ifndef APP_UTIL_H__
#define APP_UTIL_H__

#include "os_error.h"
#include "stm8l15x.h"

/**@brief The interrupt priorities available to the application while the softdevice is active. */
typedef enum
{
    APP_IRQ_PRIORITY_HIGH = 1,
    APP_IRQ_PRIORITY_LOW  = 3
} app_irq_priority_t;

enum
{
    UNIT_0_625_MS = 625,                                /**< Number of microseconds in 0.625 milliseconds. */
    UNIT_1_25_MS  = 1250,                               /**< Number of microseconds in 1.25 milliseconds. */
    UNIT_10_MS    = 10000                               /**< Number of microseconds in 10 milliseconds. */
};

#define NRF_APP_PRIORITY_THREAD    4                    /**< "Interrupt level" when running in Thread Mode. */

/**@cond NO_DOXYGEN */
#define EXTERNAL_INT_VECTOR_OFFSET 16
/**@endcond */

#define PACKED(TYPE) __packed TYPE

/**@brief Macro for doing static (i.e. compile time) assertion.
 *
 * @note If the assertion fails when compiling using Keil, the compiler will report error message
 *       "error: #94: the size of an array must be greater than zero" (while gcc will list the
 *       symbol static_assert_failed, making the error message more readable).
 *       If the supplied expression can not be evaluated at compile time, Keil will report
 *       "error: #28: expression must have a constant value".
 *
 * @note The macro is intentionally implemented not using do while(0), allowing it to be used
 *       outside function blocks (e.g. close to global type- and variable declarations).
 *       If used in a code block, it must be used before any executable code in this block.
 *
 * @param[in]   EXPR   Constant expression to be verified.
 */

#define STATIC_ASSERT(EXPR) typedef char static_assert_failed[(EXPR) ? 1 : -1]

/**@brief type for holding an encoded (i.e. little endian) 16 bit unsigned integer. */
typedef uint8_t uint16_le_t[2];

/**@brief type for holding an encoded (i.e. little endian) 32 bit unsigned integer. */
typedef uint8_t uint32_le_t[4];

/**@brief Byte array type. */
typedef struct
{
    uint16_t  size;                 /**< Number of array entries. */
    uint8_t * p_data;               /**< Pointer to array entries. */
} uint8_array_t;

/**@brief Macro for entering a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
#define CRITICAL_REGION_ENTER()   disableInterrupts()
    
/**@brief Macro for leaving a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
#define CRITICAL_REGION_EXIT()    enableInterrupts()         
    
/**@brief Perform rounded integer division (as opposed to truncating the result).
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Rounded (integer) result of dividing A by B.
 */
#define ROUNDED_DIV(A, B) (((A) + ((B) / 2)) / (B))

/**@brief Check if the integer provided is a power of two.
 *
 * @param[in]   A   Number to be tested.
 *
 * @return      true if value is power of two.
 * @return      false if value not power of two.
 */
#define IS_POWER_OF_TWO(A) ( ((A) != 0) && ((((A) - 1) & (A)) == 0) )

/**@brief To convert ticks to millisecond
 * @param[in] time          Number of millseconds that needs to be converted.
 * @param[in] resolution    Units to be converted.
 */
#define MSEC_TO_UNITS(TIME, RESOLUTION) (((TIME) * 1000) / (RESOLUTION))


/**@brief Perform integer division, making sure the result is rounded up.
 *
 * @details One typical use for this is to compute the number of objects with size B is needed to
 *          hold A number of bytes.
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Integer result of dividing A by B, rounded up.
 */
#define CEIL_DIV(A, B)      \
    /*lint -save -e573 */   \
    ((((A) - 1) / (B)) + 1) \
    /*lint -restore */

/**@brief Function for encoding a uint16 value.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data is to be written.
 *
 * @return      Number of bytes written.
 */
static inline uint8_t uint16_encode(uint16_t value, uint8_t * p_encoded_data)
{
    p_encoded_data[0] = (uint8_t) ((value & 0x00FF) >> 0);
    p_encoded_data[1] = (uint8_t) ((value & 0xFF00) >> 8);
    return sizeof(uint16_t);
}
    
/**@brief Function for encoding a uint32 value.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data is to be written.
 *
 * @return      Number of bytes written.
 */
static inline uint8_t uint32_encode(uint32_t value, uint8_t * p_encoded_data)
{
    p_encoded_data[0] = (uint8_t) ((value & 0x000000FF) >> 0);
    p_encoded_data[1] = (uint8_t) ((value & 0x0000FF00) >> 8);
    p_encoded_data[2] = (uint8_t) ((value & 0x00FF0000) >> 16);
    p_encoded_data[3] = (uint8_t) ((value & 0xFF000000) >> 24);
    return sizeof(uint32_t);
}

/**@brief Function for decoding a uint16 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static inline uint16_t uint16_decode(const uint8_t * p_encoded_data)
{
        return ( (((uint16_t)((uint8_t *)p_encoded_data)[0])) | 
                 (((uint16_t)((uint8_t *)p_encoded_data)[1]) << 8 ));
}

/**@brief Function for decoding a uint32 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static inline uint32_t uint32_decode(const uint8_t * p_encoded_data)
{
    return ( (((uint32_t)((uint8_t *)p_encoded_data)[0]) << 0)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[1]) << 8)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[2]) << 16) |
             (((uint32_t)((uint8_t *)p_encoded_data)[3]) << 24 ));
}

/**@brief Function for checking if a pointer value is aligned to a 4 byte boundary.
 *
 * @param[in]   p   Pointer value to be checked.
 *
 * @return      TRUE if pointer is aligned to a 4 byte boundary, FALSE otherwise.
 */
static inline bool is_word_aligned(void * p)
{
    return (((uint32_t)p & 0x00000003) == 0);
}

#endif // APP_UTIL_H__

/** @} */
