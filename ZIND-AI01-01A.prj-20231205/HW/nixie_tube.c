#include "stm32f10x.h"
#include "nixie_tube.h"
#include "spi.h"


void nixie_tube_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			//SPI CS
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	spi2_init();										//初始化SPI2		 
}

//以下为常规段码 
// static u8 code_table[] = {
// 	//0   1    2    3    4    5    6    7//
// 	0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
// 	//8   9    A    b    C    d    E    F// 
// 	0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,
// 	//P   U    y    L   全亮  全灭//
// 	0x73,0x3e,0x6e,0x38,0xff,0x00
// };

static u8 code_table[] = {
	//0   1    2    3    4    5    6    7//
	0x7e,0x24,0x57,0x67,0x2d,0x6b,0x7b,0x64,
	//8   9    A    b    C    d    E    F//
	0x7f,0x6f,0x7d,0x3b,0x5a,0x37,0x5b,0x59,
	//P   U    y    L   全亮  全灭//
	0x5d,0x3e,0x2f,0x1a,0xff,0x00
};


void nixie_tube_show(u8 n)
{
	SPI2_CS_LOW;

	NIXIE_TUBE1_DISENABLE;
	NIXIE_TUBE2_DISENABLE;

	spi2_read_write_byte(code_table[n]);

	NIXIE_TUBE1_ENABLE;
	NIXIE_TUBE2_ENABLE;

	SPI2_CS_HIGH;
	SPI2_CS_LOW;
}

