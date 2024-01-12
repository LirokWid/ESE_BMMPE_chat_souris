#ifndef ROBOT_CONTROL_TASK_H
#define ROBOT_CONTROL_TASK_H

#include <stdlib.h>

#include "../MOTOR/motor_driver.h"
#include "../UART/printf_uart.h"
#include "../LIDAR/lidar_driver.h"
#include "cmsis_os.h"


#define PRINTF_DEBUG

#define USE_MOTORS
//#define HOLD_POSITION

#define STARTING_MODE CAT
#define DELAY_UNTIL_MOTOR_START 3000

#define MAX_SPEED_FWD 70
#define MAX_SPEED_REV 60

#ifdef HOLD_POSITION
#define FORWARD_SPEED 0
#else
#define FORWARD_SPEED 45
#endif

#define MAX_ROTATION_SPEED 40

#define AVOID_BACKWARD_SPEED 40
#define AVOID_BACKWARD_TIME  400
#define AVOID_ROTATION_SPEED 70
#define AVOID_ROTATION_TIME  700


#define time_ms_backward 400

typedef enum type {
	CAT,
	MOUSE
}type_t;

typedef enum robot_mode{
	IDLE,
	TRACKING,
	AVOID_EDGE
}robot_mode_t;


typedef struct robot_state{
	type_t 	 	type;
	robot_mode_t mode;
}robot_state_t;


int16_t constrain(int16_t value, int8_t max, int8_t min);
int16_t map_360_0_to_100_min100(int16_t x);
int16_t get_oposite_angle_360(int16_t angle);

int robot_control_init(int task_priority);
void robot_control_Task(void *unused);
void hunt_enemy(motor_t *L_motor, motor_t *R_motor);
uint16_t attenuate(uint16_t command);
void avoid_enemy(motor_t *L_motor, motor_t *R_motor);
void control_set_rotate(int16_t command,motor_t *L_motor, motor_t *R_motor);
void control_set_speed_line_rotate(int16_t command,motor_t *L_motor, motor_t *R_motor);
void control_set_speed_line(int16_t command,motor_t *L_motor, motor_t *R_motor);
void control_set_speed(int16_t command,motor_t *L_motor, motor_t *R_motor);

#endif
