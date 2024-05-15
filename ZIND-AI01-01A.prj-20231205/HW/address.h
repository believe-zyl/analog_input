#ifndef __ADDRESS_H
#define __ADDRESS_H	 



/**
 * @description: module addr initialize function
 * @return     :{*}
 * @note      : PE8~15 used pull on the input pins and enable portE clk
 *              default module address : 1111 1111
 */	 
void address_init(void);


/**
 * @description: read module address
 * @return     :{addr} address
 * @note      : 
 */
u8 module_addr(void);



#endif


