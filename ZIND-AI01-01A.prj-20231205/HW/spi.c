#include "stm32f10x.h"
#include "spi.h"


// void spi1_init(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	SPI_InitTypeDef  SPI_InitStructure;
  
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//AFIO push-pull output
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

// 	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
// 	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//����SPI����ģʽ:����Ϊ��SPI
// 	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
// 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
// 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//���ݲ����ڵڶ���ʱ����
// 	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
// 	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
// 	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
// 	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	
// 	SPI_Init(SPI1, &SPI_InitStructure);			//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
// 	SPI_Cmd(SPI1, ENABLE);		//ʹ��SPI����
	
// 	spi1_read_write_byte(0xff);	//�������� 
// }

 
void spi2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//AFIO push-pull output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	
	SPI_Init(SPI2, &SPI_InitStructure);			//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE);		//ʹ��SPI����
	
	spi2_read_write_byte(0xff);	//�������� 
}


// u8 spi1_read_write_byte(u8 TxData1)
// {					 	
// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);		//���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  TXEΪ1����д����			  
// 	SPI_I2S_SendData(SPI1, TxData1);									//ͨ������SPIx����һ������

// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);		//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ   RXNEΪ1���Զ�����	  						    
// 	return SPI_I2S_ReceiveData(SPI1);									//����ͨ��SPIx������յ�����					    
// }


u8 spi2_read_write_byte(u8 TxData2)
{					 	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);		//���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  TXEΪ1����д����			  
	SPI_I2S_SendData(SPI2, TxData2);									//ͨ������SPIx����һ������

	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);		//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ   RXNEΪ1���Զ�����	  						    
	return SPI_I2S_ReceiveData(SPI2);									//����ͨ��SPIx������յ�����					    
}



