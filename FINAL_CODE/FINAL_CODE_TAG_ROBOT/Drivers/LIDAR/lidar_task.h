/**
 * @file lidar_task.htmlinclude [block] file-name
 * @author @LirokWid
 * @brief 
 * @version 0.1
 * @date 2024-01-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef INC_LIDAR_TASK_H_
#define INC_LIDAR_TASK_H_

#include "../UART/transmit_uart.h"
#include "lidar_driver.h"

#include "cmsis_os.h"


#define LIDAR_POINTS_SENSIBILITY 360

//1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20, 24, 30, 36, 40, 45, 60, 72, 90, 120, 180 et 360
#define LIDAR_AVERAGE_DIRECTIONS_NB 60
#define POINTS_PER_AVERAGE (LIDAR_POINTS_SENSIBILITY/LIDAR_AVERAGE_DIRECTIONS_NB)
#define MOVING_AVERAGE_DEPTH 5

int Init_lidar(int task_priority, UART_HandleTypeDef *lidar_uart);
void Lidar_ISR_triggered_Task(void *pvParameters);
void Lidar_compute_average_Task(void *pvParameters);


#endif
