#include "tasks.h"




void LED_STATE_Task(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(LED_STATE_GPIO_Port, LED_STATE_Pin);
        vTaskDelay(pdMS_TO_TICKS(500));  // 1Hz frequency
    }
}

void LED_ERROR_Task(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin);
        vTaskDelay(pdMS_TO_TICKS(333));  // 3Hz frequency
    }
}

void Start_All_Tasks(void) {
	TaskHandle_t TaskHandle_LED_STATE;
	TaskHandle_t TaskHandle_LED_ERROR;

    xTaskCreate(LED_STATE_Task, "LED_STATE_Task", configMINIMAL_STACK_SIZE, NULL, 1, &TaskHandle_LED_STATE);
    xTaskCreate(LED_ERROR_Task, "LED_ERROR_Task", configMINIMAL_STACK_SIZE, NULL, 1, &TaskHandle_LED_ERROR);
}
