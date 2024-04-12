#include "delay.h"
#include "sys.h" 
#include "adc.h"
#include "1602.h"
#include "motor.h"
#include "bsp_dht11.h"
#define LED1 PAout(13)
#define LED2 PAout(14)
#define MOTOR PCout(0)

unsigned char dis_vol1[] ={"Water:        %"};
unsigned char dis_vol2[] ={" Water Pump ON!"};
unsigned char dis_vol3[] ={"Water:        %"};
unsigned char dis_vol4[] ={"Water Pump OFF!"};
//函数声明
void display1(void);
void display2(void);
u16 H;
int main(void)
{	 
	u16 ADCValue;
	float voltage;
	u8 Mflag=2;
	u8 OPENflag=0;	//起初代表水泵关
	delay_init();
	Adc_Init();
	GPIO_InitStructReadtempCmd();
	MotoR_GPIO();
	lcd_system_reset();
	LED1=1;
	LED2=0;
	MOTOR=1;
	while(1)
	{
	    ADCValue=Get_Adc_Average(ADC_Channel_0,10);
		voltage=((float)ADCValue/4096)*3.3;//计算电压
		H=(10000*voltage)/33;
		if(H>400) //40%  阈值
		{
		  display2();
		  Mflag=1;		//储水量足够，关闭水泵
		}
		else
	   {
		 display1();
		 Mflag=0;	//储水量不足够，需要启动水泵
	   }  
	   if(Mflag==1&&OPENflag==1)
		{ 
		   	LED1=1;
           	LED2=0;
			MOTOR=1;//关闭水泵
		    Mflag=2;
		    OPENflag=0;	 //已经关好
		}
		else if(Mflag==0&&OPENflag==0)
	   {
		   LED1=0;
           LED2=1;
		   MOTOR=0;//开水泵
		   Mflag=2; 
		   OPENflag=1;	  //已经开好
           
	   }
	}
}

void display1()
{
     unsigned int i,j;
	 dis_vol1[8]=H/100+0x30;
	 dis_vol1[9]=H%100/10+0x30;
	 dis_vol1[10]='.';
	 dis_vol1[11]=H%10+0x30;
	 for(i=0;i<16;i++) 
	  lcd_char_write(i,0,dis_vol1[i]);  
  	 for(j=0;j<15;j++) 
 	  lcd_char_write(j,1,dis_vol2[j]);
}

void display2()
{	
     unsigned int i,j;
	 dis_vol3[8]=H/100+0x30;
	 dis_vol3[9]=H%100/10+0x30;
	 dis_vol3[10]='.';
	 dis_vol3[11]=H%10+0x30;
	 for(i=0;i<16;i++) 
	  lcd_char_write(i,0,dis_vol3[i]);  
  	 for(j=0;j<15;j++) 
 	  lcd_char_write(j,1,dis_vol4[j]);
}