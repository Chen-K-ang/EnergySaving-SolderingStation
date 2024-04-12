#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define LED    PAout(8)  //加热指示灯
#define RELAY  PAout(2)  //继电器

void LED_Init(void);

#endif
