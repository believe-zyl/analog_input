#ifndef __RELAY_H
#define __RELAY_H	 

#include "sys.h"


//relay pins configuration
#define RELAY3 PCout(9)
#define RELAY2 PCout(8)
#define RELAY1 PCout(7)
#define RELAY0 PCout(6)


/**
 * @description: relay initialize function
 * @return     :{*}
 * @note      : PC6~9 used output pins and enable portC clk
 */
void relay_init(void);

		 				    
#endif
