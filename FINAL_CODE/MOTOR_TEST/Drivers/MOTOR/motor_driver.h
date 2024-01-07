/*
 * driver_moteur.h
 *
 *  Created on: Oct 19, 2023
 *      Author: vande
 */

#ifndef INC_DRIVER_MOTEUR_H_
#define INC_DRIVER_MOTEUR_H_

#include "main.h"
#include <stdint.h>
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_tim.h"

#define WHEELBASE 163 //in mm
#define WHEEL_DIAMETER 40 //in mm
#define MAX_COMPTEUR 4000
#define MAX_SPEED 5000 // tour/s
#define MAX_ANGLE 180
#define MAX_ROT_SPEED 245



typedef struct motor{
	TIM_HandleTypeDef *forward_htim;
	int forward_CHANNEL;
	TIM_HandleTypeDef *reverse_htim;
	int reverse_CHANNEL;
}motor_t;

void init_motors(motor_t *L_motor, motor_t *R_motor);
void forward_mode(uint16_t speed,motor_t *motor);
void reverse_mode(uint16_t speed,motor_t *motor);
void standby_mode(motor_t *motor);
void brake_mode(motor_t *motor);
void set_speed(uint16_t speed, motor_t *motor);

void move_on_curve_left(int16_t radius,int16_t speed);
void move_on_curve_right(int16_t radius,int16_t speed);
void move_straight(int16_t speed);
void stop(void);

void speed_command(void);

void speed_command_test(void);

#endif /* INC_DRIVER_MOTEUR_H_ */
