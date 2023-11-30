/*
 * tasks.h
 *
 *  Created on: Nov 30, 2023
 *      Author: Baptiste
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include <stdio.h>

#include "main.h"
#include "usart.h"
#include "gpio.h"

void LED_STATE_Task(void *pvParameters);
void LED_ERROR_Task(void *pvParameters);
void Start_All_Tasks(void);



#endif /* INC_TASKS_H_ */
