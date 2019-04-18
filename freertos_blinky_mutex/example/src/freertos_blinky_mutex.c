

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"




xSemaphoreHandle xMutex;



static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();


	Board_LED_Set(0, 0);
	Board_LED_Set(1, 0);
	Board_LED_Set(2, 0);
}

/* LED1 toggle thread */
static void vLEDTask1(void *pvParameters) {
	bool LedState = 0;

	while (1) {

		xSemaphoreTake(xMutex, portMAX_DELAY);

		Board_LED_Set(0, LedState);

		LedState = (bool) !LedState;

		vTaskDelay(configTICK_RATE_HZ  *1);
		xSemaphoreGive(xMutex);
	}
}

/* LED2 toggle thread */
static void vLEDTask2(void *pvParameters) {
	bool LedState = false;

	while (1) {
		xSemaphoreTake(xMutex, portMAX_DELAY);

		Board_LED_Set(1, LedState);

		LedState = (bool) !LedState;

		vTaskDelay(configTICK_RATE_HZ  *3);
		xSemaphoreGive(xMutex);
	}
}

/* LED3 toggle thread */
static void vLEDTask3(void *pvParameters) {
	bool LedState = false;

	while (1) {
		xSemaphoreTake(xMutex, portMAX_DELAY);

		Board_LED_Set(2, LedState);

		LedState = (bool) !LedState;


		vTaskDelay(configTICK_RATE_HZ  *5);
		xSemaphoreGive(xMutex);
	}
}

int main(void)
{
	prvSetupHardware();
	xMutex = xSemaphoreCreateMutex();  // creating mutex

	/* LED1 toggle thread */
	xTaskCreate(vLEDTask1, (signed char *) "vTaskLed1",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* LED2 toggle thread */
	xTaskCreate(vLEDTask2, (signed char *) "vTaskLed2",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* LED3 toggle thread */
	xTaskCreate(vLEDTask3, (signed char *) "vTaskLed3",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);


	/* Start  scheduler */
	vTaskStartScheduler();


	return 1;
}

