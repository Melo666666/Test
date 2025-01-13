#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

#define SYS_RUNLED_PORT  			GPIOB
#define SYS_RUNLED_PIN   			GPIO_Pin_15
#define SYS_RUNLED_PIN_GPIO_CLK    	RCC_APB2Periph_GPIOB


#define bspLedOFF()   				GPIO_SetBits(SYS_RUNLED_PORT, SYS_RUNLED_PIN)
#define bspLedON()  				GPIO_ResetBits(SYS_RUNLED_PORT, SYS_RUNLED_PIN)

void bspLedInit(void);
void bspLedToggle(void);


#endif

