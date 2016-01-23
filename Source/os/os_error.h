/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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
 * @defgroup app_error Common application error handler
 * @{
 * @ingroup app_common
 *
 * @brief Common application error handler and macros for utilizing a common error handler.
 */

#ifndef APP_ERROR_H__
#define APP_ERROR_H__

#include "stm8l15x.h"
#include <stdbool.h>
#include "includes.h"

#define OS_ERROR_BASE_NUM                      (0x0)       ///< Global error base
#define OS_ERROR_SDM_BASE_NUM                  (0x1000)    ///< SDM error base
#define OS_ERROR_SOC_BASE_NUM                  (0x2000)    ///< SoC error base
#define OS_ERROR_STK_BASE_NUM                  (0x3000)    ///< STK error base

#define OS_SUCCESS                             (OS_ERROR_BASE_NUM + 0)  ///< Successful command
#define OS_ERROR_SVC_HANDLER_MISSING           (OS_ERROR_BASE_NUM + 1)  ///< SVC handler is missing
#define OS_ERROR_SOFTDEVICE_NOT_ENABLED        (OS_ERROR_BASE_NUM + 2)  ///< SoftDevice has not been enabled
#define OS_ERROR_INTERNAL                      (OS_ERROR_BASE_NUM + 3)  ///< Internal Error
#define OS_ERROR_NO_MEM                        (OS_ERROR_BASE_NUM + 4)  ///< No Memory for operation
#define OS_ERROR_NOT_FOUND                     (OS_ERROR_BASE_NUM + 5)  ///< Not found
#define OS_ERROR_NOT_SUPPORTED                 (OS_ERROR_BASE_NUM + 6)  ///< Not supported
#define OS_ERROR_INVALID_PARAM                 (OS_ERROR_BASE_NUM + 7)  ///< Invalid Parameter
#define OS_ERROR_INVALID_STATE                 (OS_ERROR_BASE_NUM + 8)  ///< Invalid state, operation disallowed in this state
#define OS_ERROR_INVALID_LENGTH                (OS_ERROR_BASE_NUM + 9)  ///< Invalid Length
#define OS_ERROR_INVALID_FLAGS                 (OS_ERROR_BASE_NUM + 10) ///< Invalid Flags
#define OS_ERROR_INVALID_DATA                  (OS_ERROR_BASE_NUM + 11) ///< Invalid Data
#define OS_ERROR_DATA_SIZE                     (OS_ERROR_BASE_NUM + 12) ///< Data size exceeds limit
#define OS_ERROR_TIMEOUT                       (OS_ERROR_BASE_NUM + 13) ///< Operation timed out
#define OS_ERROR_NULL                          (OS_ERROR_BASE_NUM + 14) ///< Null Pointer
#define OS_ERROR_FORBIDDEN                     (OS_ERROR_BASE_NUM + 15) ///< Forbidden Operation
#define OS_ERROR_INVALID_ADDR                  (OS_ERROR_BASE_NUM + 16) ///< Bad Memory Address
#define OS_ERROR_BUSY                          (OS_ERROR_BASE_NUM + 17) ///< Busy
 
#ifdef  USR_ERROR_CHECK 
/**@brief Function for error handling, which is called when an error has occurred. 
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint16_t error_code, uint16_t line_num, const uint8_t * p_file_name);

/**@brief Macro for calling error handler function. 
 * @param[in] ERR_CODE Error code supplied to the error handler.
*/ 
#define APP_ERROR_HANDLER(ERR_CODE)                         \
    do                                                      \
    {                                                       \
        app_error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
    } while (0)

/**@brief Macro for calling error handler function if supplied error code any other than OS_SUCCESS. 
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */    
#define APP_ERROR_CHECK(ERR_CODE)                           \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != OS_SUCCESS)                  \
        {                                                   \
            APP_ERROR_HANDLER(LOCAL_ERR_CODE);              \
        }                                                   \
    } while (0)    
    
/**@brief Macro for calling error handler function if supplied boolean value is false. 
 * @param[in] BOOLEAN_VALUE Boolean value to be evaluated.
 */    
#define APP_ERROR_CHECK_BOOL(BOOLEAN_VALUE)                 \
    do                                                      \
    {                                                       \
        const bool LOCAL_BOOLEAN_VALUE = (BOOLEAN_VALUE);   \
        if (!LOCAL_BOOLEAN_VALUE)                           \
        {                                                   \
            APP_ERROR_HANDLER(0);                           \
        }                                                   \
    } while (0)
#else
#define APP_ERROR_CHECK(...) 
#define APP_ERROR_CHECK_BOOL(...)		
#endif		/* USE_ERROR_CHECK */

#endif // APP_ERROR_H__

/** @} */
