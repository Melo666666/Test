#ifndef _BSPDEVCTL_H_
#define _BSPDEVCTL_H_

#include "stm32f10x.h"


#define Max_AirTemp 60 //  
#define Min_AirTemp 0 //  

#define Max_AirRH  100 // 
#define Min_AirRH  0 // 


typedef enum
{
	CTL_UPPER,
	CTL_HAND,
}ctl_source_enum;


void bsp_dev_ctl(void);
void bsp_dev_schedule(void);

#endif

