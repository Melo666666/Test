#include "bspDelay.h"

static __IO u32 TimingDelay;

void bspSystickInit(void)
{
	/* SystemFrequency / 10000    100us中断一次
	 */
	if(SysTick_Config(SystemCoreClock / 10000))
	{
		/* Capture error */ 
		while (1);
	}
}

//延迟上限为60s
void bspDelay100Us(__IO u32 nUs)
{
	TimingDelay = nUs;	
	while(TimingDelay != 0);
}


void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00){ 
		TimingDelay--;
	}
	
	else if(TimingDelay > 0x005B8D80){
		TimingDelay = 0;
	}
}
