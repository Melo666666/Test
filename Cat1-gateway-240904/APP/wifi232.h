#ifndef WIFI232_H
#define WIFI232_H

#include "stm32f10x.h"



#define WIFI232_RCV_NAME			("wifi232 rcv thread")
#define WIFI232_SND_NAME			("wifi232 snd thread")
#define WIFI232_CHK_NAME			("wifi232 chk thread")
#define WIFI232_STACKDEPTH			(256)
#define WIFI232_SND_PRIORITY		(configMAX_PRIORITIES-1)
#define WIFI232_RCV_PRIORITY		(configMAX_PRIORITIES-1)
#define WIFI232_CHK_PRIORITY		(configMAX_PRIORITIES-2)


void Wifi232_Thread_Init(void);


#endif /* LINK72_H */
