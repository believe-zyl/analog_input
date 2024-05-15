#ifndef __USART_H
#define __USART_H

#include "sys.h"


#define RS485_TX_EN PAout(1)	//PA1 used to select TX or RX  //H:TX or L:RX



/**
 * @description: 485 initialize function
 * @param      :{u32} RS485_BAUDRATE ��ͨѶ�����ʣ�һ��Ϊ9600
 * @param      :{u8} RS485_PARITY ��0����У��   1����У��  2��żУ��
 * @return     :{*}
 * @note      : USART2_TX   PA.2
 *              USART2_RX	PA.3
 *              485_nCS  PA.1
 */
void uart_485_init(u32 RS485_BAUDRATE, u8 RS485_PARITY);


/**
 * @description:485 send n byte data
 * @param      :{u8} *buff : �������׵�ַ
 * @param      :{u8} len : ���͵��ֽ���
 * @return     :{*}
 */
void RS485_send_data(u8 *buff, u8 len);



#endif
