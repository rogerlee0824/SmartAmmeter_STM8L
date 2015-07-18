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

#include "app_scheduler.h"
#include "stm8l15x.h"
#include <stdlib.h>
#include <string.h>
#include "os_util.h"

#define SCHED_MAX_EVENT_DATA_SIZE       4                   /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE                20                                          /**< Maximum number of events in the scheduler queue. */


/**@brief Structure for holding a scheduled event header. */
typedef struct 
{
    app_sched_event_handler_t handler;                      /**< Pointer to event handler to receive the event. */
    uint16_t                  event_data_size;              /**< Size of event data. */
} event_header_t;

STATIC_ASSERT(sizeof(event_header_t) <= APP_SCHED_EVENT_HEADER_SIZE);

static event_header_t * m_queue_event_headers;              /**< Array for holding the queue event headers. */
static uint8_t *        m_queue_event_data;                 /**< Array for holding the queue event data. */
static volatile uint8_t m_queue_start_index;                /**< Index of queue entry at the start of the queue. */
static volatile uint8_t m_queue_end_index;                  /**< Index of queue entry at the end of the queue. */
static uint16_t         m_queue_event_size;                 /**< Maximum event size in queue. */
static uint16_t         m_queue_size;                       /**< Number of queue entries. */

/**@brief Macro for checking if a queue is full. */
#define APP_SCHED_QUEUE_FULL()    (next_index(m_queue_end_index) == m_queue_start_index)

/**@brief Macro for checking if a queue is empty. */
#define APP_SCHED_QUEUE_EMPTY()   (m_queue_end_index == m_queue_start_index)

/**@brief Function for incrementing a queue index, and handle wrap-around.
 *
 * @param[in]   index   Old index.
 *
 * @return      New (incremented) index.
 */
static inline uint8_t next_index(uint8_t index)
{
    return (index < m_queue_size) ? (index + 1) : 0;
}

/**@brief Macro for initializing the event scheduler.
 *
 * @details It will also handle dimensioning and allocation of the memory buffer required by the
 *          scheduler, making sure the buffer is correctly aligned.
 *
 * @param[in] EVENT_SIZE   Maximum size of events to be passed through the scheduler.
 * @param[in] QUEUE_SIZE   Number of entries in scheduler queue (i.e. the maximum number of events
 *                         that can be scheduled for execution).
 *
 * @note Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *       several times as long as it is from the same location, e.g. to do a reinitialization).
 */
void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

/**@brief Function for initializing the Scheduler.
 *
 * @details It must be called before entering the main loop.
 *
 * @param[in]   max_event_size   Maximum size of events to be passed through the scheduler.
 * @param[in]   queue_size       Number of entries in scheduler queue (i.e. the maximum number of
 *                               events that can be scheduled for execution).
 * @param[in]   p_event_buffer   Pointer to memory buffer for holding the scheduler queue. It must
 *                               be dimensioned using the APP_SCHED_BUFFER_SIZE() macro. The buffer
 *                               must be aligned to a 4 byte boundary.
 *
 * @note Normally initialization should be done using the APP_SCHED_INIT() macro, as that will both
 *       allocate the scheduler buffer, and also align the buffer correctly.
 *
 * @retval      NRF_SUCCESS               Successful initialization.
 * @retval      NRF_ERROR_INVALID_PARAM   Invalid parameter (buffer not aligned to a 4 byte
 *                                        boundary).
 */
uint32_t app_sched_init(uint16_t event_size, uint16_t queue_size, void * p_event_buffer)
{
    uint16_t data_start_index = (queue_size + 1) * sizeof(event_header_t);
    
    // Check that buffer is correctly aligned
    if (!is_word_aligned(p_event_buffer))
    {
        return OS_ERROR_INVALID_PARAM;
    }
    
    // Initialize event scheduler
    m_queue_event_headers = p_event_buffer;
    m_queue_event_data    = &((uint8_t *)p_event_buffer)[data_start_index];
    m_queue_end_index     = 0;
    m_queue_start_index   = 0;
    m_queue_event_size    = event_size;
    m_queue_size          = queue_size;

    return OS_SUCCESS;
}

