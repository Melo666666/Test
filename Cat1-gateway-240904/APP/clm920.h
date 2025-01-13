#ifndef CLM920_H
#define CLM920_H

#include "stm32f10x.h"



#define CLM920_RCV_NAME			("clm920 rcv thread")
#define CLM920_SND_NAME			("clm920 snd thread")
#define CLM920_CHK_NAME			("clm920 chk thread")
#define CLM920_STACKDEPTH		(256)
#define CLM920_SND_PRIORITY		(configMAX_PRIORITIES-1)
#define CLM920_RCV_PRIORITY		(configMAX_PRIORITIES-1)
#define CLM920_CHK_PRIORITY		(configMAX_PRIORITIES-2)


void Clm920_Thread_Init(void);


#endif /* LINK72_H */
