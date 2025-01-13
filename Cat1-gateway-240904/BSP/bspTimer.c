#include "bspTimer.h"

//
void bspTIM2Init(uint16_t period)
{
	TIM_TimeBaseInitTypeDef TIM_Initstructure;
	/* 本项目使用 HSI 作为时钟源, SYSCLK = 8MHz, APB1 TIMER = 8MHz. */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_Initstructure.TIM_Prescaler = TIM_CKD_DIV4;
	TIM_Initstructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Initstructure.TIM_Period = 2 * period *9;
	
	TIM_TimeBaseInit(TIM2, &TIM_Initstructure);
}
//

//
void bspTIM2Enable(void)
{
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}
//

//
void bspTIM2Disable(void)
{
	TIM_Cmd(TIM2, DISABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
}
//

