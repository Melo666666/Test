#ifndef _BSPDELAY_H_
#define _BSPDELAY_H_

#include "stm32f10x.h"

void bspSystickInit(void);
void bspDelay100Us(__IO u32 nUs);

#define bspDelayMs(x) bspDelay100Us(10*x)	

#endif

