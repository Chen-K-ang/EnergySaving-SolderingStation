#ifndef _1602_H
#define _1602_H

#include "sys.h"
#include "delay.h"

#define RS PAout(13)
#define EN PAout(15)

//LCD1602ָ��
//��ʾģʽ����ָ��
// DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
//  0   0   1   DL  N   F   *   *
// DL = 1��8λ���ݽӿڣ� DL = 0��4λ���ݽӿڡ�
// N  = 1��������ʾ��    N  = 0��������ʾ��
// F  = 1��5*10�����ַ���F  = 0, 5*8�����ַ�
#define LCD_MODE_PIN8      0x38 //8λ���ݽӿڣ����У�5*8����
#define LCD_MODE_PIN4      0x28 //4λ���ݽӿڣ����У�5*8����

#define LCD_SCREEN_CLR     0x01 //����
#define LCD_CURSOR_RST     0x02 //��긴λ

//��ʾ���ؿ���ָ��
#define LCD_DIS_CUR_BLK_ON 0x0F //��ʾ������꿪�������˸
#define LCD_DIS_CUR_ON     0x0E //��ʾ������꿪����겻��˸
#define LCD_DIS_ON         0x0C //��ʾ�������أ���겻��˸
#define LCD_DIS_OFF        0x08 //��ʾ�أ����أ���겻��˸

//��ʾģʽ����
#define LCD_CURSOR_RIGHT   0x06 //������ƣ���ʾ���ƶ�(proteus�������)
#define LCD_CURSOR_LEFT    0x04 //������ƣ���ʾ���ƶ�
#define LCD_DIS_MODE_RIGHT 0x05 //������AC�Լ�������ƽ��
#define LCD_DIS_MODE_LEFT  0x07 //������AC����������ƽ��

//��ꡢ��ʾ�ƶ�ָ��
#define LCD_CUR_MOVE_LEFT  0x10 //�������
#define LCD_CUR_MOVE_RIGHT 0x14 //�������
#define LCD_DIS_MOVE_LEFT  0x18 //��ʾ����
#define LCD_DIS_MOVE_RIGHT 0x1C //��ʾ����

//����
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_MODE_FIR       0x80 //��һ����ʾ
#define LCD_MODE_SEC       0xC0 //�ڶ�����ʾ

void lcd_char_write(unsigned int x_pos,unsigned int y_pos,unsigned int lcd_dat);
void lcd_system_reset(void);
void LCD1602_write_data(unsigned int command);
void lcd_busy_wait(void);
void lcd_delay( unsigned char ms);
void GPIO_InitStructReadtempCmd(void);
void LCD1602_Show_Str(u8 x, u8 y, char *str);
#endif
