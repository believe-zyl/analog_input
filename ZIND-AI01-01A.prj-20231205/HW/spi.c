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

// 	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
// 	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//设置SPI工作模式:设置为主SPI
// 	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//设置SPI的数据大小:SPI发送接收8位帧结构
// 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//选择了串行时钟的稳态:时钟悬空高
// 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//数据捕获于第二个时钟沿
// 	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
// 	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//定义波特率预分频的值:波特率预分频值为16
// 	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
// 	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC值计算的多项式
	
// 	SPI_Init(SPI1, &SPI_InitStructure);			//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
// 	SPI_Cmd(SPI1, ENABLE);		//使能SPI外设
	
// 	spi1_read_write_byte(0xff);	//启动传输 
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


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC值计算的多项式
	
	SPI_Init(SPI2, &SPI_InitStructure);			//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE);		//使能SPI外设
	
	spi2_read_write_byte(0xff);	//启动传输 
}


// u8 spi1_read_write_byte(u8 TxData1)
// {					 	
// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);		//检查指定的SPI标志位设置与否:发送缓存空标志位	  TXE为1可以写数据			  
// 	SPI_I2S_SendData(SPI1, TxData1);									//通过外设SPIx发送一个数据

// 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);		//检查指定的SPI标志位设置与否:接受缓存非空标志位   RXNE为1可以读数据	  						    
// 	return SPI_I2S_ReceiveData(SPI1);									//返回通过SPIx最近接收的数据					    
// }


u8 spi2_read_write_byte(u8 TxData2)
{					 	
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);		//检查指定的SPI标志位设置与否:发送缓存空标志位	  TXE为1可以写数据			  
	SPI_I2S_SendData(SPI2, TxData2);									//通过外设SPIx发送一个数据

	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);		//检查指定的SPI标志位设置与否:接受缓存非空标志位   RXNE为1可以读数据	  						    
	return SPI_I2S_ReceiveData(SPI2);									//返回通过SPIx最近接收的数据					    
}




