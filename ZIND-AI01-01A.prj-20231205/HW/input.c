#include "stm32f10x.h"
#include "input.h"
#include "delay.h"



u8 Rxh[];			//存储输出数据的高8位
u8 Rxl[];			//存储输出数据的低8位
u16 adc_data[8];	//存储 ADC 8个通道的输出数据
u16 adc_real[8];	//存储各通道转化成的模拟电压实际值



void SPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//ADS8688_CS->PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化端口
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);						//PA15输出低电平

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;	//ADS8688_SCK->PB3、ADS8688_MOSI->PB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化端口
	GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_5);			//PB3、PB5输出低电平
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//ADS8688_RST->PB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
    GPIO_SetBits(GPIOB, GPIO_Pin_6);						//PB6输出高电平	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//ADS8688_MISO1->PB4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz	
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化端口
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);						//PB4输入低电平
}


void SPI_ReadWriteByte(u8 Tx_Data, u8 *MISO)			
{
	u8 i = 0;
 
	for(i = 0; i < 8; i++)
	{
		ADS8688_SCK = 0;				//上升沿写入8位数据
		
		if(Tx_Data & (1 << (7 - i))) {
			ADS8688_MOSI = 1;
		}
		else {
			ADS8688_MOSI = 0;
		}
		
		ADS8688_SCK = 1;				//下降沿读取8位数据
 
        *MISO <<= 1;		
		if(ADS8688_MISO) {
			*MISO |= 1; 
		}

		ADS8688_SCK = 0;					          
	}
}

void ADS8688_WriteCmdReg(u16 cmd)
{	
	ADS8688_CS = 0;
	SPI_ReadWriteByte((cmd & 0xFF00) >> 8, Rxh);   
	SPI_ReadWriteByte(cmd & 0xFF, Rxh);
	ADS8688_CS = 1;
}


void Enter_AUTO_RST_Mode(void)
{
	ADS8688_WriteCmdReg(AUTO_RST);
}


void ADS8688_WriteProgramRegister(u8 Addr, u8 data)
{
	ADS8688_CS = 0;
	SPI_ReadWriteByte((Addr << 1) | WRITE, Rxh);
	SPI_ReadWriteByte(data, Rxh);
	ADS8688_CS = 1;
}


void ADS8688_ReadProgramRegister(u8 Addr)
{	
	ADS8688_CS = 0;
	SPI_ReadWriteByte((Addr << 1) | READ, Rxh);
	ADS8688_CS = 1;
}


void Set_Auto_Scan_Sequence(u8 seq)
{
	ADS8688_WriteProgramRegister(AUTO_SEQ_EN, seq);
}


void Set_CH_Range_Select(u8 ch, u8 range) 
{                              
	ADS8688_WriteProgramRegister(ch, range);
}


void Set_Channel_Power_Down(u8 pwd)
{                              
	ADS8688_WriteProgramRegister(Channel_Power_Down, pwd);
}


void ADS8688_Init(void)
{
	SPI_GPIO_Init();		//模拟SPI端口初始化
	
	ADS8688_RST = 0;
	delay_us(2);
	ADS8688_RST = 1;
	delay_us(2);

	Set_Auto_Scan_Sequence(0xFF);	//自动扫描通道0-7

	Set_Channel_Power_Down(0x00);	//通道0-7上电

	Set_CH_Range_Select(Channel_0_Input_Range, VREF_U_0_25);	//设置通道0输入范围为：0 ~ 2.5×VREF
	Set_CH_Range_Select(Channel_1_Input_Range, VREF_U_0_25);
	Set_CH_Range_Select(Channel_2_Input_Range, VREF_U_0_25);
	Set_CH_Range_Select(Channel_3_Input_Range, VREF_U_0_25);
	Set_CH_Range_Select(Channel_4_Input_Range, VREF_U_0_25);
	Set_CH_Range_Select(Channel_5_Input_Range, VREF_U_0_25);
	Set_CH_Range_Select(Channel_6_Input_Range, VREF_U_0_25);
	Set_CH_Range_Select(Channel_7_Input_Range, VREF_U_0_25);

	Enter_AUTO_RST_Mode();			//进入自动扫描通道模式(具有复位功能)
}


