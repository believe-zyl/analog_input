// #include "stm32f10x.h"
#include "usart.h"
#include "timer.h"
#include "modbus.h"


u8 RS485_RX_BUFF[2048];
u16 RS485_RX_CNT;


void uart_485_init(u32 RS485_BAUDRATE, u8 RS485_PARITY)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);     //默认高电平
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);      //默认接收状态
 
    USART_DeInit(USART2);                   //复位串口2
	USART_InitStructure.USART_BaudRate = RS485_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    switch(RS485_PARITY) {
        case 0:
            USART_InitStructure.USART_Parity = USART_Parity_No;
            break;      //无校验
        case 1:
            USART_InitStructure.USART_Parity = USART_Parity_Odd;
            break;      //奇校验
        case 2:
            USART_InitStructure.USART_Parity = USART_Parity_Even;
            break;      //偶校验
    }
    USART_Init(USART2, &USART_InitStructure);

	//USART1 NVIC配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//开启中断
   
    USART_Cmd(USART2, ENABLE);							//使能串口 
	RS485_TX_EN = 0;

    timer4_init();
    modbus_reg_map();
}


void USART2_IRQHandler(void)
{
	u8 res;
    u8 err;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {	
		if(USART_GetFlagStatus(USART2, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))	//检测到噪音、帧错误或校验错误
			err = 1;
        else
			err = 0;

        res = USART_ReceiveData(USART2);	//读接收到的字节，同时相关标志自动清除

        if((RS485_RX_CNT < 2047) && (err == 0)) {
            RS485_RX_BUFF[RS485_RX_CNT] = res;
            RS485_RX_CNT++;
                        
            TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	//清除定时器溢出中断
            TIM_SetCounter(TIM4, 0);					//当接收到一个新的字节，将定时器4复位为0，重新计时（相当于喂狗）
            TIM_Cmd(TIM4, ENABLE);						//开始计时
        }
	} 
}


void RS485_send_data(u8 *buff, u8 len)
{ 
    RS485_TX_EN = 1;        //切换为发送模式
    while(len--) {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);    //等待发送区为空
        USART_SendData(USART2, *(buff++));
    }
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);         //等待发送完成
}



