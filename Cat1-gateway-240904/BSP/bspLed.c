#include "bspLed.h"



void bspLedInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SYS_RUNLED_PIN_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = SYS_RUNLED_PIN;
	GPIO_Init(SYS_RUNLED_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(SYS_RUNLED_PORT, SYS_RUNLED_PIN);
}

void bspLedToggle(void)
{
	if(1 == GPIO_ReadOutputDataBit(SYS_RUNLED_PORT, SYS_RUNLED_PIN))
		bspLedON();
	else bspLedOFF();
}

