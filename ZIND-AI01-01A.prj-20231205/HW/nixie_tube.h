#ifndef __NIXIE_TUBE_H
#define __NIXIE_TUBE_H	 



#define SPI2_CS_HIGH	GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define SPI2_CS_LOW		GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define NIXIE_TUBE1_ENABLE	        GPIO_ResetBits(GPIOD, GPIO_Pin_8)
#define NIXIE_TUBE1_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_8)

#define NIXIE_TUBE2_ENABLE  	    GPIO_ResetBits(GPIOD, GPIO_Pin_9)
#define NIXIE_TUBE2_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_9)

#define NIXIE_TUBE3_ENABLE  	    GPIO_ResetBits(GPIOD, GPIO_Pin_10)
#define NIXIE_TUBE3_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_10)

#define NIXIE_TUBE4_ENABLE  	    GPIO_ResetBits(GPIOD, GPIO_Pin_11)
#define NIXIE_TUBE4_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_11)

#define NIXIE_TUBE5_ENABLE  	    GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define NIXIE_TUBE5_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_12)

#define NIXIE_TUBE6_ENABLE  	    GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define NIXIE_TUBE6_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_13)

#define NIXIE_TUBE7_ENABLE  	    GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define NIXIE_TUBE7_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_14)

#define NIXIE_TUBE8_ENABLE  	    GPIO_ResetBits(GPIOD, GPIO_Pin_15)
#define NIXIE_TUBE8_DISENABLE		GPIO_SetBits(GPIOD, GPIO_Pin_15)


/**
 * @description: nixie_tube initialize function
 * @return     :{*}
 * @note      : PD8~15 used output pins and enable portD clk
 */
void nixie_tube_init(void);		


/**
 * @description: nixie_tube show function
 * @param      :{u8} n
 * @return     :{*}
 * @note      : refresh nixie_tube 
 */
void nixie_tube_show(u8 n);

		 				    
#endif
