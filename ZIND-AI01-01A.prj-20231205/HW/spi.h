#ifndef __SPI_H
#define __SPI_H	 


/**
 * @description: spi1 initialize function
 * @return     :{*}
 * @note      : cofig spi port 1;
 *              port1 used to analog input
 */
// void spi1_init(void);		


/**
 * @description: spi2 initialize function
 * @return     :{*}
 * @note      : cofig spi port 2;
 *              port2 used to drive nixie_tube
 */
void spi2_init(void);


/**
 * @description: spi1 read or write function
 * @param      :{u8} TxData : send out data
 * @return     :{u8} receive data
 * @note      : SPI1 read or write one Byte data
 */
// u8 spi1_read_write_byte(u8 TxData1);	


/**
 * @description: spi2 read or write function
 * @param      :{u8} TxData : send out data
 * @return     :{u8} receive data
 * @note      : SPI2 read or write one Byte data
 */
u8 spi2_read_write_byte(u8 TxData2);	

				    
#endif

