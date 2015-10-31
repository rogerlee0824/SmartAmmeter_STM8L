#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "stm8l15x.h"
#include "includes.h"

extern uint16_t adc_value[];
//extern uint16_t ADCdata;
extern uint8_t adc_is_compeleted;
void battery_measure(float * p_bat_voltage);

#endif