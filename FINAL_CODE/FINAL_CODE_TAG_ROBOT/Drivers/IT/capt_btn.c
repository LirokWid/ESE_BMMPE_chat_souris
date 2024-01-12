/*
 * bordure_bumper.c
 *
 *  Created on: Dec 6, 2023
 *      Author: maxim
 */

#include "capt_btn.h"


uint16_t GPIO_Pin_mem;
struct CAPT_BTN_MAP_struct CAPT_BTN_MAP;
const struct CAPT_BTN_MAP_struct CAPT_BTN_MAP_RESET = {NONE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

TaskHandle_t TaskHandle_CAPT_BTN;
SemaphoreHandle_t sem_capt1 = NULL;

extern robot_state_t robot;


void capt_btn_task() {
	for	(;;) {

		xSemaphoreTake(sem_capt1, portMAX_DELAY);
		switch (GPIO_Pin_mem)
		{
			case BUMPER_1_Pin 	: CAPT_BTN_MAP.BUMPER_1  = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; 	break;
			case BUMPER_2_Pin 	: CAPT_BTN_MAP.BUMPER_2  = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; 	break;
			case BUMPER_3_Pin 	: CAPT_BTN_MAP.BUMPER_3  = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; 	break;
			case BUMPER_4_Pin 	: CAPT_BTN_MAP.BUMPER_4  = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; 	break;
			case BTN_1_Pin 		: CAPT_BTN_MAP.BTN_1     = TRUE; CAPT_BTN_MAP.Pressed = BTN; 		break;
			case BTN_2_Pin 		: CAPT_BTN_MAP.BTN_2     = TRUE; CAPT_BTN_MAP.Pressed = BTN; 		break;
			case BTN_3_Pin 		: CAPT_BTN_MAP.BTN_3     = TRUE; CAPT_BTN_MAP.Pressed = BTN; 		break;
			case BORDURE_1_Pin 	: CAPT_BTN_MAP.BORDURE_1 = TRUE; CAPT_BTN_MAP.Pressed = BORDURE; 	break;
			case BORDURE_2_Pin 	: CAPT_BTN_MAP.BORDURE_2 = TRUE; CAPT_BTN_MAP.Pressed = BORDURE; 	break;
			//case BORDURE_3_Pin : CAPT_BTN_MAP.BORDURE_3 = TRUE; break;
		}

#ifdef USE_BUTTONS

		if(CAPT_BTN_MAP.BTN_1)
		{//BTN1 pressed
			//Start or stop the motors after a delay
			if(robot.mode == IDLE)
			{
				vTaskDelay(pdMS_TO_TICKS(1000));
				robot.mode = TRACKING;
			}else{
				robot.mode = IDLE;
			}
		}
		if(CAPT_BTN_MAP.BTN_2)
		{//BTN2 pressed
			//Change mode from cat to mouse
		}
		if(CAPT_BTN_MAP.BTN_3)
		{//BTN3 pressed
			//Change mode from cat to mouse
			robot.type = (robot.type==CAT)?MOUSE:CAT;
		}
#endif

#ifdef USE_BORDERS
		if(CAPT_BTN_MAP.Pressed == BORDURE)
		{
			robot.mode = AVOID_EDGE;
		}
#endif

#ifdef USE_BUMPERS
		if(CAPT_BTN_MAP.Pressed == BUMPER)
		{
			robot.type=(robot.type==CAT)?MOUSE:CAT;
		}
#endif

		HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin); //See refresh rate on a led
		CAPT_BTN_MAP = CAPT_BTN_MAP_RESET;
	}
}

int capt_btn_init(int priority)
{
	sem_capt1 = xSemaphoreCreateBinary();
	BaseType_t xReturned;

	printf("Loading btn_ISR task with priority %d\n\r",priority);

	xReturned = xTaskCreate(
					capt_btn_task,
					"capt_btn_task",
					500,
					NULL,
					priority,
					&TaskHandle_CAPT_BTN);

	if(xReturned==pdPASS){
		printf("Task loaded\r\n");
		return 0;
	}
	else {
		printf("Error loading task\r\n");
		return 1;
	}
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	BaseType_t higher_priority_task_woken = pdFALSE;
	GPIO_Pin_mem = GPIO_Pin;
	//capt_btn_task();
	xSemaphoreGiveFromISR(sem_capt1, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	BaseType_t higher_priority_task_woken = pdFALSE;
	GPIO_Pin_mem = GPIO_Pin;
	//capt_btn_task();
	xSemaphoreGiveFromISR(sem_capt1, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

