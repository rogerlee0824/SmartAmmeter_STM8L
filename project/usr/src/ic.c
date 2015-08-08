#include "ic.h"
#include "at88sc102.h"
#include "includes.h"

ic_event_t ic_event;

void IC_Card_Handle(void)
{


}

/***********************************************************************
  * @brief  Handle the event from IC Card.
  * @param  None
  * @retval None
************************************************************************/
void ic_event_handler(void * p_event_data, uint16_t event_size)
{
    ic_event_t * ic_event_temp = p_event_data;
    
    switch(ic_event_temp->eIC_event)
    {
        case IC_CARD_INIT:
            AT88SC102_Init();
            break;
            
        case IC_CARD_INSERT:
            IC_Card_Handle();
            break;

        default:
            break;
    }
}