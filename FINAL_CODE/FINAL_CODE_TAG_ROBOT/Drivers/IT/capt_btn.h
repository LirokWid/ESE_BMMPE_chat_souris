/**
 * @file bordure_bumper.h
 * @brief Header file for bordure bumper task.
 */

#ifndef IT_CAPT_BTN_H_
#define IT_CAPT_BTN_H_

#include <main.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "../CONTROL/robot_control_task.h"
#include "../PARAMS/params.h"

/**
 * @enum BOOL
 * @brief Enumeration for boolean values.
 */
enum BOOL {
    FALSE, /**< False value. */
    TRUE   /**< True value. */
};

/**
 * @enum BTN_TYPE
 * @brief Enumeration for button types.
 */
enum BTN_TYPE {
    NONE,    /**< No button pressed. */
    BUMPER,  /**< Bumper button pressed. */
    BTN,     /**< General button pressed. */
    BORDURE  /**< Bordure button pressed. */
};

/**
 * @struct CAPT_BTN_MAP_struct
 * @brief Structure to map button states.
 */
struct CAPT_BTN_MAP_struct {
    enum BTN_TYPE Pressed;  /**< Pressed button type. */
    enum BOOL BUMPER_1;     /**< Bumper 1 state. */
    enum BOOL BUMPER_2;     /**< Bumper 2 state. */
    enum BOOL BUMPER_3;     /**< Bumper 3 state. */
    enum BOOL BUMPER_4;     /**< Bumper 4 state. */
    enum BOOL BTN_1;        /**< Button 1 state. */
    enum BOOL BTN_2;        /**< Button 2 state. */
    enum BOOL BTN_3;        /**< Button 3 state. */
    enum BOOL BORDURE_1;    /**< Bordure 1 state. */
    enum BOOL BORDURE_2;    /**< Bordure 2 state. */
    // enum BOOL BORDURE_3; /**< Bordure 3 state. */
};

/**
 * @brief CAPT_BTN task function.
 */
void capt_btn_task();

/**
 * @brief Initialize the capt_btn task.
 * @param priority Priority of the task.
 * @return 0 if successful, 1 if there is an error.
 */
int capt_btn_init(int priority);

/**
 * @brief Callback function for EXTI rising edge.
 * @param GPIO_Pin The GPIO pin that triggered the interrupt.
 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

/**
 * @brief Callback function for EXTI falling edge.
 * @param GPIO_Pin The GPIO pin that triggered the interrupt.
 */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin);

#endif /* IT_CAPT_BTN_H_ */
