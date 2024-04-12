#include "delay.h"
#include "sys.h" 
#include "adc.h"
#include "key.h"
#include "1602.h"

#define BUZZER PAout(5)
#define LED1 PAout(8)
#define RELAY PAout(14)

unsigned char dis_vol1[] ={"Temp:          "};
unsigned char dis_vol2[] ={"Set:           "};

//ȫ�ֱ���
unsigned char set_flag = 0;   //0��������ʾ 1�������¶�
unsigned char mode_flag = 0;  //0��˯��     1��������̨

unsigned int temp_val = 0;//��ǰ�¶�ֵ
unsigned int temp_set = 0;//��ǰ�¶�ֵ


//��������
void display1(void);
void display2(void);
void keyval_deal(unsigned char val);
unsigned char lcd_display(void);
int main(void)
{
	//�ֲ���������
	unsigned char key_val = 0;
	unsigned int vout_val = 0;
	float  val_temp = 0, cur_val_temp = 0;
	
	u16 ADCValue;
	float voltage;
	
	delay_init();
	Adc_Init();
	KEY_Init();
	GPIO_InitStructReadtempCmd();
	lcd_system_reset();
	LED1=1;
	RELAY=0;
	BUZZER = 0;
	delay_ms(20);
	LCD1602_Show_Str(0, 1, "Sleep");
	while(1)
	{
		key_val = KEY_Scan(0);
		keyval_deal(key_val);
		
		ADCValue=Get_Adc_Average(ADC_Channel_1,10);
		voltage=((float)ADCValue/4096.0f)*5.0f;//�����ѹ
		temp_val=(200*voltage);
		
		if(temp_val < temp_set) //40%  ��ֵ
		{
			LED1 = 0;		//��ˮ���㹻���ر�ˮ��
		} else
			LED1 = 1;
		lcd_display();
//		display1();

	}
}

void keyval_deal(unsigned char val)
{
	switch (val) {
		//������˯��
		case 1:	
			mode_flag++;
			if(mode_flag > 1)
				mode_flag = 0;
			LCD1602_write_data(0x01);  //����
			break;
		//����ģʽ
		case 2:
			set_flag++;
			if(set_flag > 1)
				set_flag = 0;
			LCD1602_write_data(0x01);  //����
			break;
		//��
		case 3:
			temp_set+=100;
			break;
		//��
		case 4:
			temp_set-=100;
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
//		TIM_SetCompare1(TIM4, 0);  //�رռ��ȹ���
		LCD1602_Show_Str(0, 0, "Sleep");
		LED1 = 1;
		BUZZER = 1;
		return 0;
	}
	//������̨

//	LCD1602_Show_Str(0, 1, "Temp:");
	//����ģʽ
	if (set_flag) {
		LCD1602_Show_Str(0, 0, "Error");
		BUZZER = 1;
//		LCD1602_Show_Str(2, 0, "                    ");
//		LCD1602_Show_Str(2, 1, "                    ");
//		sprintf(str, "%.1f", temp_set.f_data);
//		LCD1602_Show_Str(7, 1, str);
//		AT24CXX_Write(0x00, temp_set.byte, 4);  //������ֵд��Ƭ�ⴢ��
		RELAY=1;
		return 0;
	}
	RELAY=0;
	BUZZER = 0;
	//��ʾģʽ
//	sprintf(str, "%.1f", (float)(temp_val));  //��ǰ�¶�
//	LCD1602_Show_Str(2, 1, str);

//	LCD1602_Show_Str(0, 0, "Cur:");  //��ǰ����
//	sprintf(str, "%.1f", cur_val);
//	LCD1602_Show_Str(7, 0, str);
	dis_vol1[8]=temp_val/100+0x30;
	dis_vol1[9]=temp_val%100/10+0x30;
	dis_vol1[10]=temp_val%10+0x30;

	dis_vol2[8]=temp_set/100+0x30;
	dis_vol2[9]=temp_set%100/10+0x30;
	dis_vol2[10]=temp_set%10+0x30;
	LCD1602_Show_Str(0, 0, dis_vol1);
	LCD1602_Show_Str(0, 1, dis_vol2);
	return 1;
}


void display2()
{	

}