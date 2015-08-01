#include "valve.h"
#include "includes.h"
#include "stm8l15x_gpio.h"

valve_event_t valve_event;

emValve_state_t ChechValveState(void)
{
    emValve_state_t valve_state = VALVE_OPEN;
    
    GPIO_Init(GPIO_PORT_VALVE_MONITOR, GPIO_PIN_VALVE_MONITOR, GPIO_Mode_In_FL_No_IT);
    delay1ms(2);
    if(GPIO_ReadInputDataBit(GPIO_PORT_VALVE_MONITOR, GPIO_PIN_VALVE_MONITOR) == RESET)
    {
        printf("Close!!!\r\n");
        valve_state = VALVE_CLOSE;                   // Close
    }
    else
    {
        printf("Open!!!\r\n");
    }

    return valve_state;
}

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
                
            /* Standby */
            GPIO_ResetBits(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA);
            GPIO_ResetBits(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB);
                    
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
                
                /* Standby */
                GPIO_ResetBits(GPIO_PORT_VALVE_IA, GPIO_PIN_VALVE_IA);
                GPIO_ResetBits(GPIO_PORT_VALVE_IB, GPIO_PIN_VALVE_IB);
                    
                /* Enable interrupts */
                enableInterrupts();
			}
			break;
		
		default:
            break;	
	}
}