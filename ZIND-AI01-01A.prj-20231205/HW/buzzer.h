#ifndef __BUZZER_H
#define __BUZZER_H	 

#include "sys.h"
 

#define BUZZER1 PCout(4)	


/**
 * @description: buzzer initialize function
 * @return     :{*}
 * @note      : PC4 used output pins and enable portC clk
 */
void buzzer_init(void);	

 
/**
 * @description: buzzer1 work
 * @return     :{*}
 * @note      : buzzer1 operates at a frequency of 1s
 */
void buzzer1_work(void);

		 				    
#endif
