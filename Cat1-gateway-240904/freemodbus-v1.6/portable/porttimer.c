/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "bspTimer.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTimerout50us)
{
    bspTIM2Init(usTimerout50us * 50);
    return TRUE;
}

void vMBPortTimersEnable()
{
    bspTIM2Enable();
}

void vMBPortTimersDisable()
{
    bspTIM2Disable();
}

static void prvvTIMERExpiredISR(void)
{
    (void)pxMBPortCBTimerExpired();
}

// [F] 定时器tim2中断处理函数
void TIM2_IRQHandler(void)
{
    prvvTIMERExpiredISR();
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
// END