void Get_AUTO_RST_Mode_Data(u8 chn)
{	
	u16 i = 0;
 
	for(i=0; i<chn; i++)
	{
		ADS8688_CS = 0;
		SPI_ReadWriteByte(0x00, Rxh);		//先写后读
		SPI_ReadWriteByte(0x00, Rxl);
		
		SPI_ReadWriteByte(0xFF, Rxh);
		SPI_ReadWriteByte(0xFF, Rxl);
		ADS8688_CS = 1;
	
		adc_data[i] = (Rxh[0] << 8) | Rxl[0];
	}
}


// double adc_data_convert(u16 x)
// {
// 	double y;
// 	// y = (s16)(((double)(x - 32768) * 20.48 / (double)65535) * 1000);	//通道范围±10.24V
// 	// y = (u16)(((double)(x - 32768) * 10.24 / (double)65535) * 1000);	//通道范围±5.12V
// 	// y = (u16)(((double)(x - 32768) * 5.12 / (double)65535) * 1000);	//通道范围±2.56V
// 	y = (u16)(((double)x * 10.24 / (double)65535) * 1000);		//通道范围0~10.24V
// 	// y = (u16)(((double)x * 5.12 / (double)65535) * 1000);	//通道范围0~5.12V

// 	return y;
// }

// short adc_data_convert(u16 x)
// {
// 	short y;
// 	// y = (x - 0x8000) * 3125 / 10000;		//通道范围±10.24V
// 	// y = (u16)(((double)(x - 32768) * 10.24 / (double)65535) * 1000);	//通道范围±5.12V
// 	// y = (u16)(((double)(x - 32768) * 5.12 / (double)65535) * 1000);	//通道范围±2.56V
// 	y = x * 15625 / 100000;		//通道范围0~10.24V
// 	// y = x * 78125 / 1000000;	//通道范围0~5.12V

// 	return y;
// }

short adc_data_convert(u16 x, u8 AI_RANGE)
{
	short y;
	switch(AI_RANGE) {
        case 0:
            y = x * 15625 / 100000;		//通道范围0~10.24V
            break;
        case 1:
            y = x * 78125 / 1000000;	//通道范围0~5.12V
            break;
        case 2:
            y = (x - 0x8000) * 3125 / 10000;	//通道范围±10.24V
            break;
		case 3:
            y = (u16)(((double)(x - 32768) * 10.24 / (double)65535) * 1000);	//通道范围±5.12V
            break;
		case 4:
            y = (u16)(((double)(x - 32768) * 5.12 / (double)65535) * 1000);		//通道范围±2.56V
            break;
    }
	return y;
}


void get_real_adc_data(u16 *adc_real, u8 AI_RANGE)
{
	adc_real[0] = (u16)adc_data_convert(adc_data[0], AI_RANGE);
	adc_real[1] = (u16)adc_data_convert(adc_data[1], AI_RANGE);
	adc_real[2] = (u16)adc_data_convert(adc_data[2], AI_RANGE);
	adc_real[3] = (u16)adc_data_convert(adc_data[3], AI_RANGE);
	adc_real[4] = (u16)adc_data_convert(adc_data[4], AI_RANGE);
	adc_real[5] = (u16)adc_data_convert(adc_data[5], AI_RANGE);
	adc_real[6] = (u16)adc_data_convert(adc_data[6], AI_RANGE);
	adc_real[7] = (u16)adc_data_convert(adc_data[7], AI_RANGE);
}


void get_ADS_all(u8 chn, u16 *adc_real, u8 AI_RANGE)
{
	Get_AUTO_RST_Mode_Data(chn);
	get_real_adc_data(adc_real, AI_RANGE);
}

 

