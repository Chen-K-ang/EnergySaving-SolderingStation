#include "motor.h"
#include "delay.h"
unsigned int phaseF[4] ={0x0008,0x0004,0x0002,0x0001};// D-C-B-A   反转
unsigned int phaseZ[4]={0x0001,0x0002,0x0004,0x0008};// A-B-C-D   正转
void Delay(unsigned int count)						  	//延时函数
{
	unsigned int i;
	for(;count!=0;count--)
	{
		i=1000;
		while(i--);
	}	
}
void MotoRcw(void)  //反转
{  
    int  i;  
    for(i=0;i<4;i++)  
    {  
       GPIO_Write(GPIOC,phaseF[i]);
       Delay(100);  
    }  
}
void MotoRccw(void)  //正转
{  
    int i; 
    for(i=0;i<4;i++)  
    {  
        GPIO_Write(GPIOC,phaseZ[i]); 
       Delay(100);    
    }  
}
void MotorStop(void) //停止
{  
      GPIO_Write(GPIOC,0x0000); 
}
//控制电机正转还是反转某个角度
//direction方向，1为正转，0为反转
//angle角度，可为0-360具有实际意义
void Motor_Ctrl_Direction_Angle(int direction, int angle)
{
	u16 j;
	if(direction == 1)
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRccw();//正转
		}
		 MotorStop();//停止
  }
	else
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRcw();//反转
		}
		 MotorStop();//停止
	}
}
void MotoR_GPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 	//使能GPIOC时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PC0-PC3引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	//配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //GPIOC速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化PC	
}


















