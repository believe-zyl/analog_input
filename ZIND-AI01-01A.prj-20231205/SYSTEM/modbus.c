#include "stm32f10x.h"
#include "modbus.h"
#include "usart.h"
#include "input.h"
#include "address.h"


// u8 RS485_Addr = 1;                //从机地址

extern u8 RS485_FRAME_FLAG;       //帧结束标记
extern u8 RS485_RX_BUFF[2048];    //接收缓冲区2048字节
extern u16 RS485_RX_CNT;          //接收计数器
u8 RS485_TX_BUFF[2048];           //发送缓冲区

u16 startRegAddr;       //寄存器起始地址
u16 RegNum;             //寄存器数量
u16 calCRC;             //所接收数据的CRC

u32 *Modbus_InputIO[100];       //输入开关量寄存器指针
u32 *Modbus_OutputIO[100];      //输出开关量寄存器指针
u16 *Modbus_HoldReg[1000];      //保持寄存器指针



void modbus_reg_map(void)
{
    //输入开关量寄存器指针指向
    Modbus_InputIO[0] = (u32*) & PCin(4);      //KEY0
    Modbus_InputIO[1] = (u32*) & PCin(5);      //KEY1

    //输出开关量寄存器指针指向
    Modbus_OutputIO[0] = (u32*) & PBout(6);    //RELAY0
    Modbus_OutputIO[1] = (u32*) & PBout(7);    //RELAY1
    Modbus_OutputIO[2] = (u32*) & PBout(8);    //RELAY2
    Modbus_OutputIO[3] = (u32*) & PBout(9);    //RELAY3

    //保持寄存器指针指向
    Modbus_HoldReg[0] = (u16*)&adc_real[0];
    Modbus_HoldReg[1] = (u16*)&adc_real[1];
    Modbus_HoldReg[2] = (u16*)&adc_real[2];
    Modbus_HoldReg[3] = (u16*)&adc_real[3];
    Modbus_HoldReg[4] = (u16*)&adc_real[4];
    Modbus_HoldReg[5] = (u16*)&adc_real[5];
    Modbus_HoldReg[6] = (u16*)&adc_real[6];
    Modbus_HoldReg[7] = (u16*)&adc_real[7];
}


