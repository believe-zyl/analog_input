#ifndef __INPUT_H
#define __INPUT_H	
#include "sys.h" 


#define ADS8688_CS      PAout(15)
#define ADS8688_SCK     PBout(3)
#define ADS8688_RST     PBout(6)
#define ADS8688_MOSI    PBout(5)
#define ADS8688_MISO    PBin(4)


//command register map
//only care about high 8 bits
#define NO_OP		0x0000 
#define STDBY		0x8200 
#define PWR_DN  	0x8300 
#define RST			0x8500 
#define AUTO_RST	0xA000 
#define MAN_Ch_0	0xC000 
#define MAN_CH_1	0xC400 
#define MAN_Ch_2	0xC800 
#define MAN_Ch_3	0xCC00 
#define MAN_Ch_4	0xD000 
#define MAN_Ch_5	0xD400 
#define MAN_Ch_6	0xD800 
#define MAN_Ch_7	0xDC00 
#define MAN_AUX		0xE000



//program register map
//format : 15~9     8       7~0
//         addr     W/R     DATA
//register bit15 ~ 9
#define AUTO_SEQ_EN				    0x01
#define Channel_Power_Down		    0x02 

#define Feature_Select			    0x03

#define Channel_0_Input_Range		0x05  
#define Channel_1_Input_Range		0x06 
#define Channel_2_Input_Range		0x07 
#define Channel_3_Input_Range		0x08 
#define Channel_4_Input_Range		0x09 
#define Channel_5_Input_Range		0x0A 
#define Channel_6_Input_Range		0x0B 
#define Channel_7_Input_Range		0x0C

#define Ch_0_Hysteresis				0x15 
#define Ch_0_High_Threshold_MSB		0x16 
#define Ch_0_High_Threshold_LSB		0x17 
#define Ch_0_Low_Threshold_MSB		0x18 
#define Ch_0_Low_Threshold_LSB		0x19 
 
#define Ch_7_Hysteresis				0x38 
#define Ch_7_High_Threshold_MSB		0x39 
#define Ch_7_High_Threshold_LSB		0x3A 
#define Ch_7_Low_Threshold_MSB		0x3B 
#define Ch_7_Low_Threshold_LSB		0x3C 
 
#define Command_Read_Back		    0x3F

//register bit8
#define WRITE		1
#define READ 		0
 
//register AUTO_SEQ_EN bit7 ~ 0
//default value is FFh(bit7~0 all selected)
#define CH7_EN      0x80
#define CH6_EN      0x40
#define CH5_EN      0x20
#define CH4_EN      0x10
#define CH3_EN      0x08
#define CH2_EN      0x04
#define CH1_EN      0x02
#define CH0_EN      0x01 

//register Channel_Power_Down bit7 ~ 0
//default value is 00h(bit7~0 all powered up)
#define CH7_PD      0x80
#define CH6_PD      0x40
#define CH5_PD      0x20
#define CH4_PD      0x10
#define CH3_PD      0x08
#define CH2_PD      0x04
#define CH1_PD      0x02
#define CH0_PD      0x01

//register Channel_x_Input_Range bit7 ~ 3 is 00000
//register Channel_x_Input_Range bit2 ~ 0
#define VREF_B_25           0x00	//channel range：±2.5×VREF = ±10.24V
#define VREF_B_125		    0x01	//channel range：±1.25×VREF = ±5.12V
#define VREF_B_0625  		0x02	//channel range：±0.625×VREF = ±2.56V
#define VREF_U_0_25			0x05	//channel range：0~2.5×VREF = 0~10.24V
#define VREF_U_0_125		0x06	//channel range：0~1.25×VREF = 0~5.12V

 
 
extern u16 adc_real[8];



/*===============================================================
函数名称：SPI_GPIO_Init
函数功能：模拟SPI端口初始化
输入参数：无
返 回 值：无
================================================================*/
void SPI_GPIO_Init(void);

