#include "LCD1602.h"
#include "adc.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "timer.h"
#include "24cxx.h"
#include <stdio.h>

#define ABS(x) (((x) > 0) ? (x) : (-x))  //绝对值计算
#define CUR_LIMIT 15 //安全电流限制值

union float_data
{
	float f_data;
	unsigned char byte[4];
};

union float_data temp_set; //共用体储存浮点数

//全局变量
unsigned char set_flag = 0;   //0：正常显示 1：设置温度
unsigned char mode_flag = 1;  //0：睡眠     1：开启焊台
unsigned int temp_val = 0;//当前温度值、焊台设定值

float cur_val = 0, cur_val_limit = CUR_LIMIT;  //电流值

void keyval_deal(unsigned char val);
unsigned char lcd_display(void);

int main()
{
	//局部变量设置
	unsigned char key_val = 0;
	unsigned int vout_val = 0;
	float  val_temp = 0, cur_val_temp = 0;

	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁用JTAG
	//初始化
	delay_init();
	LED_Init();
	BEEP_Init();
	KEY_Init();
	adc_init();
	AT24CXX_Init();
	TIM3_Int_Init(4999, 7199); //500ms定时中断
	TIM4_PWM_Init(199,7199);  //值200=100%加热 加热电阻丝pwm
	LCD1602_Init();
	LCD1602_ClearScreen();

	//读取储存的温度设定值
	AT24CXX_Read(0x00, temp_set.byte, 4);
	if (temp_set.f_data > 600.0f)  //判断是否读取误
		temp_set.f_data = 0.0f;
	delay_ms(20);
	LCD1602_Show_Str(0, 1, "Sleep");
	while (1) {
		//按键采集与处理键值
		key_val = KEY_Scan(0);
		keyval_deal(key_val);
		//电压、温度采集
		vout_val = get_adc_average(0, 5);  //采集5次，取均值
		temp_val = get_adc_average(1, 5);
		//数据处理
		cur_val_temp = (float)vout_val * (3.3 / 4096);  //将6分频模式下的数据值进行转换
		cur_val = (cur_val_temp - 2.5) * 10;  //实际电路电流值
		if (cur_val > cur_val_limit) {
			BEEP = 1;    //持续报警
			RELAY = 0;   //断开电阻丝
			TIM_Cmd(TIM4, DISABLE); //关闭电阻丝加热
			while (1);  //此时程序死循环，只有焊台拔插头断电才可以
		}
		//lcd显示
		lcd_display();
		//电阻丝加热+指示灯显示
		if (temp_set.f_data < temp_val) {   //实际温度比设定温度高，需要降温，led加热灯熄灭
			TIM_SetCompare1(TIM4, 0);
			LED = 1;
		} else {
			val_temp = temp_set.f_data - temp_val;
			LED = 0;           //处于加热调整状态，led灯亮起
			if (val_temp > 200)
				val_temp = 200;    //200为pwm最大值 100%
			TIM_SetCompare1(TIM4, val_temp);
		}
	}

	return 0;
}

void keyval_deal(unsigned char val)
{
	switch (val) {
		//开启与睡眠
		case 1:	
			mode_flag++;
			if(mode_flag > 1)
				mode_flag = 0;
			LCD1602_Write_Cmd(0x01);  //清屏
			break;
		//设置模式
		case 2:
			set_flag++;
			if(set_flag > 1)
				set_flag = 0;
			break;
		//加
		case 3:
			temp_set.f_data++;
			break;
		//减
		case 4:
			temp_set.f_data--;
			break;
		default:
			break;
	}
}

unsigned char lcd_display(void)
{
	char str[6] = {0}; //打印数据储存数组
	//睡眠模式
	if (!mode_flag) {
		TIM_SetCompare1(TIM4, 0);  //关闭加热功能
		LCD1602_Show_Str(0, 1, "Sleep");
		return 0;
	}
	//开启焊台

	LCD1602_Show_Str(0, 1, "Temp:");
	//设置模式
	if (set_flag) {
		LCD1602_Show_Str(2, 0, "                    ");
		LCD1602_Show_Str(2, 1, "                    ");
		sprintf(str, "%.1f", temp_set.f_data);
		LCD1602_Show_Str(7, 1, str);
		AT24CXX_Write(0x00, temp_set.byte, 4);  //将设置值写入片外储存
		return 0;
	}
	
	//显示模式
	sprintf(str, "%.1f", (float)(temp_val));  //当前温度
	LCD1602_Show_Str(2, 1, str);

	LCD1602_Show_Str(0, 0, "Cur:");  //当前电流
	sprintf(str, "%.1f", cur_val);
	LCD1602_Show_Str(7, 0, str);

	return 1;
}

//中断处理长时间电流
void TIM3_IRQHandler(void)
{
	static float last_cur_val = 0;
	static unsigned int work_count = 0;
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //更新标志位
		
		if ((ABS(last_cur_val - cur_val) < 0.2) && mode_flag) {//加热电流差值小于0.2，并且处于工作状态
			work_count++;
			last_cur_val = cur_val; //更新温度值，为下次判断做准备
		}

		if (work_count > 600) {//5min，超过实际加热时间，设置为睡眠模式
			work_count++;
			BEEP = 0;
			temp_set.f_data = 0;
			mode_flag = 0;
		} else if (work_count > 606) { //报警3s后关闭蜂鸣器，重新开始计数
			work_count = 0;
			BEEP = 1;
		}
	}
}
