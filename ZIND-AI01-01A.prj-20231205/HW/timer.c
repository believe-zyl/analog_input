#include "stm32f10x.h"
#include "timer.h"
#include "buzzer.h"
#include "led.h"
#include "usart.h"


u16 RS485_FRAME_DISTANCE = 4;	//数据帧最小间隔(ms),超过此时间则认为是下一帧
u8 RS485_FRAME_FLAG = 0;		//帧结束标记



void timer2_init(u16 arr2, u16 psc2)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr2 * 10 - 1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值，计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc2 * 100 - 1; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  		//从优先级5级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  			//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	// TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	//使能或者失能指定的TIM2中断源

	TIM_Cmd(TIM2, ENABLE);  					//使能TIM2外设

	buzzer_init();
}


void timer3_init(u16 arr3, u16 psc3)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr3 * 10 - 1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值，计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc3 * 100 - 1; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  			//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 12;  		//从优先级12级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  			//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//使能或者失能指定的TIM3 中断源

	TIM_Cmd(TIM3, ENABLE);  					//使能TIM3外设

	led_init();
}


void timer4_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//时钟使能

	TIM_TimeBaseStructure.TIM_Period = RS485_FRAME_DISTANCE * 10;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值，计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; 			//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 		//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  			//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  		//从优先级4级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  			//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	//使能或者失能指定的TIM4 中断源
	
	// TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	// TIM_Cmd(TIM4, ENABLE);  					//使能TIM4外设
}


void TIM2_IRQHandler(void)  							
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) { //检查指定的TIM中断发生与否:TIM 中断源 
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  	//清除TIMx的中断待处理位:TIM 中断源 
		buzzer1_work();
	}
}


void TIM3_IRQHandler(void)  							
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) { //检查指定的TIM中断发生与否:TIM 中断源 
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  	//清除TIMx的中断待处理位:TIM 中断源 
		led_flash();
	}
}


void TIM4_IRQHandler(void)
{                                                                   
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	//清除中断标志
        TIM_Cmd(TIM4, DISABLE);						//停止定时器
        // RS485_TX_EN = 1;							//停止接收，切换为发送状态
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
        RS485_FRAME_FLAG = 1;						//置位帧结束标记
    }
}


