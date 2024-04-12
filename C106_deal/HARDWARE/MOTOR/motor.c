#include "motor.h"
#include "delay.h"
unsigned int phaseF[4] ={0x0008,0x0004,0x0002,0x0001};// D-C-B-A   ��ת
unsigned int phaseZ[4]={0x0001,0x0002,0x0004,0x0008};// A-B-C-D   ��ת
void Delay(unsigned int count)						  	//��ʱ����
{
	unsigned int i;
	for(;count!=0;count--)
	{
		i=1000;
		while(i--);
	}	
}
void MotoRcw(void)  //��ת
{  
    int  i;  
    for(i=0;i<4;i++)  
    {  
       GPIO_Write(GPIOC,phaseF[i]);
       Delay(100);  
    }  
}
void MotoRccw(void)  //��ת
{  
    int i; 
    for(i=0;i<4;i++)  
    {  
        GPIO_Write(GPIOC,phaseZ[i]); 
       Delay(100);    
    }  
}
void MotorStop(void) //ֹͣ
{  
      GPIO_Write(GPIOC,0x0000); 
}
//���Ƶ����ת���Ƿ�תĳ���Ƕ�
//direction����1Ϊ��ת��0Ϊ��ת
//angle�Ƕȣ���Ϊ0-360����ʵ������
void Motor_Ctrl_Direction_Angle(int direction, int angle)
{
	u16 j;
	if(direction == 1)
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRccw();//��ת
		}
		 MotorStop();//ֹͣ
  }
	else
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRcw();//��ת
		}
		 MotorStop();//ֹͣ
	}
}
void MotoR_GPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 	//ʹ��GPIOCʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PC0-PC3��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	//����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //GPIOC�ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);				//��ʼ��PC	
}


















