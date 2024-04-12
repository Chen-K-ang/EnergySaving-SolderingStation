#ifndef _ADC_H
#define _ADC_H

#include "sys.h"
#include "delay.h"

void adc_init(void);
u16 get_adc(u8 ch);
u16 get_adc_average(u8 ch, u8 times);


#endif

