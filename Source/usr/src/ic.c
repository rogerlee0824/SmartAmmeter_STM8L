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
	uint8_t check_temp[3] = {0xff,0xff,0xff};
	uint8_t yemp[256] = {0},i;
	uint8_t temp = 1;
    
    switch(ic_event_temp->eIC_event)
    {
        case IC_CARD_INIT:
            SLE4442_Init();
            break;
            
        case IC_CARD_INSERT:
			#ifdef IC_CARD_DEBUG
                printf("\r\nIC Card insert!!!\r\n");
            #endif
			SLE4442_I2C_Init();
			SLE4442_PowerON();

			if(SLE4442_Verify(check_temp))
			{
				#ifdef IC_CARD_DEBUG
                	printf("[IC] Verify OK...\r\n");
            	#endif
			}
			else
			{
				#ifdef IC_CARD_DEBUG
                	printf("[IC] Verify Fail...\r\n");
            	#endif
			}
			
			SLE4442_WriteMainMem(0x20, &temp);
			SLE4442_WriteMainMem(0x21, &temp);
			SLE4442_WriteMainMem(0x22, &temp);
			SLE4442_ReadMainMem(0x00, yemp, sizeof(yemp));
			#ifdef IC_CARD_DEBUG
				for(uint16_t i = 0;i < sizeof(yemp);i ++)
				{
					printf("0x%02x, ",yemp[i]);
					if(!((i + 1)%16))
					{
						printf("\r\n");
					}
				}
				printf("\r\n");
			#endif
            break;

		case IC_CARD_DEINIT:
			#ifdef IC_CARD_DEBUG
                printf("IC Card insert!!!\r\n");
            #endif
			SLE4442_I2C_DeInit();
			SLE4442_PowerOff();
			
            break;

        default:
            break;
    }
}