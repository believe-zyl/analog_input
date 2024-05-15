#ifndef __MODBUS_H
#define __MODBUS_H


/**
 * @description: Modbus寄存器和STM32寄存器的映射关系
 * @return     :{*}
 */
void modbus_reg_map(void);

/**
 * @description: ModBus_RTU CRC compute function
 * @param      :{u8} *ptr   :需要校验的字节数组
 * @param      :{u8} len    :需要校验的字节数
 * @return     :{u16} crc   :16位的CRC校验码
 */
u16 crc_compute(u8 *ptr,u8 len);

/**
 * @description: ModBus服务函数
 * @return     :{*}
 * @note       : RS485服务程序，用于处理接收到的数据(请在主函数中循环调用)
 */
void RS485_service(void);

/**
 * @description: Modbus功能码01处理程序
 * @return     :{*}
 * @note       : 读输出开关量
 */
void modbus_01_solve(void);

/**
 * @description: Modbus功能码02处理程序
 * @return     :{*}
 * @note       : 读输入开关量
 */
void modbus_02_solve(void);

/**
 * @description: Modbus功能码03处理程序
 * @return     :{*}
 * @note       : 读保持寄存器
 */
void modbus_03_solve(void);

// /**
//  * @description: Modbus功能码04处理程序
//  * @return     :{*}
//  * @note       : 读输入寄存器
//  */
// void modbus_04_solve(void);

// /**
//  * @description: Modbus功能码05处理程序
//  * @return     :{*}
//  * @note       : 写单个输出开关量
//  */
// void modbus_05_solve(void);

// /**
//  * @description: Modbus功能码06处理程序
//  * @return     :{*}
//  * @note       : 写单个保持寄存器
//  */
// void modbus_06_solve(void);

// /**
//  * @description: Modbus功能码15处理程序
//  * @return     :{*}
//  * @note       : 写多个输出开关量
//  */
// void modbus_15_solve(void);

/**
 * @description: Modbus功能码16处理程序
 * @return     :{*}
 * @note       : 写多个保持寄存器
 */
void modbus_16_solve(void);


#endif

