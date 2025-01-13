#ifndef CFGTHREAD_H
#define CFGTHREAD_H

#include "stm32f10x.h"

#define CFG_NAME			("cfg thread")
#define	CFG_STACKDEPTH		(256)
#define CFG_PRIORITY		(configMAX_PRIORITIES-4)

#define SYS_NAME			("sys thread")
#define	SYS_STACKDEPTH		(256)
#define SYS_PRIORITY		(configMAX_PRIORITIES-4)


#define SOFTVISION		1009
#define HARDVISION		1002


//typedef struct{
//	u16 regID;
//	int16_t regVal;
//	
//}dev_unit_struct;

void cfgThread_init(void);

void write_device(u16 regId, u16 regVal);

#endif /* LEDTHREAD_H */
