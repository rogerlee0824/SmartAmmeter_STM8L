#include "cc112x.h"

cc112x_event_t cc112x_event;

/***********************************************************************
  * @brief  Deal with the event handler for cc112x.
  * @param  p_event_data
  * @param  event_size
  * @retval None
************************************************************************/
void cc112x_event_handler(void * p_event_data, uint16_t event_size)
{
    cc112x_event_t * p_cc112x_event_temp = p_event_data;
    
    switch(p_cc112x_event_temp->eCC112x_event)
    {
        case CC112X_INIT_EVENT:
            
            break;
            
        case CC112X_TRANSMIT_EVENT:
        
            break;
            
        case CC112X_RECEIVE_EVENT:
        
            break;
            
        default:
            break;
    }
}