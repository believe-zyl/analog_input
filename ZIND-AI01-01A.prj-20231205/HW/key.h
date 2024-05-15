#ifndef __KEY_H
#define __KEY_H	 

	 

#define KEY0 PEin(2)		//PE2 used erasure
#define KEY1 PEin(3)		//PE3 used eliminate flash
#define KEY2 PEin(4)		//PE4 used test


/**
 * @description: key initialize function
 * @return     :{*}
 * @note      : PE2~4 used pull on the input pins and enable portE clk
 *              default effective of key : low
 */	 
void key_init(void);


/**
 * @description: key scan function
 * @return     :{*}
 * @note      : KEY0 used erasure : all buzzer off
                KEY1 used eliminate flash : all relay off
 */
void key_scan(void);



#endif


