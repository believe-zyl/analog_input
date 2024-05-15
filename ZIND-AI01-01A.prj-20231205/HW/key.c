#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
#include "relay.h"
#include "buzzer.h"
#include "nixie_tube.h"



void key_init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);		 
}


void key_scan(void)
{	 
	static u8 key_up = 1;	//key undo flag
 
	if(key_up && (KEY0 == 0 || KEY1 == 0)) {
		delay_ms(10);		//Debounce 
		key_up = 0;

		if(KEY0 == 0) {			                    
			RELAY0 = 0;
            RELAY1 = 0;
            RELAY2 = 0;
            RELAY3 = 0;
			TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
			BUZZER1 = 0;
		} else { 
			nixie_tube_show(21);
		}
	} else if(KEY0 == 1 && KEY1 == 1)
		key_up = 1; 	    
}


