#ifndef _NVIC_H_
#define _NVIC_H_

#include "stm32f10x.h"

typedef enum{
    NoSend,
    UpperSend,
    UartSend,
    NetSend,
}Dev_Priority_Enum;

void NVIC_Configuration(void);
void set_dev_priority(u8 level);

#endif

