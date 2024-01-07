#ifndef ROBOT_CONTROL_TASK_H
#define ROBOT_CONTROL_TASK_H


#include "../MOTOR/motor_driver.h"
#include "../UART/printf_uart.h"
#include "../LIDAR/lidar_driver.h"
#include "cmsis_os.h"


#define PRINTF_DEBUG

typedef enum robot_state{
	IDLE,
	CAT,
	MOUSE,
	AVOID_EDGE,

}robot_state_t;

int robot_control_init(int task_priority);
void robot_control_Task(void *unused);
void hunt_enemy();

#endif
