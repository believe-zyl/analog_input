#include "stm32f10x.h"
#include "timer.h"
#include "buzzer.h"
#include "led.h"
#include "usart.h"


u16 RS485_FRAME_DISTANCE = 4;	//����֡��С���(ms),������ʱ������Ϊ����һ֡
u8 RS485_FRAME_FLAG = 0;		//֡�������



void timer2_init(u16 arr2, u16 psc2)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr2 * 10 - 1; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ��������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc2 * 100 - 1; 	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  		//�����ȼ�5��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  			//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	// TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	//ʹ�ܻ���ʧ��ָ����TIM2�ж�Դ

	TIM_Cmd(TIM2, ENABLE);  					//ʹ��TIM2����

	buzzer_init();
}


void timer3_init(u16 arr3, u16 psc3)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr3 * 10 - 1; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ��������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc3 * 100 - 1; 	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  			//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 12;  		//�����ȼ�12��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  			//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//ʹ�ܻ���ʧ��ָ����TIM3 �ж�Դ

	TIM_Cmd(TIM3, ENABLE);  					//ʹ��TIM3����

	led_init();
}


void timer4_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = RS485_FRAME_DISTANCE * 10;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ��������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; 			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 		//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  		//�����ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  			//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	//ʹ�ܻ���ʧ��ָ����TIM4 �ж�Դ
	
	// TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	// TIM_Cmd(TIM4, ENABLE);  					//ʹ��TIM4����
}


void TIM2_IRQHandler(void)  							
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) { //���ָ����TIM�жϷ������:TIM �ж�Դ 
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  	//���TIMx���жϴ�����λ:TIM �ж�Դ 
		buzzer1_work();
	}
}


void TIM3_IRQHandler(void)  							
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) { //���ָ����TIM�жϷ������:TIM �ж�Դ 
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  	//���TIMx���жϴ�����λ:TIM �ж�Դ 
		led_flash();
	}
}


void TIM4_IRQHandler(void)
{                                                                   
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	//����жϱ�־
        TIM_Cmd(TIM4, DISABLE);						//ֹͣ��ʱ��
        // RS485_TX_EN = 1;							//ֹͣ���գ��л�Ϊ����״̬
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
        RS485_FRAME_FLAG = 1;						//��λ֡�������
    }
}


