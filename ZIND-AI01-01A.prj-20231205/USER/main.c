#include "stm32f10x_conf.h"
#include "sys.h"	
#include "delay.h"	
#include "usart.h"
#include "key.h"
#include "relay.h"
#include "spi.h"
#include "input.h"
#include "nixie_tube.h"
#include "timer.h"
#include "modbus.h"





int main(void)
{
	SystemInit();		//系统时钟设置
	NVIC_Configuration();
	delay_init(72);		//延时初始化
	key_init();			//初始化与按键连接的硬件接口
    relay_init();
	nixie_tube_init();
	timer2_init(500, 72);
	timer3_init(300, 72);

	ADS8688_Init();

	uart_485_init(9600, 0);


	while(1)
	{
		u8 i;
		u8 channel_number = 8;
		u8 AI_RANGE = 0;

		get_ADS_all(channel_number, adc_real, AI_RANGE);
		for(i = 0; i < 8; i++) {
			if(adc_real[i] > 100 && adc_real[i] < 600) {
				TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
				nixie_tube_show(i);
				RELAY0 = 1;
            	RELAY1 = 1;
			} else if(adc_real[i] > 6615) {
				TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
				nixie_tube_show(i);
				RELAY2 = 1;
            	RELAY3 = 1;
			}
		}

		key_scan();

		RS485_service();
	}
}


