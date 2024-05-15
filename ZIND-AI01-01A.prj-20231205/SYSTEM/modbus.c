#include "stm32f10x.h"
#include "modbus.h"
#include "usart.h"
#include "input.h"
#include "address.h"


// u8 RS485_Addr = 1;                //�ӻ���ַ

extern u8 RS485_FRAME_FLAG;       //֡�������
extern u8 RS485_RX_BUFF[2048];    //���ջ�����2048�ֽ�
extern u16 RS485_RX_CNT;          //���ռ�����
u8 RS485_TX_BUFF[2048];           //���ͻ�����

u16 startRegAddr;       //�Ĵ�����ʼ��ַ
u16 RegNum;             //�Ĵ�������
u16 calCRC;             //���������ݵ�CRC

u32 *Modbus_InputIO[100];       //���뿪�����Ĵ���ָ��
u32 *Modbus_OutputIO[100];      //����������Ĵ���ָ��
u16 *Modbus_HoldReg[1000];      //���ּĴ���ָ��



void modbus_reg_map(void)
{
    //���뿪�����Ĵ���ָ��ָ��
    Modbus_InputIO[0] = (u32*) & PCin(4);      //KEY0
    Modbus_InputIO[1] = (u32*) & PCin(5);      //KEY1

    //����������Ĵ���ָ��ָ��
    Modbus_OutputIO[0] = (u32*) & PBout(6);    //RELAY0
    Modbus_OutputIO[1] = (u32*) & PBout(7);    //RELAY1
    Modbus_OutputIO[2] = (u32*) & PBout(8);    //RELAY2
    Modbus_OutputIO[3] = (u32*) & PBout(9);    //RELAY3

    //���ּĴ���ָ��ָ��
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
    return(((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8));	//���ظߵ��ֽ�ת�����crcֵ
}


void RS485_service(void)
{
    u16 recCRC;
    if(RS485_FRAME_FLAG == 1) {
        if(RS485_RX_BUFF[0] == module_addr()) {      //��ַ��ȷ
            if((RS485_RX_BUFF[1] == 01) || (RS485_RX_BUFF[1] == 02) || (RS485_RX_BUFF[1] == 03) || (RS485_RX_BUFF[1] == 04) || (RS485_RX_BUFF[1] == 05) || (RS485_RX_BUFF[1] == 06) || (RS485_RX_BUFF[1] == 15) || (RS485_RX_BUFF[1] == 16)) {  //��������ȷ
                startRegAddr = (((u16)RS485_RX_BUFF[2]) << 8) | RS485_RX_BUFF[3];   //��ȡ�Ĵ�����ʼ��ַ
                if(startRegAddr < 1000) {     //�Ĵ�����ַ�ڷ�Χ��       
                    calCRC = crc_compute(RS485_RX_BUFF, RS485_RX_CNT - 2);   //�������������ݵ�CRC
                    recCRC = RS485_RX_BUFF[RS485_RX_CNT - 1] | (((u16)RS485_RX_BUFF[RS485_RX_CNT - 2]) << 8);   //���յ���CRC
                    if(calCRC == recCRC) {          //CRCУ����ȷ
                        switch(RS485_RX_BUFF[1]) {  //���ݲ�ͬ�Ĺ�������д���
                            case 01: {       //�����������
                                modbus_01_solve();
                                break;
                            }
                            case 02: {       //�����뿪����
                                modbus_02_solve();
                                break;
                            }                            
                            case 03: {       //��������ּĴ���
                                modbus_03_solve();
                                break;
                            }
                            // case 04: {       //���������Ĵ���
                            //     modbus_04_solve();
                            //     break;
                            // }
                            // case 05: {       //д�������������
                            //     modbus_05_solve();
                            //     break;
                            // }
                            // case 06: {       //д�������ּĴ���
                            //     modbus_06_solve();
                            //     break;
                            // }
                            // case 15: {       //д������������
                            //     modbus_15_solve();
                            //     break;
                            // }
                            // case 16: {       //д������ּĴ���
                            //     modbus_16_solve();
                            //     break;
                            // }
                        }
                    } else {     //CRCУ�����
                        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                        RS485_TX_BUFF[2] = 0x04;        //�쳣��
                        RS485_send_data(RS485_TX_BUFF, 3);
                    }        
                } else {     //�Ĵ�����ַ������Χ
                    RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                    RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                    RS485_TX_BUFF[2] = 0x02;        //�쳣��
                    RS485_send_data(RS485_TX_BUFF,3);
                }                                                
            } else {     //���������
                RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
                RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
                RS485_TX_BUFF[2] = 0x01;        //�쳣��
                RS485_send_data(RS485_TX_BUFF, 3);
            }
        }

        RS485_FRAME_FLAG = 0;   //��λ֡������־
        RS485_RX_CNT = 0;       //���ռ���������
        RS485_TX_EN = 0;      //��������ģʽ
    }                
}


void modbus_01_solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];	//��ȡ�Ĵ�������
    if((startRegAddr + RegNum) < 100) {		//�Ĵ�����ַ+�����ڷ�Χ��
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        ByteNum = RegNum / 8;	    //�ֽ���
        if(RegNum % 8)
			ByteNum += 1;		    //���λ���������������ֽ���+1
        RS485_TX_BUFF[2] = ByteNum;	//����Ҫ��ȡ���ֽ���
        for(i = 0; i < RegNum; i++) {
            if(i % 8 == 0) 
				RS485_TX_BUFF[3 + i / 8] = 0x00;
            RS485_TX_BUFF[3 + i / 8] >>= 1;		//��λ�ȷ���
            RS485_TX_BUFF[3 + i / 8] |= ((*Modbus_OutputIO[startRegAddr + i]) << 7) & 0x80;
            if(i == RegNum - 1)	{	//���͵����һ��λ��
                if(RegNum % 8)
					RS485_TX_BUFF[3 + i / 8] >>= 8 - (RegNum % 8);	//������һ���ֽڻ�����������ʣ��MSB���0
            }
        }
        calCRC = crc_compute(RS485_TX_BUFF, ByteNum + 3);
        RS485_TX_BUFF[ByteNum + 3] = (calCRC >> 8) & 0xFF;
        RS485_TX_BUFF[ByteNum + 4] = (calCRC) & 0xFF;
        RS485_send_data(RS485_TX_BUFF, ByteNum + 5);
    } else {		//�Ĵ�����ַ+����������Χ
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;			//�쳣��
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


void modbus_02_solve(void)
{
    u16 ByteNum;
    u16 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];	//��ȡ�Ĵ�������
    if((startRegAddr + RegNum) < 100) {		//�Ĵ�����ַ+�����ڷ�Χ��
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        ByteNum = RegNum / 8;		//�ֽ���
        if(RegNum % 8) 
			ByteNum += 1;			//���λ���������������ֽ���+1
        RS485_TX_BUFF[2] = ByteNum;	//����Ҫ��ȡ���ֽ���
        for(i = 0; i < RegNum; i++) {
            if(i % 8 == 0) 
				RS485_TX_BUFF[3 + i / 8] = 0x00;
        	RS485_TX_BUFF[3 + i / 8] >>= 1;	//��λ�ȷ���
            RS485_TX_BUFF[3 + i / 8] |= ((*Modbus_InputIO[startRegAddr + i]) << 7) & 0x80;
            if(i == RegNum - 1)	{	//���͵����һ��λ��
                if(RegNum % 8) 
					RS485_TX_BUFF[3 + i / 8] >>= 8 - (RegNum % 8);	//������һ���ֽڻ�����������ʣ��MSB���0
            }
        }
        calCRC = crc_compute(RS485_TX_BUFF, ByteNum + 3);
        RS485_TX_BUFF[ByteNum + 3] = (calCRC >> 8) & 0xFF;
        RS485_TX_BUFF[ByteNum + 4] = (calCRC) & 0xFF;
        RS485_send_data(RS485_TX_BUFF, ByteNum + 5);
    } else	{	//�Ĵ�����ַ+����������Χ
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;		//�쳣��
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


void modbus_03_solve(void)
{
    u8 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];	//��ȡ�Ĵ�������
    if((startRegAddr + RegNum) < 1000) {		//�Ĵ�����ַ+�����ڷ�Χ��
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
        RS485_TX_BUFF[2] = RegNum * 2;
        for(i = 0 ; i < RegNum; i++) {
            RS485_TX_BUFF[3 + i * 2] = (*Modbus_HoldReg[startRegAddr + i] >> 8) & 0xFF;     //�ȷ��͸��ֽ�
            RS485_TX_BUFF[4 + i * 2] = (*Modbus_HoldReg[startRegAddr + i]) & 0xFF;          //�ٷ��͵��ֽ�
        }
        calCRC = crc_compute(RS485_TX_BUFF, RegNum * 2 + 3);
        RS485_TX_BUFF[RegNum * 2 + 3] = (calCRC >> 8) & 0xFF;       // �ȸߺ��
        RS485_TX_BUFF[RegNum * 2 + 4] = (calCRC) & 0xFF;
        RS485_send_data(RS485_TX_BUFF, RegNum * 2 + 5);
    } else {		//�Ĵ�����ַ+����������Χ
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;			//�쳣�� 
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


void modbus_16_solve(void)
{
    u8 i;
    RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | ((RS485_RX_BUFF[5]));	//��ȡ�Ĵ�������
    if((startRegAddr + RegNum) < 1000) {		//�Ĵ�����ַ+�����ڷ�Χ��
		for(i = 0; i < RegNum; i++) {
            *Modbus_HoldReg[startRegAddr + i] = RS485_RX_BUFF[7 + i * 2];	//���ֽ���ǰ                 /////// ���ֽ���ǰ�����ֽ��ں�����
            *Modbus_HoldReg[startRegAddr + i] |= ((u16)RS485_RX_BUFF[8 + i * 2]) << 8;	//���ֽ��ں�
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
    } else {		//�Ĵ�����ַ+����������Χ
        RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
        RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
        RS485_TX_BUFF[2] = 0x02;		//�쳣��
        RS485_send_data(RS485_TX_BUFF, 3);
    }
}


// void modbus_04_solve(void)
// {
//     u8 i;
//     RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5]; //��ȡ�Ĵ�������
//     if((startRegAddr + RegNum) < 1000) {        //�Ĵ�����ַ+�����ڷ�Χ��
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1];
//         RS485_TX_BUFF[2] = RegNum * 2;
//         for(i = 0; i < RegNum; i++) {
//             RS485_TX_BUFF[3 + i * 2] = (*Modbus_HoldReg[startRegAddr + i] >> 8) & 0xFF; //�ȷ��͸��ֽ�
//             RS485_TX_BUFF[4 + i * 2] = *Modbus_HoldReg[startRegAddr + i] & 0xFF;        //���͵��ֽ�
//         }
//         calCRC = crc_compute(RS485_TX_BUFF, RegNum * 2 + 3);
//         RS485_TX_BUFF[RegNum * 2 + 3] = calCRC & 0xFF;
//         RS485_TX_BUFF[RegNum * 2 + 4] = (calCRC >> 8) & 0xFF;
//         RS485_send_data(RS485_TX_BUFF, RegNum * 2 + 5);
//     } else {        //�Ĵ�����ַ+����������Χ
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
//         RS485_TX_BUFF[2] = 0x02;        //�쳣��
//         RS485_send_data(RS485_TX_BUFF, 3);
//     }
// }


// void modbus_05_solve(void)
// {
//     if(startRegAddr < 100) {		//�Ĵ�����ַ�ڷ�Χ��
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
//     } else {		//�Ĵ�����ַ������Χ
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
//         RS485_TX_BUFF[2] = 0x02;		//�쳣��
//         RS485_send_data(RS485_TX_BUFF, 3);
//     }
// }


// void modbus_06_solve(void)
// {
//     *Modbus_HoldReg[startRegAddr] = RS485_RX_BUFF[4] << 8;		//���ֽ���ǰ                    ////////�޸�Ϊ���ֽ���ǰ�����ֽ��ں�
//     *Modbus_HoldReg[startRegAddr] |= ((u16)RS485_RX_BUFF[5]);	//���ֽ��ں�
         
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
//     RegNum = (((u16)RS485_RX_BUFF[4]) << 8) | RS485_RX_BUFF[5];		//��ȡ�Ĵ�������
//     if((startRegAddr + RegNum) < 100) {		//�Ĵ�����ַ+�����ڷ�Χ��        
//         for(i = 0; i < RegNum; i++) {
//             if(RS485_RX_BUFF[7 + i / 8] & 0x01) 
// 				*Modbus_OutputIO[startRegAddr + i] = 0x01;
//             else 
// 				*Modbus_OutputIO[startRegAddr + i] = 0x00;
//             RS485_RX_BUFF[7 + i / 8] >>= 1;		//�ӵ�λ��ʼ
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
//     } else {		//�Ĵ�����ַ+����������Χ
//         RS485_TX_BUFF[0] = RS485_RX_BUFF[0];
//         RS485_TX_BUFF[1] = RS485_RX_BUFF[1] | 0x80;
//         RS485_TX_BUFF[2] = 0x02;		//�쳣��
//         RS485_send_data(RS485_TX_BUFF, 3);
//     }
// }



