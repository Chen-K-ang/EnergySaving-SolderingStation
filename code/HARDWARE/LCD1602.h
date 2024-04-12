/*1602.h 文件*/
#ifndef __LCD1602_H
#define __LCD1602_H	  
#include "stm32f10x.h"
#include "delay.h"
#define LCD_DATA_PORT GPIOB
 
//1602指令/数据 引脚
#define	LCD_RS_Set()	GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define	LCD_RS_Clr()	GPIO_ResetBits(GPIOB, GPIO_Pin_13)
 
//1602读写引脚
#define	LCD_RW_Set()	GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define	LCD_RW_Clr()	GPIO_ResetBits(GPIOB, GPIO_Pin_14)
 
//1602使能引脚
#define	LCD_EN_Set()	GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define	LCD_EN_Clr()	GPIO_ResetBits(GPIOB, GPIO_Pin_15)
 
//1602数据端口	D4~7
#define	DATAOUT(x)	GPIO_Write(LCD_DATA_PORT, x)
 
#define u8  unsigned char 
 
void LCD1602_Wait_Ready(void);
void LCD1602_Write_Cmd(u8 cmd);
void LCD1602_Write_Dat(u8 dat);
void LCD1602_ClearScreen(void);
void LCD1602_Set_Cursor(u8 x, u8 y);
void LCD1602_Show_Str(u8 x, u8 y, char *str);
void LCD1602_Init(void);
 
#endif
