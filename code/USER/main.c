#include "LCD1602.h"
#include "adc.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "timer.h"
#include "24cxx.h"
#include <stdio.h>

#define ABS(x) (((x) > 0) ? (x) : (-x))  //����ֵ����
#define CUR_LIMIT 15 //��ȫ��������ֵ

union float_data
{
	float f_data;
	unsigned char byte[4];
};

union float_data temp_set; //�����崢�渡����

//ȫ�ֱ���
unsigned char set_flag = 0;   //0��������ʾ 1�������¶�
unsigned char mode_flag = 1;  //0��˯��     1��������̨
unsigned int temp_val = 0;//��ǰ�¶�ֵ����̨�趨ֵ

float cur_val = 0, cur_val_limit = CUR_LIMIT;  //����ֵ

void keyval_deal(unsigned char val);
unsigned char lcd_display(void);

int main()
{
	//�ֲ���������
	unsigned char key_val = 0;
	unsigned int vout_val = 0;
	float  val_temp = 0, cur_val_temp = 0;

	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //����JTAG
	//��ʼ��
	delay_init();
	LED_Init();
	BEEP_Init();
	KEY_Init();
	adc_init();
	AT24CXX_Init();
	TIM3_Int_Init(4999, 7199); //500ms��ʱ�ж�
	TIM4_PWM_Init(199,7199);  //ֵ200=100%���� ���ȵ���˿pwm
	LCD1602_Init();
	LCD1602_ClearScreen();

	//��ȡ������¶��趨ֵ
	AT24CXX_Read(0x00, temp_set.byte, 4);
	if (temp_set.f_data > 600.0f)  //�ж��Ƿ��ȡ��
		temp_set.f_data = 0.0f;
	delay_ms(20);
	LCD1602_Show_Str(0, 1, "Sleep");
	while (1) {
		//�����ɼ��봦���ֵ
		key_val = KEY_Scan(0);
		keyval_deal(key_val);
		//��ѹ���¶Ȳɼ�
		vout_val = get_adc_average(0, 5);  //�ɼ�5�Σ�ȡ��ֵ
		temp_val = get_adc_average(1, 5);
		//���ݴ���
		cur_val_temp = (float)vout_val * (3.3 / 4096);  //��6��Ƶģʽ�µ�����ֵ����ת��
		cur_val = (cur_val_temp - 2.5) * 10;  //ʵ�ʵ�·����ֵ
		if (cur_val > cur_val_limit) {
			BEEP = 1;    //��������
			RELAY = 0;   //�Ͽ�����˿
			TIM_Cmd(TIM4, DISABLE); //�رյ���˿����
			while (1);  //��ʱ������ѭ����ֻ�к�̨�β�ͷ�ϵ�ſ���
		}
		//lcd��ʾ
		lcd_display();
		//����˿����+ָʾ����ʾ
		if (temp_set.f_data < temp_val) {   //ʵ���¶ȱ��趨�¶ȸߣ���Ҫ���£�led���ȵ�Ϩ��
			TIM_SetCompare1(TIM4, 0);
			LED = 1;
		} else {
			val_temp = temp_set.f_data - temp_val;
			LED = 0;           //���ڼ��ȵ���״̬��led������
			if (val_temp > 200)
				val_temp = 200;    //200Ϊpwm���ֵ 100%
			TIM_SetCompare1(TIM4, val_temp);
		}
	}

	return 0;
}

void keyval_deal(unsigned char val)
{
	switch (val) {
		//������˯��
		case 1:	
			mode_flag++;
			if(mode_flag > 1)
				mode_flag = 0;
			LCD1602_Write_Cmd(0x01);  //����
			break;
		//����ģʽ
		case 2:
			set_flag++;
			if(set_flag > 1)
				set_flag = 0;
			break;
		//��
		case 3:
			temp_set.f_data++;
			break;
		//��
		case 4:
			temp_set.f_data--;
			break;
		default:
			break;
	}
}

unsigned char lcd_display(void)
{
	char str[6] = {0}; //��ӡ���ݴ�������
	//˯��ģʽ
	if (!mode_flag) {
		TIM_SetCompare1(TIM4, 0);  //�رռ��ȹ���
		LCD1602_Show_Str(0, 1, "Sleep");
		return 0;
	}
	//������̨

	LCD1602_Show_Str(0, 1, "Temp:");
	//����ģʽ
	if (set_flag) {
		LCD1602_Show_Str(2, 0, "                    ");
		LCD1602_Show_Str(2, 1, "                    ");
		sprintf(str, "%.1f", temp_set.f_data);
		LCD1602_Show_Str(7, 1, str);
		AT24CXX_Write(0x00, temp_set.byte, 4);  //������ֵд��Ƭ�ⴢ��
		return 0;
	}
	
	//��ʾģʽ
	sprintf(str, "%.1f", (float)(temp_val));  //��ǰ�¶�
	LCD1602_Show_Str(2, 1, str);

	LCD1602_Show_Str(0, 0, "Cur:");  //��ǰ����
	sprintf(str, "%.1f", cur_val);
	LCD1602_Show_Str(7, 0, str);

	return 1;
}

//�жϴ���ʱ�����
void TIM3_IRQHandler(void)
{
	static float last_cur_val = 0;
	static unsigned int work_count = 0;
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���±�־λ
		
		if ((ABS(last_cur_val - cur_val) < 0.2) && mode_flag) {//���ȵ�����ֵС��0.2�����Ҵ��ڹ���״̬
			work_count++;
			last_cur_val = cur_val; //�����¶�ֵ��Ϊ�´��ж���׼��
		}

		if (work_count > 600) {//5min������ʵ�ʼ���ʱ�䣬����Ϊ˯��ģʽ
			work_count++;
			BEEP = 0;
			temp_set.f_data = 0;
			mode_flag = 0;
		} else if (work_count > 606) { //����3s��رշ����������¿�ʼ����
			work_count = 0;
			BEEP = 1;
		}
	}
}
