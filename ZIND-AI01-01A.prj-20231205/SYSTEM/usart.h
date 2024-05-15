#ifndef __USART_H
#define __USART_H

#include "sys.h"


#define RS485_TX_EN PAout(1)	//PA1 used to select TX or RX  //H:TX or L:RX



/**
 * @description: 485 initialize function
 * @param      :{u32} RS485_BAUDRATE ：通讯波特率，一般为9600
 * @param      :{u8} RS485_PARITY ：0：无校验   1：奇校验  2：偶校验
 * @return     :{*}
 * @note      : USART2_TX   PA.2
 *              USART2_RX	PA.3
 *              485_nCS  PA.1
 */
void uart_485_init(u32 RS485_BAUDRATE, u8 RS485_PARITY);


/**
 * @description:485 send n byte data
 * @param      :{u8} *buff : 发送区首地址
 * @param      :{u8} len : 发送的字节数
 * @return     :{*}
 */
void RS485_send_data(u8 *buff, u8 len);



#endif
