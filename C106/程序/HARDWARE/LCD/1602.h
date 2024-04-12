#ifndef _1602_H
#define _1602_H

#include "sys.h"
#include "delay.h"

#define RS PAout(11)
#define EN PAout(12)

void lcd_char_write(unsigned int x_pos,unsigned int y_pos,unsigned int lcd_dat);
void lcd_system_reset(void);
void lcd_command_write( unsigned int command);
void lcd_busy_wait(void);
void lcd_delay( unsigned char ms);
void GPIO_InitStructReadtempCmd(void);

#endif
