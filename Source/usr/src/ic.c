#include "ic.h"
#include "sle4442.h"
#include "includes.h"

ic_event_t ic_card_event;
uint8_t ic_default_atr[4] = {0xa2, 0x13, 0x10, 0x91};
uint8_t ic_default_manufacture[4] = {0xff, 0xff, 0x81, 0x15};

void IC_Card_CheckData(void)
{
	uint8_t yemp[256];

	SLE4442_ReadMainMem(0x00, yemp, sizeof(yemp));
	for(uint16_t i = 0;i < sizeof(yemp);i ++)
	{
		printf("0x%02x, ",yemp[i]);
		if(!((i + 1)%16))
		{
			printf("\r\n");
		}
	}
	printf("\r\n");
}

static uint8_t IC_Card_CheckICManufactureInfo(void)
{
	uint8_t state;
	uint8_t temp_rd[32];
	protect_area_header_t *p_protect_area_header;

	SLE4442_ReadMainMem(0x00, temp_rd, sizeof(temp_rd));
	p_protect_area_header = (protect_area_header_t *)temp_rd;
	if((p_protect_area_header->atr_header.atr[0] == ic_default_atr[0]) && 
		(p_protect_area_header->atr_header.atr[1] == ic_default_atr[1]) && 
		(p_protect_area_header->atr_header.atr[2] == ic_default_atr[2]) && 
		(p_protect_area_header->atr_header.atr[3] == ic_default_atr[3]) && 
		(p_protect_area_header->manufacture_header.arr[0] == ic_default_manufacture[0]) && 
		(p_protect_area_header->manufacture_header.arr[1] == ic_default_manufacture[1])	&&
		(p_protect_area_header->manufacture_header.arr[2] == ic_default_manufacture[2])	&&
		(p_protect_area_header->manufacture_header.arr[3] == ic_default_manufacture[3]))
	{
		state = 1;
	}
	else
	{
		state = 0;
	}

	return state;
}

static void IC_Card_ClearGas(void)
{
	uint8_t arr[4] = {0};
	
	for(uint8_t i = 0;i < sizeof(arr);i ++)
	{
		SLE4442_WriteMainMem(IC_CARD_GAS_START_ADDR + i, arr + i);
	}
}

static uint32_t IC_Card_GetNewGas(void)
{
	uint32_t_or_u8_u tmp;
	
	#ifdef IC_CARD_DEBUG
		printf("\r\n[IC] IC_Card_GetNewGas\r\n");
	#endif

	SLE4442_ReadMainMem(IC_CARD_GAS_START_ADDR, tmp.arr, sizeof(uint32_t));
	ConvertU32Endian(&tmp.l);
	
	if((tmp.arr[0] == 0xff) && (tmp.arr[1] == 0xff) &&(tmp.arr[2] == 0xff) && (tmp.arr[3] == 0xff))
	{
		return 0;
	}
	else
	{
		return (tmp.l);
	}

}

static void IC_Card_AddNewGas(void)
{
	uint32_t rd_num_temp = 0;
	uint32_t ic_new_gas = 0;

	#ifdef IC_CARD_DEBUG
		printf("\r\n[IC] IC_Card_AddNewGas\r\n");
	#endif
	
	ic_new_gas = IC_Card_GetNewGas() * GAS_COEFFICIENT;
	rd_num_temp = DataMem_GetTopGas();
	rd_num_temp += ic_new_gas;
	DataMem_SetTopGas(rd_num_temp);
	IC_Card_ClearGas();
}

/***********************************************************************
  * @brief  Handle the event from IC Card.
  * @param  None
  * @retval None
************************************************************************/
void ic_event_handler(void * p_event_data, uint16_t event_size)
{
    ic_event_t * ic_event_temp = p_event_data;
	uint8_t pass_temp[3] = {0xff,0xff,0xff};
	uint8_t check_state = 0;
    
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

			if(1 == IC_Card_CheckICManufactureInfo())
			{
				if(1 == SLE4442_Verify(pass_temp))
				{
					#ifdef IC_CARD_DEBUG
                	printf("[IC] Verify OK...\r\n");
            		#endif
					check_state = 1;
				}
				else
				{
					#ifdef IC_CARD_DEBUG
                		printf("[IC] Verify Fail...\r\n");
            		#endif
					check_state = 0;
				}

				if(1 == check_state)
				{
					IC_Card_AddNewGas();
					
					#ifdef IC_CARD_DEBUG
						IC_Card_CheckData();
					#endif

					lcd_event.eLcd_event = LCD_DISPLAY_REMAIN_GAS;
            		app_sched_event_put(&lcd_event,sizeof(lcd_event),lcd_event_handler);
				}
			}
			#ifdef IC_CARD_DEBUG
			else
			{
				printf("[IC] ***The IC Card is illegal***\r\n");
			}
			#endif
			
			ic_card_event.eIC_event = IC_CARD_DEINIT;
            app_sched_event_put(&ic_card_event,sizeof(ic_event_t),ic_event_handler);
            break;

		case IC_CARD_DEINIT:
			#ifdef IC_CARD_DEBUG
                printf("IC_CARD_DEINIT!!!\r\n");
            #endif
			SLE4442_I2C_DeInit();
			SLE4442_PowerOff();
            break;

        default:
            break;
    }
}
