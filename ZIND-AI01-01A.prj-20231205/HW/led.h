#ifndef __LED_H
#define __LED_H	 

#include "sys.h"


#define WORK_LED PAout(4)	


/**
 * @description: LED initialize function
 * @return     :{*}
 * @note      : PA.4 used output pin and enable portA clk
 */
void led_init(void);


void signal_led_init(void);


/**
 * @description: LED flash function
 * @return     :{*}
 * @note      : ARM work indicating lamp, flash
 */
void led_flash(void);

		 				    
#endif
