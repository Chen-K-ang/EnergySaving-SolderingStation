#ifndef __MOTOR_H
#define __MOTOR_H	
#include "sys.h"

void Delay(unsigned int count);
void MotoRcw(void);
void MotoRccw(void) ;
void MotorStop(void);
void Motor_Ctrl_Direction_Angle(int direction, int angle);
void MotoR_GPIO(void);

#endif 
