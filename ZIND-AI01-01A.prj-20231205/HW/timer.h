#ifndef __TIMER_H
#define __TIMER_H	 


/**
 * @description: timer2 initialize function
 * @param      :{u16} arr2
 * @param      :{u16} psc2
 * @return     :{*}
 * @note      : 
 */
void timer2_init(u16 arr2, u16 psc2);


/**
 * @description: timer3 initialize function
 * @param      :{u16} arr3
 * @param      :{u16} psc3
 * @return     :{*}
 * @note      : Tout = (arr * psc) / Tclk
 *              Tclk = 72MHz
 *              usual psc is 72
 *              arr = ms
 */
void timer3_init(u16 arr3, u16 psc3);


/**
 * @description: timer4 initialize function
 * @return     :{*}
 * @note       : 用于判断接收空闲时间，当空闲时间大于指定时间，认为一帧结束
 */
void timer4_init(void);
		 				    
#endif
