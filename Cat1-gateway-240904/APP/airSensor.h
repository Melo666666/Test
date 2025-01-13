#ifndef _AIRSENSOR_H
#define _AIRSENSOR_H

#include "stm32f10x.h"


#define AIRSENSOR_SND_NAME			("airsensor snd thread")
#define AIRSENSOR_RCV_NAME			("airsensor rcv thread")
#define AIRSENSOR_STACKDEPTH		(256)
#define AIRSENSOR_SND_PRIORITY		(configMAX_PRIORITIES-3)
#define AIRSENSOR_RCV_PRIORITY		(configMAX_PRIORITIES-3)

#define 	AIR_QUEUE_LENGTH		50




void airSensorThread_init(void);



#endif /* LEDTHREAD_H */
