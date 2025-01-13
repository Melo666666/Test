#include "NVIC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ledthread.h"
#include "bspIWDG.h"
#include "clm920.h"
#include "modbusCB.h"
#include "cfgThread.h"
#include "netThread.h"
#include "airSensor.h"
#include "devCom.h"



int main(void)
{
	/* NVIC Set. */
	NVIC_Configuration();
	/* freeRTOS objects set. */
	
	ledthread_init();
	modbus_init();
	
	cfgThread_init();
	netThread_init();
	airSensorThread_init();
	devComThread_init();
	
	IWDG_Init(IWDG_Prescaler_64, 156 * 10);
	/* start the scheduler. */
	vTaskStartScheduler();

	/* the end. */
	while(1) {};
}



