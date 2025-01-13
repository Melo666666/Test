#ifndef _BSPTIMER_H_
#define _BSPTIMER_H_

#include "stm32f10x.h"

void bspTIM2Init(uint16_t period);
void bspTIM2Enable(void);
void bspTIM2Disable(void);

#endif
