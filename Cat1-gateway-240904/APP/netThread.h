#ifndef NETTHREAD_H
#define NETTHREAD_H

#include "stm32f10x.h"

#define NET_NAME		("net thread")
#define NET_STACKDEPTH	(256)
#define NET_PRIORITY	(configMAX_PRIORITIES-3)


void netThread_init(void);



#endif /* LEDTHREAD_H */