/*===============================================================
函数名称：SPI_ReadWriteByte
函数功能：模拟SPI读写8位数据，上升沿写数据，下降沿读数据
输入参数：Tx_Data：写入的数据，
		  MISO：ADC输出数据的指针
返 回 值：无
================================================================*/
void SPI_ReadWriteByte(u8 Tx_Data, u8 *MISO);

/*===============================================================
函数名称：ADS8688_WriteCmdReg
函数功能：写ADS8688命令寄存器
输入参数：cmd：写入的命令
返 回 值：无
================================================================*/
void ADS8688_WriteCmdReg(u16 cmd);

/*===============================================================
函数名称：Enter_AUTO_RST_Mode
函数功能：进入AUTO_RST模式
输入参数：无
返 回 值：无
================================================================*/
void Enter_AUTO_RST_Mode(void);

/*===============================================================
函数名称：ADS8688_WriteProgramRegister
函数功能：向程序寄存器中写入地址和数据
输入参数：Addr：地址位：BIT[15:9]、读写位：BIT8，
	     data：数据位：BIT[7:0]
返 回 值：无
================================================================*/
void ADS8688_WriteProgramRegister(u8 Addr, u8 data);

/*===============================================================
函数名称：ADS8688_ReadProgramRegister
函数功能：读取程序寄存器中的数据
输入参数：Addr：地址位：BIT[15:9]、读写位：BIT8
返 回 值：data：读取的8位数据
================================================================*/
void ADS8688_ReadProgramRegister(u8 Addr);

/*===============================================================
函数名称：Set_Auto_Scan_Sequence
函数功能：设置自动扫描通道顺序
输入参数：seq：所选的扫描通道
返 回 值：无
================================================================*/
void Set_Auto_Scan_Sequence(u8 seq);

/*===============================================================
函数名称：Set_CH_Range_Select
函数功能：设置通道输入范围
输入参数：ch(通道地址)，可选参数：Channel_n_Input_Range,n=0-7
		  range(输入范围)，可选参数：VREF_B_25、VREF_B_125、VREF_B_0625、VREF_U_0_25、VREF_U_0_125	
返 回 值：无                         	
==================================== ============================*/
void Set_CH_Range_Select(u8 ch, u8 range);

/*===============================================================
函数名称：Set_Channel_Power_Down
函数功能：设置通道输入范围
输入参数：pwd：0x00(通道全部上电)，0xFF(通道全部掉电)
返 回 值：无                         	
==================================== ============================*/
void Set_Channel_Power_Down(u8 pwd);

/*===============================================================
函数名称：ADS8688_Init
函数功能：ADS8688初始化
输入参数：无
返 回 值：无
================================================================*/
void ADS8688_Init(void);

/*===============================================================
函数名称：Get_AUTO_RST_Mode_Data
函数功能：读取AUTO_RST模式下的输出数据
输入参数：chn：总通道号
返 回 值：无
================================================================*/
void Get_AUTO_RST_Mode_Data(u8 chn);

/**
 * @description: convert the adc data to mv
 * @param      :{u16} x ：which data need convert
 * @param      :{u8} AI_RANGE ：analog input range
 * @return     :{short} y : mv
 */
short adc_data_convert(u16 x, u8 AI_RANGE);

/**
 * @description: adc convert data put into adc_real[]
 * @param      :{u16} *adc_real : transmit adc_real[] first address
 * @param      :{u8} AI_RANGE ：analog input range
 * @return     :{*}
 */
void get_real_adc_data(u16 *adc_real, u8 AI_RANGE);

/**
 * @description: get auto scan channel data mv
 * @param      :{u8} chn : transmit channel count
 * @param      :{u16} *adc_real : transmit adc_real[] first address
 * @param      :{u8} AI_RANGE ：analog input range
 * @return     :{*}
 */
void get_ADS_all(u8 chn, u16 *adc_real, u8 AI_RANGE);


#endif

