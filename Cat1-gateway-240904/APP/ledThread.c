#include "ledThread.h"
#include "bspLed.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bspIWDG.h"


#define LED_NAME		("led thread")
#define LED_STACKDEPTH	(128)
#define LED_PRIORITY	(configMAX_PRIORITIES-4)
const static TickType_t delay = 500 / portTICK_PERIOD_MS;

static void led_thread_entry(void *parameter);

//extern TaskHandle_t xHandle1;
//extern TaskHandle_t xHandle2;
//extern TaskHandle_t xHandle3;
//extern TaskHandle_t modbus_handle;
//size_t g_HeapSize=0;
//size_t g_LinkQrySize=0;
//size_t g_LinkSndSize=0;
//size_t g_LinkRcvSize=0;
//size_t g_ModbusSize=0;
/**
 * This function is the entry of led thread.
 */
static void led_thread_entry(void *parameter)
{
	bspLedInit();

	while(1)
	{
		bspLedToggle();
//		g_HeapSize = xPortGetFreeHeapSize();
//		g_LinkSndSize = uxTaskGetStackHighWaterMark(xHandle1);
//		g_LinkRcvSize = uxTaskGetStackHighWaterMark(xHandle2);
//		g_LinkQrySize = uxTaskGetStackHighWaterMark(xHandle3);
//		g_ModbusSize = uxTaskGetStackHighWaterMark(modbus_handle);
		vTaskDelay(delay);

		IWDG_Feed();
	}
}
/* end */

/**
 * This function initialize the thread of led.
 */
void ledthread_init(void)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	xReturned = xTaskCreate(led_thread_entry, LED_NAME, LED_STACKDEPTH, 0, LED_PRIORITY, &xHandle);

	if (xReturned != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}
/* end */
