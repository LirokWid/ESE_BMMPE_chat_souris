/**
 * @file robot_control_task.h
 * @brief Header file for robot control task.
 */

#ifndef ROBOT_CONTROL_TASK_H
#define ROBOT_CONTROL_TASK_H

#include <stdlib.h>

#include "../MOTOR/motor_driver.h"
#include "../UART/printf_uart.h"
#include "../LIDAR/lidar_driver.h"
#include "cmsis_os.h"

/**
 * @def PRINTF_DEBUG
 * @brief Debug flag for printf.
 */
#define PRINTF_DEBUG

/**
 * @def USE_MOTORS
 * @brief Enable/disable motor control.
 */
#define USE_MOTORS
//#define HOLD_POSITION

/**
 * @def STARTING_MODE
 * @brief Default starting mode for the robot.
 */
#define STARTING_MODE CAT

/**
 * @def DELAY_UNTIL_MOTOR_START
 * @brief Delay until motor start in milliseconds.
 */
#define DELAY_UNTIL_MOTOR_START 3000

/**
 * @def MAX_SPEED_FWD
 * @brief Maximum forward speed for the robot.
 */
#define MAX_SPEED_FWD 75

/**
 * @def MAX_SPEED_REV
 * @brief Maximum reverse speed for the robot.
 */
#define MAX_SPEED_REV 60

#ifdef HOLD_POSITION
#define FORWARD_SPEED 0
#else
#define FORWARD_SPEED 60
#endif

/**
 * @def MAX_ROTATION_SPEED
 * @brief Maximum rotation speed for the robot.
 */
#define MAX_ROTATION_SPEED 40

/**
 * @def AVOID_BACKWARD_SPEED
 * @brief Speed used during backward movement for obstacle avoidance.
 */
#define AVOID_BACKWARD_SPEED 40

/**
 * @def AVOID_BACKWARD_TIME
 * @brief Time for backward movement during obstacle avoidance in milliseconds.
 */
#define AVOID_BACKWARD_TIME  400

/**
 * @def AVOID_ROTATION_SPEED
 * @brief Rotation speed during obstacle avoidance.
 */
#define AVOID_ROTATION_SPEED 70

/**
 * @def AVOID_ROTATION_TIME
 * @brief Time for rotation during obstacle avoidance in milliseconds.
 */
#define AVOID_ROTATION_TIME  700

/**
 * @def time_ms_backward
 * @brief Time for backward movement in milliseconds.
 */
#define time_ms_backward 400

/**
 * @enum type_t
 * @brief Enumeration for robot types.
 */
typedef enum type {
    CAT,  /**< Cat type. */
    MOUSE /**< Mouse type. */
} type_t;

/**
 * @enum robot_mode_t
 * @brief Enumeration for robot modes.
 */
typedef enum robot_mode {
    IDLE,          /**< Idle mode. */
    TRACKING,      /**< Tracking mode. */
    AVOID_EDGE,    /**< Avoid edge mode. */
    PUSH_BACK      /**< Push back mode. */
} robot_mode_t;

/**
 * @struct robot_state_t
 * @brief Structure representing the state of the robot.
 */
typedef struct robot_state {
    type_t        type; /**< Type of the robot. */
    robot_mode_t  mode; /**< Mode of the robot. */
} robot_state_t;

/**
 * @brief Constrain a value within a specified range.
 * @param value The value to be constrained.
 * @param max The maximum allowed value.
 * @param min The minimum allowed value.
 * @return The constrained value.
 */
int16_t constrain(int16_t value, int8_t max, int8_t min);

/**
 * @brief Map a 360-degree angle to a range from 0 to 100, with a minimum of -100.
 * @param x The input angle.
 * @return The mapped value.
 */
int16_t map_360_0_to_100_min100(int16_t x);

/**
 * @brief Get the opposite angle in the 360-degree range.
 * @param angle The input angle.
 * @return The opposite angle.
 */
int16_t get_oposite_angle_360(int16_t angle);

/**
 * @brief Initialize the robot control task.
 * @param task_priority Priority of the task.
 * @return 0 if successful, 1 if there is an error.
 */
int robot_control_init(int task_priority);

/**
 * @brief Robot control task function.
 * @param unused Unused parameter.
 */
void robot_control_Task(void *unused);

/**
 * @brief Hunt the enemy based on LIDAR readings.
 * @param L_motor Pointer to the left motor.
 * @param R_motor Pointer to the right motor.
 */
void hunt_enemy(motor_t *L_motor, motor_t *R_motor);

/**
 * @brief Attenuate the command based on angle difference.
 * @param command The command to be attenuated.
 * @return The attenuated command.
 */
uint16_t attenuate(uint16_t command);

/**
 * @brief Avoid the enemy based on LIDAR readings.
 * @param L_motor Pointer to the left motor.
 * @param R_motor Pointer to the right motor.
 */
void avoid_enemy(motor_t *L_motor, motor_t *R_motor);

/**
 * @brief Set the robot to rotate at a specified speed.
 * @param command The rotation speed command.
 * @param L_motor Pointer to the left motor.
 * @param R_motor Pointer to the right motor.
 */
void control_set_rotate(int16_t command, motor_t *L_motor, motor_t *R_motor);

/**
 * @brief Set the robot to move with speed and rotation.
 * @param command The movement command.
 * @param L_motor Pointer to the left motor.
 * @param R_motor Pointer to the right motor.
 */
void control_set_speed_line_rotate(int16_t command, motor_t *L_motor, motor_t *R_motor);

/**
 * @brief Set the robot to move in a straight line with a specified speed.
 * @param command The movement command.
 * @param L_motor Pointer to the left motor.
 * @param R_motor Pointer to the right motor.
 */
void control_set_speed_line(int16_t command, motor_t *L_motor, motor_t *R_motor);

/**
 * @brief Set the robot to move with a specified speed.
 * @param command The movement command.
 * @param L_motor Pointer to the left motor.
 * @param R_motor Pointer to the right motor.
 */
void control_set_speed(int16_t command, motor_t *L_motor, motor_t *R_motor);

#endif
