#include "ic.h"
//#include "at88sc102.h"
#include "sle4442.h"
#include "includes.h"

ic_event_t ic_card_event;

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
	uint8_t yemp[20] = {0},i;
    
    switch(ic_event_temp->eIC_event)
    {
        case IC_CARD_INIT:
            SLE4442_Init();
			SLE4442_Reset();
			//SLE4442_ReadMainMem(0, yemp, 5);
			SLE4442_Verify(&yemp[0]);
			
			ic_card_event.eIC_event = IC_CARD_INIT;
	app_sched_event_put(&ic_card_event,sizeof(ic_event_t),ic_event_handler);
            break;
            
        case IC_CARD_INSERT:
			
            break;

        default:
            break;
    }
}