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
    GPIO_SetBits(GPIOA, GPIO_Pin_2);     //Ĭ�ϸߵ�ƽ
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);      //Ĭ�Ͻ���״̬
 
    USART_DeInit(USART2);                   //��λ����2
	USART_InitStructure.USART_BaudRate = RS485_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    switch(RS485_PARITY) {
        case 0:
            USART_InitStructure.USART_Parity = USART_Parity_No;
            break;      //��У��
        case 1:
            USART_InitStructure.USART_Parity = USART_Parity_Odd;
            break;      //��У��
        case 2:
            USART_InitStructure.USART_Parity = USART_Parity_Even;
            break;      //żУ��
    }
    USART_Init(USART2, &USART_InitStructure);

	//USART1 NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//�����ж�
   
    USART_Cmd(USART2, ENABLE);							//ʹ�ܴ��� 
	RS485_TX_EN = 0;

    timer4_init();
    modbus_reg_map();
}


void USART2_IRQHandler(void)
{
	u8 res;
    u8 err;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {	
		if(USART_GetFlagStatus(USART2, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))	//��⵽������֡�����У�����
			err = 1;
        else
			err = 0;

        res = USART_ReceiveData(USART2);	//�����յ����ֽڣ�ͬʱ��ر�־�Զ����

        if((RS485_RX_CNT < 2047) && (err == 0)) {
            RS485_RX_BUFF[RS485_RX_CNT] = res;
            RS485_RX_CNT++;
                        
            TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	//�����ʱ������ж�
            TIM_SetCounter(TIM4, 0);					//�����յ�һ���µ��ֽڣ�����ʱ��4��λΪ0�����¼�ʱ���൱��ι����
            TIM_Cmd(TIM4, ENABLE);						//��ʼ��ʱ
        }
	} 
}


void RS485_send_data(u8 *buff, u8 len)
{ 
    RS485_TX_EN = 1;        //�л�Ϊ����ģʽ
    while(len--) {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);    //�ȴ�������Ϊ��
        USART_SendData(USART2, *(buff++));
    }
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);         //�ȴ��������
}



