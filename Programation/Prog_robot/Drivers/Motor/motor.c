/*
 * motor.c
 *
 *  Created on: Oct 19, 2023
 *      Author: vande
 */

#include "motor.h"
#include "stm32g0xx_hal_tim.h"

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

//tests
void test1(){
	for(;;){
		//move_on_curve_right(100,30);
		//vTaskDelay(2000);
		move_straight(90);
		vTaskDelay(2000);
		stop();
		vTaskDelay(2000);
	}
}

void Start_motor_Tasks(){
	TaskHandle_t TaskHandle_test1;
	xTaskCreate(test1,"test1",configMINIMAL_STACK_SIZE,NULL,1,&TaskHandle_test1);
}
// fonctions de bas niveau

void forward_mode(uint16_t speed,struct driver_motor_t motor)
{
	if (speed>MAX_SPEED){speed=MAX_SPEED;}
	uint16_t compare=speed*MAX_COMPTEUR/MAX_SPEED;
	__HAL_TIM_SET_COMPARE(motor.reverse_htim, motor.reverse_CHANNEL, 0);
	__HAL_TIM_SET_COMPARE(motor.forward_htim, motor.forward_CHANNEL, compare);
}

void reverse_mode(uint16_t speed,struct driver_motor_t motor)
{
	if (speed>MAX_SPEED){speed=MAX_SPEED;}
	uint16_t compare=speed*MAX_COMPTEUR/MAX_SPEED;
	__HAL_TIM_SET_COMPARE(motor.forward_htim, motor.forward_CHANNEL, 0);
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

void move_on_curve_left(int16_t radius,int16_t speed)
{
	uint16_t speed_R=speed+speed*WHEELBASE/(2*radius);
	uint16_t speed_L=speed-speed*WHEELBASE/(2*radius);

	set_speed(speed_R,right_motor);
	set_speed(speed_L,left_motor);
}

void move_on_curve_right(int16_t radius,int16_t speed)
{
	uint16_t speed_L=speed+speed*WHEELBASE/(2*radius);
	uint16_t speed_R=speed-speed*WHEELBASE/(2*radius);

	set_speed(speed_R,right_motor);
	set_speed(speed_L,left_motor);
}

void move_straight(int16_t speed)
{
	set_speed(speed,right_motor);
	set_speed(speed,left_motor);
}

void stop(void)
{
	set_speed(0,right_motor);
	set_speed(0,left_motor);
}
