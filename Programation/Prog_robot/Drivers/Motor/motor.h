/*
 * motor.h
 *
 *  Created on: Oct 19, 2023
 *      Author: vande
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include <stdint.h>
#include "stm32g0xx_hal.h"
#include "cmsis_os.h"

#define WHEELBASE 163 //in mm
#define WHEEL_DIAMETER 40 //in mm
#define MAX_COMPTEUR 127
#define MAX_SPEED 100 // tour/s //à confirmé


extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;

typedef struct driver_motor_t{
	TIM_HandleTypeDef *forward_htim;
	int forward_CHANNEL;
	TIM_HandleTypeDef *reverse_htim;
	int reverse_CHANNEL;
}driver_motor_t;

void forward_mode(uint16_t speed,struct driver_motor_t motor);
void reverse_mode(uint16_t speed,struct driver_motor_t motor);
void standBy_mode(struct driver_motor_t motor);
void brake_mode(struct driver_motor_t motor);
void set_speed(uint16_t speed, struct driver_motor_t motor);

void move_on_curve_left(int16_t radius,int16_t speed);
void move_on_curve_right(int16_t radius,int16_t speed);
void move_straight(int16_t speed);
void stop(void);

void test1(void);
void test2 (void);

#endif /* MOTOR_MOTOR_H_ */
