/**
  ******************************************************************************
  * @file    bsp_dht11.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��ʪ�ȴ�����Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 ָ����  ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */




#include "bsp_dht11.h"




static void                           DHT11_GPIO_Config                       ( void );
static void                           DHT11_Mode_IPU                          ( void );
static void                           DHT11_Mode_Out_PP                       ( void );
static uint8_t                        DHT11_ReadByte                          ( void );



 /**
  * @brief  DHT11 ��ʼ������
  * @param  ��
  * @retval ��
  */
void DHT11_Init ( void )
{
	DHT11_GPIO_Config ();
	
	DHT11_Dout_1;               // ����GPIOC15
}
//ʹ��ˮ�μ�ʱ����׼������ ���ô��Լ�ʱ
void dht11_delay_us(int32_t time){
   while(time--){   
   }
}

void dht11_delay_ms(int32_t time){   
 
   uint32_t i = 0;
	 i = time*1400;
   while(i--){
	 }
}

/*
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 */
static void DHT11_GPIO_Config ( void )
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure; 

	
	/*����DHT11_Dout_GPIO_PORT������ʱ��*/
  DHT11_Dout_SCK_APBxClock_FUN ( DHT11_Dout_GPIO_CLK, ENABLE );	
 
	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
  	GPIO_Init ( DHT11_Dout_GPIO_PORT, &GPIO_InitStructure );		  
	
}


/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/	
	  GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;

	   /*��������ģʽΪ��������ģʽ*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
		/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	  /*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
	  GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 
	
}


/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
  	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	 
	
}


/* 
 * ��DHT11��ȡһ���ֽ�
 */
static uint8_t DHT11_ReadByte ( void )
{
	uint8_t i, temp=0;
	uint8_t try_num = 0;

	for(i=0;i<8;i++)    
	{	 
		temp <<= 1;
		//�ȴ��ӻ�����,��ʾ1Bit���ݿ�ʼ����
	while((DHT11_Dout_IN()==Bit_SET)&& try_num<100 ){
		try_num++;
		dht11_delay_us(1);
	}
	
	//�ȴ��ӻ����ߣ���ʼ1Bit����ֵ
	try_num = 0;
	while((DHT11_Dout_IN()==Bit_RESET)&& try_num<100){
		try_num++;
		dht11_delay_us(1);
	}

	dht11_delay_us(30); //�ߵ�ƽ����28us��ʾ1
	if((DHT11_Dout_IN()==Bit_SET))
		{
			temp |= 1;
	} else {
		temp |= 0;
	}
	
	}
	
	return temp;
	
}


/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{  
	/*���ģʽ*/
	DHT11_Mode_Out_PP();
	/*��������*/
	//printf("\r\n computer high \r\n");
	DHT11_Dout_0;
	/*��ʱ20ms*/
//	Delay_ms(18);
	dht11_delay_ms(18);
	//printf("\r\n computer low \r\n");
	/*�������� ������ʱ18us*/
	DHT11_Dout_1; 

	//Delay_us(30);   //��ʱ20us
	dht11_delay_us(35);
//printf("\r\n accept \r\n");
	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
	DHT11_Mode_IPU();

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(DHT11_Dout_IN()==Bit_RESET)     
	{
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(DHT11_Dout_IN()==Bit_RESET);

		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(DHT11_Dout_IN()==Bit_SET);
		
		/*��ʼ��������*/   
		DHT11_Data->humi_int= DHT11_ReadByte();

		DHT11_Data->humi_deci= DHT11_ReadByte();

		DHT11_Data->temp_int= DHT11_ReadByte();

		DHT11_Data->temp_deci= DHT11_ReadByte();

		DHT11_Data->check_sum= DHT11_ReadByte();
		//printf("\r\nreceive\n");

		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		DHT11_Mode_Out_PP();
		/*��������*/
		DHT11_Dout_1;

		/*����ȡ�������Ƿ���ȷ*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return SUCCESS;
		else 
			return ERROR;
	}
	
	else
		return ERROR;
	
}

	  


/*************************************END OF FILE******************************/
