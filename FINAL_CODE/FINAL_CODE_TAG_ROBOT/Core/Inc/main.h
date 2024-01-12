/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LIDAR_MOTOR_PWM_Pin GPIO_PIN_12
#define LIDAR_MOTOR_PWM_GPIO_Port GPIOC
#define LIDAR_MOTOR_ENABLE_Pin GPIO_PIN_13
#define LIDAR_MOTOR_ENABLE_GPIO_Port GPIOC
#define LIDAR_MESURE_ENABLE_Pin GPIO_PIN_14
#define LIDAR_MESURE_ENABLE_GPIO_Port GPIOC
#define MOTOR_1_FWD_Pin GPIO_PIN_1
#define MOTOR_1_FWD_GPIO_Port GPIOC
#define MOTOR_1_REV_Pin GPIO_PIN_2
#define MOTOR_1_REV_GPIO_Port GPIOC
#define LED_STATE_Pin GPIO_PIN_0
#define LED_STATE_GPIO_Port GPIOA
#define LED_ERROR_Pin GPIO_PIN_1
#define LED_ERROR_GPIO_Port GPIOA
#define ENCODER_2_A_Pin GPIO_PIN_6
#define ENCODER_2_A_GPIO_Port GPIOA
#define ENCODER_2_B_Pin GPIO_PIN_7
#define ENCODER_2_B_GPIO_Port GPIOA
#define BUMPER_3_Pin GPIO_PIN_13
#define BUMPER_3_GPIO_Port GPIOB
#define BUMPER_3_EXTI_IRQn EXTI4_15_IRQn
#define BUMPER_4_Pin GPIO_PIN_14
#define BUMPER_4_GPIO_Port GPIOB
#define BUMPER_4_EXTI_IRQn EXTI4_15_IRQn
#define ENCODER_1_B_Pin GPIO_PIN_8
#define ENCODER_1_B_GPIO_Port GPIOA
#define ENCODER_1_A_Pin GPIO_PIN_9
#define ENCODER_1_A_GPIO_Port GPIOA
#define BUMPER_2_Pin GPIO_PIN_6
#define BUMPER_2_GPIO_Port GPIOC
#define BUMPER_2_EXTI_IRQn EXTI4_15_IRQn
#define BUMPER_1_Pin GPIO_PIN_7
#define BUMPER_1_GPIO_Port GPIOC
#define BUMPER_1_EXTI_IRQn EXTI4_15_IRQn
#define BORDURE_1_Pin GPIO_PIN_8
#define BORDURE_1_GPIO_Port GPIOD
#define BORDURE_1_EXTI_IRQn EXTI4_15_IRQn
#define BORDURE_2_Pin GPIO_PIN_9
#define BORDURE_2_GPIO_Port GPIOD
#define BORDURE_2_EXTI_IRQn EXTI4_15_IRQn
#define BTN_1_Pin GPIO_PIN_10
#define BTN_1_GPIO_Port GPIOA
#define BTN_1_EXTI_IRQn EXTI4_15_IRQn
#define BTN_2_Pin GPIO_PIN_11
#define BTN_2_GPIO_Port GPIOA
#define BTN_2_EXTI_IRQn EXTI4_15_IRQn
#define BTN_3_Pin GPIO_PIN_12
#define BTN_3_GPIO_Port GPIOA
#define BTN_3_EXTI_IRQn EXTI4_15_IRQn
#define LED_3_Pin GPIO_PIN_0
#define LED_3_GPIO_Port GPIOD
#define LED_2_Pin GPIO_PIN_5
#define LED_2_GPIO_Port GPIOD
#define LED_1_Pin GPIO_PIN_6
#define LED_1_GPIO_Port GPIOD
#define MOTOR_2_REV_Pin GPIO_PIN_8
#define MOTOR_2_REV_GPIO_Port GPIOB
#define MOTOR_2_FWD_Pin GPIO_PIN_9
#define MOTOR_2_FWD_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
