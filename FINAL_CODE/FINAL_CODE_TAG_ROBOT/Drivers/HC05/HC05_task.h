#ifndef INC_TASKS_HC05_H_
#define INC_TASKS_HC05_H_

#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "usart.h"
#include "gpio.h"

#define Q_PRINT_BT_LENGTH 100
#define Q_PRINT_BT_SIZE sizeof(uint8_t)

extern UART_HandleTypeDef huart1; //STlink
extern UART_HandleTypeDef huart3; //Bluetooth

void print_BT_Task(void *pvParameters);
void send_text(void *pvParameters);
void fill_queue(QueueHandle_t queue, const char* text);
void read_BT(void *pvParameters);
void renvoie(void *pvParameters);
void Start_All_Tasks(void);


#endif /* INC_TASKS_HC05_H_ */