/**@brief Function for scheduling an event.
 *
 * @details Puts an event into the event queue.
 *
 * @param[in]   p_event_data   Pointer to event data to be scheduled.
 * @param[in]   p_event_size   Size of event data to be scheduled.
 * @param[in]   handler        Event handler to receive the event.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t app_sched_event_put(void *                    p_event_data,
                             uint16_t                  event_data_size,
                             app_sched_event_handler_t handler)
{
    uint32_t err_code;

    if (event_data_size <= m_queue_event_size)
    {
        uint16_t event_index = 0xFFFF;
            
        CRITICAL_REGION_ENTER();
            
        if (!APP_SCHED_QUEUE_FULL())
        {
            event_index       = m_queue_end_index;
            m_queue_end_index = next_index(m_queue_end_index);
        }
            
        CRITICAL_REGION_EXIT();
        
        if (event_index != 0xFFFF)
        {
            // NOTE: This can be done outside the critical region since the event consumer will
            //       always be called from the main loop, and will thus never interrupt this code.
            m_queue_event_headers[event_index].handler = handler;
            if ((p_event_data != NULL) && (event_data_size > 0))
            {
                memcpy(&m_queue_event_data[event_index * m_queue_event_size],
                       p_event_data,
                       event_data_size);
                m_queue_event_headers[event_index].event_data_size = event_data_size;
            }
            else                                    /* The event is invalid. */
            {
                m_queue_event_headers[event_index].event_data_size = 0;
            }

            err_code = OS_SUCCESS;
        }
        else
        {
            err_code = OS_ERROR_NO_MEM;
        }
    }
    else
    {
        err_code = OS_ERROR_INVALID_LENGTH;
    }

    return err_code;
}


/**@brief Function for reading the next event from specified event queue.
 *
 * @param[out]  pp_event_data       Pointer to pointer to event data.
 * @param[out]  p_event_data_size   Pointer to size of event data.
 * @param[out]  p_event_handler     Pointer to event handler function pointer.
 *
 * @return      OS_SUCCESS if new event, OS_ERROR_NOT_FOUND if event queue is empty.
 */
static uint32_t app_sched_event_get(void **                     pp_event_data,
                                    uint16_t *                  p_event_data_size,
                                    app_sched_event_handler_t * p_event_handler)
{
    uint32_t err_code = OS_ERROR_NOT_FOUND;

    if (!APP_SCHED_QUEUE_EMPTY())
    {
        uint16_t event_index;
        
        // NOTE: There is no need for a critical region here, as this function will only be called
        //       from app_sched_execute() from inside the main loop, so it will never interrupt
        //       app_sched_event_put(). Also, updating of (i.e. writing to) the start index will be
        //       an atomic operation.
        event_index         = m_queue_start_index;
        m_queue_start_index = next_index(m_queue_start_index);
        
        *pp_event_data     = &m_queue_event_data[event_index * m_queue_event_size];
        *p_event_data_size = m_queue_event_headers[event_index].event_data_size;
        *p_event_handler   = m_queue_event_headers[event_index].handler;

        err_code = OS_SUCCESS;
    }
    return err_code;
}

/**@brief Function for executing all scheduled events.
 *
 * @details This function must be called from within the main loop. It will execute all events
 *          scheduled since the last time it was called.
 */
void app_sched_execute(void)
{
    void *                    p_event_data;
    uint16_t                  event_data_size;
    app_sched_event_handler_t event_handler;

    // Get next event (if any), and execute handler
    while ((app_sched_event_get(&p_event_data, &event_data_size, &event_handler) == OS_SUCCESS))
    {
        event_handler(p_event_data, event_data_size);
    }
}

/**@brief Function for waiting an event.
 *
 * @details Go to the low power mode and wait for a new event.
 *
 * @param[in]   void
 *
 * @return      void
 */
void app_evt_wait(void)
{
	if(APP_SCHED_QUEUE_EMPTY())
	{
		wfi();	
	}
}
