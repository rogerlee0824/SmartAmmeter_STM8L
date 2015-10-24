#include "valve.h"
#include "includes.h"
#include "stm8l15x_gpio.h"

valve_event_t valve_event;

/***********************************************************************
  * @brief  Check the state of vlave.
  * @param  None
  * @retval VALVE_OPEN --- The valve is open;
  *         VALVE_CLOSE --- The valve is closed
************************************************************************/
emValve_state_t ChechValveState(void)
{
    emValve_state_t valve_state = VALVE_OPEN;
    
    GPIO_Init(GPIO_PORT_VALVE_MONITOR, GPIO_PIN_VALVE_MONITOR, GPIO_Mode_In_FL_No_IT);
    delay1ms(2);
    if(GPIO_ReadInputDataBit(GPIO_PORT_VALVE_MONITOR, GPIO_PIN_VALVE_MONITOR) == RESET)
    {
        #ifdef VALVE_DEBUG
        printf("Close!!!\r\n");
        #endif
        valve_state = VALVE_CLOSE;                   // Close
    }
    #ifdef VALVE_DEBUG
    else
    {
        printf("Open!!!\r\n");
    }
    #endif
    
    return valve_state;
}

/***********************************************************************
  * @brief  Deal with the event handler for valve.
  * @param  p_event_data
  * @param  event_size
  * @retval None
************************************************************************/
void valve_event_handler(void * p_event_data, uint16_t event_size)
{
	valve_event_t * p_valve_event = p_event_data;
	uint16_t time_out = 1000;
	
	switch(p_valve_event->eValve_event)
	{
        case VALVE_OPEN_EVENT:
            #ifdef VALVE_DEBUG
            printf("[VALVE] VALVE_OPEN_EVENT\r\n");
            #endif
            /* Disable interrupts */
            disableInterrupts();
                
            GPIO_Init(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA, GPIO_Mode_Out_PP_Low_Fast);
            GPIO_Init(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB, GPIO_Mode_Out_PP_Low_Fast);
            GPIO_SetBits(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA);
            GPIO_ResetBits(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB);
                
            delay1ms(500); 
            while((ChechValveState() == VALVE_CLOSE) && (--time_out))
            {
                #ifdef VALVE_DEBUG
                printf("[VALVE] time_out = %d\r\n",time_out);
                #endif
                delay1ms(5);
            }

            if(time_out)
            {
                /* Stop */
                GPIO_SetBits(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA);
                GPIO_SetBits(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB);
                    
                #ifdef VALVE_DEBUG
                printf("[VALVE] Has open!!!\r\n");
                #endif
            }
            #ifdef VALVE_DEBUG
            else
            { 
                printf("[VALVE] time_out!!!\r\n");
            }
            #endif
                
            valve_event.eValve_event = VALVE_STANDBY_EVENT;
            app_sched_event_put(&valve_event,sizeof(valve_event),valve_event_handler);
                    
            /* Enable interrupts */
            enableInterrupts();
			break;
		
		case VALVE_CLOSE_EVENT:
            #ifdef VALVE_DEBUG
            printf("[VALVE] VALVE_CLOSE_EVENT\r\n");
            #endif
			if(ChechValveState() == VALVE_OPEN)
			{
                /* Disable interrupts */
                disableInterrupts();
                
				GPIO_Init(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA, GPIO_Mode_Out_PP_Low_Fast);
				GPIO_Init(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB, GPIO_Mode_Out_PP_Low_Fast);
                GPIO_ResetBits(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA);
				GPIO_SetBits(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB);
				
				while((ChechValveState() == VALVE_OPEN) && (--time_out))
				{
                    #ifdef VALVE_DEBUG
                    printf("[VALVE] time_out = %d\r\n",time_out);
                    #endif
					delay1ms(2);
				}

				if(!time_out)
				{
                    /* Stop */
                    GPIO_SetBits(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA);
                    GPIO_SetBits(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB);
                    #ifdef VALVE_DEBUG
                    printf("[VALVE] Has closed!!!\r\n");
                    #endif
				}
                #ifdef VALVE_DEBUG
                else
                {
                    printf("[VALVE] time_out!!!\r\n");
                }
                #endif
                
                valve_event.eValve_event = VALVE_STANDBY_EVENT;
                app_sched_event_put(&valve_event,sizeof(valve_event),valve_event_handler);
                    
                /* Enable interrupts */
                enableInterrupts();
			}
			break;
            
        case VALVE_STANDBY_EVENT:
            #ifdef VALVE_DEBUG
            printf("[VALVE] VALVE_STANDBY_EVENT\r\n");
            #endif
            
            GPIO_Init(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA, GPIO_Mode_Out_PP_Low_Fast);
			GPIO_Init(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB, GPIO_Mode_Out_PP_Low_Fast);
			GPIO_Init(GPIO_PORT_VALVE_MONITOR, GPIO_PIN_VALVE_MONITOR, GPIO_Mode_Out_PP_Low_Fast);
                
            /* Standby */
            GPIO_ResetBits(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA);
            GPIO_ResetBits(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB);			
            break;
		
		default:
            break;	
	}
}