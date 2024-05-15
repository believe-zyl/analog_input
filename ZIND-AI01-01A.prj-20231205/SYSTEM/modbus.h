#ifndef __MODBUS_H
#define __MODBUS_H


/**
 * @description: Modbus�Ĵ�����STM32�Ĵ�����ӳ���ϵ
 * @return     :{*}
 */
void modbus_reg_map(void);

/**
 * @description: ModBus_RTU CRC compute function
 * @param      :{u8} *ptr   :��ҪУ����ֽ�����
 * @param      :{u8} len    :��ҪУ����ֽ���
 * @return     :{u16} crc   :16λ��CRCУ����
 */
u16 crc_compute(u8 *ptr,u8 len);

/**
 * @description: ModBus������
 * @return     :{*}
 * @note       : RS485����������ڴ�����յ�������(������������ѭ������)
 */
void RS485_service(void);

/**
 * @description: Modbus������01�������
 * @return     :{*}
 * @note       : �����������
 */
void modbus_01_solve(void);

/**
 * @description: Modbus������02�������
 * @return     :{*}
 * @note       : �����뿪����
 */
void modbus_02_solve(void);

/**
 * @description: Modbus������03�������
 * @return     :{*}
 * @note       : �����ּĴ���
 */
void modbus_03_solve(void);

// /**
//  * @description: Modbus������04�������
//  * @return     :{*}
//  * @note       : ������Ĵ���
//  */
// void modbus_04_solve(void);

// /**
//  * @description: Modbus������05�������
//  * @return     :{*}
//  * @note       : д�������������
//  */
// void modbus_05_solve(void);

// /**
//  * @description: Modbus������06�������
//  * @return     :{*}
//  * @note       : д�������ּĴ���
//  */
// void modbus_06_solve(void);

// /**
//  * @description: Modbus������15�������
//  * @return     :{*}
//  * @note       : д������������
//  */
// void modbus_15_solve(void);

/**
 * @description: Modbus������16�������
 * @return     :{*}
 * @note       : д������ּĴ���
 */
void modbus_16_solve(void);


#endif

