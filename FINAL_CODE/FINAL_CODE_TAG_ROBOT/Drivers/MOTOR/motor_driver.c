/*
 * driver_moteur.c
 *
 *  Created on: Oct 19, 2023
 *      Author: vande
 */


#include "motor_driver.h"

extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;

uint8_t lidar_angle = 0;

void init_motors(motor_t *L_motor, motor_t *R_motor)
{
	L_motor->forward_htim 		= &htim15;
	L_motor->reverse_htim		= &htim15;
	L_motor->forward_CHANNEL 	= TIM_CHANNEL_1;
	L_motor->reverse_CHANNEL	= TIM_CHANNEL_2;

	R_motor->forward_htim 		= &htim17;
	R_motor->reverse_htim		= &htim16;
	R_motor->forward_CHANNEL 	= TIM_CHANNEL_1;
	R_motor->reverse_CHANNEL	= TIM_CHANNEL_1;

	HAL_TIM_PWM_Start(L_motor->forward_htim,L_motor->forward_CHANNEL);
	HAL_TIM_PWM_Start(L_motor->reverse_htim,L_motor->reverse_CHANNEL);
	HAL_TIM_PWM_Start(R_motor->forward_htim,R_motor->forward_CHANNEL);
	HAL_TIM_PWM_Start(R_motor->reverse_htim,R_motor->reverse_CHANNEL);
}

// fonctions de bas niveau
void forward_mode(uint16_t speed, motor_t *motor)
{
	uint8_t compare = speed;
	__HAL_TIM_SET_COMPARE(
			motor->reverse_htim,
			motor->reverse_CHANNEL,
			0);
	__HAL_TIM_SET_COMPARE(
			motor->forward_htim,
			motor->forward_CHANNEL,
			compare);
}

void reverse_mode(uint16_t speed, motor_t *motor)
{
	uint8_t compare = speed;
	__HAL_TIM_SET_COMPARE(
			motor->reverse_htim,
			motor->reverse_CHANNEL,
			compare);
	__HAL_TIM_SET_COMPARE(
			motor->forward_htim,
			motor->forward_CHANNEL,
			0);
}

void standby_mode(motor_t *motor)
{
	__HAL_TIM_SET_COMPARE(motor->reverse_htim, motor->reverse_CHANNEL, 0);
	__HAL_TIM_SET_COMPARE(motor->forward_htim, motor->forward_CHANNEL, 0);
}
void brake_mode(motor_t *motor)
{
	__HAL_TIM_SET_COMPARE(
			motor->reverse_htim,
			motor->reverse_CHANNEL,
			ARR_VALUE);
	__HAL_TIM_SET_COMPARE(
			motor->forward_htim,
			motor->forward_CHANNEL,
			ARR_VALUE);
}

void set_speed(int16_t speed, motor_t *motor)
{
	if (speed == 0) {
		brake_mode(motor);
	} else if (speed > 0) {
		forward_mode(speed, motor);
	} else {
		reverse_mode(-speed, motor);
	}
}