u16 crc_compute(u8 *ptr,u8 len)
{
	u8 i,j,temp;
	u16 crc = 0xFFFF;	
	for(i = 0; i < len; i++) {
		crc = crc ^ (*ptr);
		for(j = 0; j < 8; j++) {
			temp = crc & 0x0001;
			crc = crc >> 1;
			if(temp)
				crc = crc ^ 0xA001;
		}
		ptr++;
	}
    return(((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8));	//返回高低字节转换后的crc值
}


void RS485_service(void)
{
    u16 recCRC;
    if(RS485_FRAME_FLAG == 1) {
        if(RS485_RX_BUFF[0] == module_addr()) {      //地址正确
            if((RS485_RX_BUFF[1] == 01) || (RS485_RX_BUFF[1] == 02) || (RS485_RX_BUFF[1] == 03) || (RS485_RX_BUFF[1] == 04) || (RS485_RX_BUFF[1] == 05) || (RS485_RX_BUFF[1] == 06) || (RS485_RX_BUFF[1] == 15) || (RS485_RX_BUFF[1] == 16)) {  //功能码正确
                startRegAddr = (((u16)RS485_RX_BUFF[2]) << 8) | RS485_RX_BUFF[3];   //获取寄存器起始地址
                if(startRegAddr < 1000) {     //寄存器地址在范围内       
                    calCRC = crc_compute(RS485_RX_BUFF, RS485_RX_CNT - 2);   //计算所接收数据的CRC
                    recCRC = RS485_RX_BUFF[RS485_RX_CNT - 1] | (((u16)RS485_RX_BUFF[RS485_RX_CNT - 2]) << 8);   //接收到的CRC
                    if(calCRC == recCRC) {          //CRC校验正确
                        switch(RS485_RX_BUFF[1]) {  //根据不同的功能码进行处理
                            case 01: {       //读输出开关量
                                modbus_01_solve();
                                break;
                            }
                            case 02: {       //读输入开关量
                                modbus_02_solve();
                                break;
                            }                            
                            case 03: {       //读多个保持寄存器
                                modbus_03_solve();
                                break;
                            }
                            // case 04: {       //读多个输入寄存器
                            //     modbus_04_solve();
                            //     break;
                            // }
                            // case 05: {       //写单个输出开关量
                            //     modbus_05_solve();
                            //     break;
                            // }
                            // case 06: {       //写单个保持寄存器
                            //     modbus_06_solve();
                            //     break;
                            // }
                            // case 15: {       //写多个输出开关量
                            //     modbus_15_solve();
                            //     break;
                            // }
                            // case 16: {       //写多个保持寄存器
                            //     modbus_16_solve();
                            //     break;
                            // }
                        }
                    } else {     //CRC校验错误
                        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                        RS485_TX_BUFF[2] = 0x04;        //异常码
                        RS485_send_data(RS485_TX_BUFF, 3);
                    }        
                } else {     //寄存器地址超出范围
                    RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                    RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                    RS485_TX_BUFF[2] = 0x02;        //异常码
                    RS485_send_data(RS485_TX_BUFF,3);
                }                                                
            } else {     //功能码错误
                RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                RS485_TX_BUFF[2] = 0x01;        //异常码
                RS485_send_data(RS485_TX_BUFF, 3);
            }
        }

        RS485_FRAME_FLAG = 0;   //复位帧结束标志
        RS485_RX_CNT = 0;       //接收计数器清零
        RS485_TX_EN = 0;      //开启接收模式
    }                
}


void modbus_01_solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];	//获取寄存器数量
    if((startRegAddr + RegNum) < 100) {		//寄存器地址+数量在范围内
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        ByteNum = RegNum / 8;	    //字节数
        if(RegNum % 8)
			ByteNum += 1;		    //如果位数还有余数，则字节数+1
        RS485_TX_BUFF[2] = ByteNum;	//返回要读取的字节数
        for(i = 0; i < RegNum; i++) {
            if(i % 8 == 0) 
				RS485_TX_BUFF[3 + i / 8] = 0x00;
            RS485_TX_BUFF[3 + i / 8] >>= 1;		//低位先发送
            RS485_TX_BUFF[3 + i / 8] |= ((*Modbus_OutputIO[startRegAddr + i]) << 7) & 0x80;
            if(i == RegNum - 1)	{	//发送到最后一个位了
                if(RegNum % 8)
					RS485_TX_BUFF[3 + i / 8] >>= 8 - (RegNum % 8);	//如果最后一个字节还有余数，则剩余MSB填充0
            }
        }
        calCRC = crc_compute(RS485_TX_BUFF, ByteNum + 3);
        RS485_TX_BUFF[ByteNum + 3] = (calCRC >> 8) & 0xFF;
        RS485_TX_BUFF[ByteNum + 4] = (calCRC) & 0xFF;
        RS485_send_data(RS485_TX_BUFF, ByteNum + 5);
    } else {		//寄存器地址+数量超出范围
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;			//异常码
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


void modbus_02_solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];	//获取寄存器数量
    if((startRegAddr + RegNum) < 100) {		//寄存器地址+数量在范围内
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        ByteNum = RegNum / 8;		//字节数
        if(RegNum % 8) 
			ByteNum += 1;			//如果位数还有余数，则字节数+1
        RS485_TX_BUFF[2] = ByteNum;	//返回要读取的字节数
        for(i = 0; i < RegNum; i++) {
            if(i % 8 == 0) 
				RS485_TX_BUFF[3 + i / 8] = 0x00;
        	RS485_TX_BUFF[3 + i / 8] >>= 1;	//低位先发送
            RS485_TX_BUFF[3 + i / 8] |= ((*Modbus_InputIO[startRegAddr + i]) << 7) & 0x80;
            if(i == RegNum - 1)	{	//发送到最后一个位了
                if(RegNum % 8) 
					RS485_TX_BUFF[3 + i / 8] >>= 8 - (RegNum % 8);	//如果最后一个字节还有余数，则剩余MSB填充0
            }
        }
        calCRC = crc_compute(RS485_TX_BUFF, ByteNum + 3);
        RS485_TX_BUFF[ByteNum + 3] = (calCRC >> 8) & 0xFF;
        RS485_TX_BUFF[ByteNum + 4] = (calCRC) & 0xFF;
        RS485_send_data(RS485_TX_BUFF, ByteNum + 5);
    } else	{	//寄存器地址+数量超出范围
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;		//异常码
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


void modbus_03_solve(void)
{
    u8 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];	//获取寄存器数量
    if((startRegAddr + RegNum) < 1000) {		//寄存器地址+数量在范围内
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        RS485_TX_BUFF[2] = RegNum * 2;
        for(i = 0 ; i < RegNum; i++) {
            RS485_TX_BUFF[3 + i * 2] = (*Modbus_HoldReg[startRegAddr + i] >> 8) & 0xFF;     //先发送高字节
            RS485_TX_BUFF[4 + i * 2] = (*Modbus_HoldReg[startRegAddr + i]) & 0xFF;          //再发送低字节
        }
        calCRC = crc_compute(RS485_TX_BUFF, RegNum * 2 + 3);
        RS485_TX_BUFF[RegNum * 2 + 3] = (calCRC >> 8) & 0xFF;       // 先高后低
        RS485_TX_BUFF[RegNum * 2 + 4] = (calCRC) & 0xFF;
        RS485_send_data(RS485_TX_BUFF, RegNum * 2 + 5);
    } else {		//寄存器地址+数量超出范围
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;			//异常码 
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


void modbus_16_solve(void)
{
    u8 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | ((RS485_RX_BUFF[5]));	//获取寄存器数量
    if((startRegAddr + RegNum) < 1000) {		//寄存器地址+数量在范围内
		for(i = 0; i < RegNum; i++) {
            *Modbus_HoldReg[startRegAddr + i] = RS485_RX_BUFF[7 + i * 2];	//低字节在前                 /////// 低字节在前，高字节在后正常
            *Modbus_HoldReg[startRegAddr + i] |= ((u16)RS485_RX_BUFF[8 + i * 2]) << 8;	//高字节在后
        }
                 
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        RS485_TX_BUFF[2] = RS485_RX_BUFF[2];
        RS485_TX_BUFF[3] = RS485_RX_BUFF[3];
        RS485_TX_BUFF[4] = RS485_RX_BUFF[4];
        RS485_TX_BUFF[5] = RS485_RX_BUFF[5];
                 
        calCRC = crc_compute(RS485_TX_BUFF, 6);
        RS485_TX_BUFF[6] = (calCRC >> 8) & 0xFF;
        RS485_TX_BUFF[7] = (calCRC) & 0xFF;
        RS485_send_data(RS485_TX_BUFF, 8);
    } else {		//寄存器地址+数量超出范围
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;		//异常码
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


// void modbus_04_solve(void)
// {
//     u8 i;
//     RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5]; //获取寄存器数量
//     if((startRegAddr + RegNum) < 1000) {        //寄存器地址+数量在范围内
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
//         RS485_TX_BUFF[2] = RegNum * 2;
//         for(i = 0; i < RegNum; i++) {
//             RS485_TX_BUFF[3 + i * 2] = (*Modbus_HoldReg[startRegAddr + i] >> 8) & 0xFF; //先发送高字节
//             RS485_TX_BUFF[4 + i * 2] = *Modbus_HoldReg[startRegAddr + i] & 0xFF;        //后发送低字节
//         }
//         calCRC = crc_compute(RS485_TX_BUFF, RegNum * 2 + 3);
//         RS485_TX_BUFF[RegNum * 2 + 3] = calCRC & 0xFF;
//         RS485_TX_BUFF[RegNum * 2 + 4] = (calCRC >> 8) & 0xFF;
//         RS485_send_data(RS485_TX_BUFF, RegNum * 2 + 5);
//     } else {        //寄存器地址+数量超出范围
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
//         RS485_TX_BUFF[2] = 0x02;        //异常码
//         RS485_send_data(RS485_TX_BUFF, 3);
//     }
// }


// void modbus_05_solve(void)
// {
//     if(startRegAddr < 100) {		//寄存器地址在范围内
//         if((RS485_RX_BUFF[4] == 0xFF) || (RS485_RX_BUFF[5] == 0xFF)) 
// 			*Modbus_OutputIO[startRegAddr] = 0x01;
//         else 
// 			*Modbus_OutputIO[startRegAddr] = 0x00;
		
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
//         RS485_TX_BUFF[2] = RS485_RX_BUFF[2];
//         RS485_TX_BUFF[3] = RS485_RX_BUFF[3];
//         RS485_TX_BUFF[4] = RS485_RX_BUFF[4];
//         RS485_TX_BUFF[5] = RS485_RX_BUFF[5];
                 
//         calCRC = crc_compute(RS485_TX_BUFF, 6);
//         RS485_TX_BUFF[6] = (calCRC >> 8) & 0xFF;
//         RS485_TX_BUFF[7] = (calCRC) & 0xFF;
//         RS485_send_data(RS485_TX_BUFF, 8);
//     } else {		//寄存器地址超出范围
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
//         RS485_TX_BUFF[2] = 0x02;		//异常码
//         RS485_send_data(RS485_TX_BUFF, 3);
//     }
// }


// void modbus_06_solve(void)
// {
//     *Modbus_HoldReg[startRegAddr] = RS485_RX_BUFF[4] << 8;		//高字节在前                    ////////修改为高字节在前，低字节在后
//     *Modbus_HoldReg[startRegAddr] |= ((u16)RS485_RX_BUFF[5]);	//低字节在后
         
//     RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//     RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
//     RS485_TX_BUFF[2] = RS485_RX_BUFF[2];
//     RS485_TX_BUFF[3] = RS485_RX_BUFF[3];
//     RS485_TX_BUFF[4] = RS485_RX_BUFF[4];
//     RS485_TX_BUFF[5] = RS485_RX_BUFF[5];
         
//     calCRC = crc_compute(RS485_TX_BUFF, 6);
//     RS485_TX_BUFF[6] = (calCRC >> 8) & 0xFF;
//     RS485_TX_BUFF[7] = (calCRC) & 0xFF;
//     RS485_send_data(RS485_TX_BUFF, 8);
// }


// void modbus_15_solve(void)
// {
// 	u16 i;
//     RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];		//获取寄存器数量
//     if((startRegAddr + RegNum) < 100) {		//寄存器地址+数量在范围内        
//         for(i = 0; i < RegNum; i++) {
//             if(RS485_RX_BUFF[7 + i / 8] & 0x01) 
// 				*Modbus_OutputIO[startRegAddr + i] = 0x01;
//             else 
// 				*Modbus_OutputIO[startRegAddr + i] = 0x00;
//             RS485_RX_BUFF[7 + i / 8] >>= 1;		//从低位开始
//         }
                 
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
//         RS485_TX_BUFF[2] = RS485_RX_BUFF[2];
//         RS485_TX_BUFF[3] = RS485_RX_BUFF[3];
//         RS485_TX_BUFF[4] = RS485_RX_BUFF[4];
//         RS485_TX_BUFF[5] = RS485_RX_BUFF[5];
//         calCRC = crc_compute(RS485_TX_BUFF, 6);
//         RS485_TX_BUFF[6] = (calCRC >> 8) & 0xFF;
//         RS485_TX_BUFF[7] = (calCRC) & 0xFF;
//         RS485_send_data(RS485_TX_BUFF, 8);
//     } else {		//寄存器地址+数量超出范围
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
//         RS485_TX_BUFF[2] = 0x02;		//异常码
//         RS485_send_data(RS485_TX_BUFF, 3);
//     }
// }



