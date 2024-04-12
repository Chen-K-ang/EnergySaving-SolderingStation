#include "1602.h"	
#include "delay.h"
//1602初始化
u16 temp;
void lcd_delay( unsigned char ms) /*LCD1602 延时*/
{
     unsigned int j;
    while(ms--){
        for(j=0;j<300;j++)
            {;}
        }
}

void GPIO_InitStructReadtempCmd(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void lcd_busy_wait(void) /*LCD1602 忙等待*/
{
	u8 sta;
    GPIOB->ODR=0xFF;
	RS=0; 
	do
	{	
	    EN = 1;
		lcd_delay(5);	
 		sta = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	}while(sta & 0x80);
	 EN=0;
}

void lcd_command_write( unsigned int command) /*LCD1602 命令字写入*/
{
    RS=0;
    EN=0;
	lcd_delay(5);
    temp=(temp&0xf00f)|(command<<4);
	GPIO_Write(GPIOB,temp);
	lcd_delay(10);
    EN=1;
	lcd_delay(10);
    EN=0;     
}


void lcd_system_reset(void) /*LCD1602 初始化*/
{
    lcd_command_write(0x38);
    lcd_command_write(0x38);
    lcd_command_write(0x38);
    lcd_command_write(0x0c);
    lcd_command_write(0x06);
    lcd_command_write(0x01);
    lcd_command_write(0x80); 
}

 void lcd_char_write(unsigned int x_pos,unsigned int y_pos,unsigned int lcd_dat) /*LCD1602 字符写入*/
{
    x_pos &= 0x0f; 
    y_pos &= 0x01; 
    if(y_pos==1) x_pos += 0x40;
    x_pos += 0x80;
    lcd_command_write(x_pos);
    lcd_delay(5);
    RS=1;
    EN=0;
	lcd_delay(5);
    temp=(temp&0xf00f)|(lcd_dat<<4);
	GPIO_Write(GPIOB,temp); 
	lcd_delay(10);
    EN=1;
	lcd_delay(10);
    EN=0; 
}

