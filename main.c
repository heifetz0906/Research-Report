/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��дEEPROM
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./i2c/i2c.h"
#include "./mpu6050/mpu6050.h"


//�����Ƿ�ʹ��LCD������ʾ������Ҫ�Ļ��������ע�͵�����
#define USE_LCD_DISPLAY

#ifdef USE_LCD_DISPLAY
 #include "./lcd/bsp_lcd.h"
#endif

/*���������*/
uint32_t Task_Delay[NumOfTask]={0};

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	
	short Acel[3];
	short Gyro[3];
	float Temp;
	
	SysTick_Init();
	
  LED_GPIO_Config();
	
	
#ifdef USE_LCD_DISPLAY	
		 /*��ʼ��Һ����*/
		LCD_Init();
		LCD_LayerInit();
		LTDC_Cmd(ENABLE);
		
		/*�ѱ�����ˢ��ɫ*/
		LCD_SetLayer(LCD_BACKGROUND_LAYER);  
		LCD_Clear(LCD_COLOR_BLACK);
		
		/*��ʼ����Ĭ��ʹ��ǰ����*/
		LCD_SetLayer(LCD_FOREGROUND_LAYER); 
		/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
		LCD_SetTransparency(0xFF);
		LCD_Clear(LCD_COLOR_BLACK);
		
			/*����������ɫ������ı�����ɫ*/
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);	

		/*Ӣ������*/
		LCD_SetFont(&Font16x24); 	
		
		LCD_DisplayStringLine(LINE(1),(uint8_t* )"           This is a MPU6050 demo       ");			
		LCD_DisplayStringLine(LINE(2),(uint8_t* )"                 (Base Version)       ");		
#endif
	
	
  /*��ʼ��USART1*/
  Debug_USART_Config(); 
		
	//��ʼ�� I2C
	I2cMaster_Init(); 

	printf("\r\n ��ӭʹ�ñ���  STM32 F429 �����塣\r\n");		 

	printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");

 	 	//MPU6050��ʼ��
	MPU6050_Init();
	
	//���MPU6050
	if (MPU6050ReadID() == 1)
	{	
		while(1)
		{
			if(Task_Delay[0]==TASK_ENABLE)
			{
				LED2_TOGGLE;
				Task_Delay[0]=1000;
			}
			
			if(Task_Delay[1]==0)
			{
				MPU6050ReadAcc(Acel);
				printf("���ٶȣ�%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
				MPU6050ReadGyro(Gyro);
				printf("    ������%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
				MPU6050_ReturnTemp(&Temp);
				printf("    �¶�%8.2f\r\n",Temp);				
				
				
				#ifdef USE_LCD_DISPLAY	
					{
						char cStr [ 70 ];
						sprintf ( cStr, "Acceleration:%8d%8d%8d",Acel[0],Acel[1],Acel[2] );	//���ٶ�ԭʼ����


						LCD_DisplayStringLine(LINE(7),(uint8_t* )cStr);			

						sprintf ( cStr, "Gyro        :%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2] );	//��ԭʼ����

						LCD_DisplayStringLine(LINE(8),(uint8_t* )cStr);			

						sprintf ( cStr, "Temperture  :%8.2f",Temp );	//�¶�ֵ
						LCD_DisplayStringLine(LINE(9),(uint8_t* )cStr);			

					}
				#endif
				
				Task_Delay[1]=500; //����һ�����ݣ��ɸ����Լ���������߲���Ƶ�ʣ���100ms����һ��
				
			}

			//*************************************	��������������ĸ�ʽ************************************//
	//		if(Task_Delay[i]==0)
	//		{
	//			Task(i);
	//			Task_Delay[i]=;
	//		}

		}

	}
	else
	{
			printf("\r\nû�м�⵽MPU6050��������\r\n");
			LED_RED; 
			#ifdef USE_LCD_DISPLAY			
				/*����������ɫ������ı�����ɫ*/
				LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);	

				LCD_DisplayStringLine(LINE(4),(uint8_t* )"No MPU6050 detected! ");			//Ұ���Դ���16*24��ʾ
				LCD_DisplayStringLine(LINE(5),(uint8_t* )"Please check the hardware connection! ");			//Ұ���Դ���16*24��ʾ

			#endif
		while(1);	
	}


}




/*********************************************END OF FILE**********************/

