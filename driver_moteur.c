/*
 * driver_moteur.c
 *
 *  Created on: Oct 19, 2023
 *      Author: vande
 */

#include "driver_moteur.h"
#include "stm32g0xx_hal_tim.h"
#include "main.h"

uint8_t max_rot_speed = 245;
uint8_t max_angle = 180;

float lidar_angle = 0.0;

struct driver_motor_t left_motor={
		.forward_htim=&htim15,
		.forward_CHANNEL=TIM_CHANNEL_1,
		.reverse_htim=&htim15,
		.reverse_CHANNEL=TIM_CHANNEL_2
};
struct driver_motor_t right_motor={
		.forward_htim=&htim17,
		.forward_CHANNEL=TIM_CHANNEL_1,
		.reverse_htim=&htim16,
		.reverse_CHANNEL=TIM_CHANNEL_1
};

// fonctions de bas niveau

void forward_mode(uint16_t speed,struct driver_motor_t motor)
{
	uint16_t compare=speed*MAX_COMPTEUR/MAX_SPEED;
	__HAL_TIM_SET_COMPARE(motor.reverse_htim, motor.reverse_CHANNEL, MAX_COMPTEUR);
	__HAL_TIM_SET_COMPARE(motor.forward_htim, motor.forward_CHANNEL, compare);
}

void reverse_mode(uint16_t speed,struct driver_motor_t motor)
{
	uint16_t compare=speed*MAX_COMPTEUR/MAX_SPEED;
	__HAL_TIM_SET_COMPARE(motor.forward_htim, motor.forward_CHANNEL, MAX_COMPTEUR);
	__HAL_TIM_SET_COMPARE(motor.reverse_htim, motor.reverse_CHANNEL, compare);
}

void standBy_mode(struct driver_motor_t motor)
{
	__HAL_TIM_SET_COMPARE(motor.reverse_htim, motor.reverse_CHANNEL, 0);
	__HAL_TIM_SET_COMPARE(motor.forward_htim, motor.forward_CHANNEL, 0);
}

void brake_mode(struct driver_motor_t motor)
{
	__HAL_TIM_SET_COMPARE(motor.reverse_htim, motor.reverse_CHANNEL, MAX_COMPTEUR);
	__HAL_TIM_SET_COMPARE(motor.forward_htim, motor.forward_CHANNEL, MAX_COMPTEUR);
}

void set_speed(uint16_t speed,struct driver_motor_t motor){
	if(speed==0){brake_mode(motor);}
	else if(speed>0){forward_mode(speed,motor);}
	else {reverse_mode(-speed,motor);}
}

//fonctions plus haut niveau

void stop(void)
{
	set_speed(0,right_motor);
	set_speed(0,left_motor);
}

void speed_command(void) {
	float delta_m = (lidar_angle-max_angle)/max_angle*max_rot_speed;
	set_speed(delta_m,right_motor);
	set_speed(-delta_m,left_motor);
}

void speed_command_test(void) {
	for (int i = 0; i <= 10; i++) {
		float delta_m = (18*i - max_angle) / max_angle * max_rot_speed;
		set_speed(delta_m, right_motor);
		set_speed(-delta_m, left_motor);
		printf("Commande en vitesse : %.2f rpm\r\n");
		HAL_Delay(1000);
	}
}
