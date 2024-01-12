/*
 * bordure_bumper.h
 *
 *  Created on: Dec 6, 2023
 *      Author: maxim
 */

#ifndef IT_CAPT_BTN_H_
#define IT_CAPT_BTN_H_

#include <main.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "../CONTROL/robot_control_task.h"
#include "../PARAMS/params.h"

enum BOOL {FALSE, TRUE};
enum BTN_TYPE {NONE, BUMPER, BTN, BORDURE};

struct CAPT_BTN_MAP_struct {
	enum BTN_TYPE Pressed;
	enum BOOL BUMPER_1;
	enum BOOL BUMPER_2;
	enum BOOL BUMPER_3;
	enum BOOL BUMPER_4;
	enum BOOL BTN_1;
	enum BOOL BTN_2;
	enum BOOL BTN_3;
	enum BOOL BORDURE_1;
	enum BOOL BORDURE_2;
	//enum BOOL BORDURE_3;
};
void capt_btn_task();
int capt_btn_init(int priority);
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin);


#endif /* IT_CAPT_BTN_H_ */
