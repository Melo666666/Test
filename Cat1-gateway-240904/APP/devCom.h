#ifndef _DEVCOM_H
#define _DEVCOM_H

#include "stm32f10x.h"


#define DEV_SND_NAME			("dev snd thread")
#define DEV_RCV_NAME			("dev rcv thread")
#define DEV_STACKDEPTH			(256)
#define DEV_SND_PRIORITY		(configMAX_PRIORITIES-2)
#define DEV_RCV_PRIORITY		(configMAX_PRIORITIES-2)

#define DEV_QUEUE_LENGTH		50




void devComThread_init(void);



#endif /* LEDTHREAD_H */
