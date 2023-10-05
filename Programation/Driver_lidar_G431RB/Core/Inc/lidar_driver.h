/*
 * lidar_driver.h
 *
 *  Created on: Oct 5, 2023
 *      Author: Baptiste
 */

#ifndef INC_LIDAR_DRIVER_H_
#define INC_LIDAR_DRIVER_H_

#define MOTOR_ENABLE 	PC1
#define MOTOR_PWM		PC0
#define LIDAR_TX		PC4
#define LIDAR_RX		PC5
#define MESURE_ENABLE	PC2

#include "gpio.h"
#include "main.h"

int i = LIDAR_MESURE_ENABLE_GPIO_Port;

int turn_on_motor();







#endif /* INC_LIDAR_DRIVER_H_ */
