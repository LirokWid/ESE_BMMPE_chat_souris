/**
 * @file robot_control_task.c
 * @brief Implementation file for robot control task.
 */

#include "robot_control_task.h"

TaskHandle_t TaskHandle_robot_ctrl;

extern h_lidar_t lidar;

robot_state_t robot;

int robot_control_init(int task_priority)
{
    BaseType_t xReturned;

    printf("Loading robot control task with priority %d\n\r", task_priority);
    xReturned = xTaskCreate(
        robot_control_Task,
        "rbt_ctrl_Task",
        500,
        NULL,
        task_priority,
        &TaskHandle_robot_ctrl);

    if (xReturned == pdPASS) {
        printf("Task loaded\r\n");
        return 0;
    } else {
        printf("Error loading task\r\n");
        return 1;
    }
}

void robot_control_Task(void *unused)
{
    printf("Starting robot_control task\r\n");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (!lidar.is_sending)
	{//Reset the ucontroler until the lidar start
        HAL_NVIC_SystemReset();
    }

    motor_t L_motor;
    motor_t R_motor;
    init_motors(&L_motor, &R_motor);

    robot.mode = IDLE;
    robot.type = STARTING_MODE;

    for (;;) {
        HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin); // See refresh rate on a led

        if (robot.type == MOUSE) {
            HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, SET);
        } else {
            HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, RESET);
        }

        switch (robot.mode) {
        case IDLE:
            standby_mode(&L_motor);
            standby_mode(&R_motor);
            break;
        case TRACKING:
            if (robot.type == CAT) {
                hunt_enemy(&L_motor, &R_motor);
            } else {
                avoid_enemy(&L_motor, &R_motor);
            }
            break;
        case AVOID_EDGE:
            control_set_speed(-AVOID_BACKWARD_SPEED, &L_motor, &R_motor);
            vTaskDelay(pdMS_TO_TICKS(AVOID_BACKWARD_TIME));
            control_set_rotate(AVOID_ROTATION_SPEED, &L_motor, &R_motor);
            vTaskDelay(pdMS_TO_TICKS(AVOID_ROTATION_TIME));
            standby_mode(&L_motor);
            standby_mode(&R_motor);

            robot.mode = TRACKING;
            break;
        case PUSH_BACK:
            control_set_speed(-AVOID_BACKWARD_SPEED, &L_motor, &R_motor);
            vTaskDelay(pdMS_TO_TICKS(AVOID_BACKWARD_TIME));
            standby_mode(&L_motor);
            standby_mode(&R_motor);
            vTaskDelay(pdMS_TO_TICKS(1000));
            robot.mode = TRACKING;
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // Run at 20 Hz
    }
}

void control_set_speed_line_rotate(int16_t command, motor_t *L_motor, motor_t *R_motor)
{
#ifdef USE_MOTORS
    int16_t speed_L, speed_R;

    speed_L = constrain(command + FORWARD_SPEED, MAX_SPEED_FWD, -MAX_SPEED_REV);
    speed_R = constrain(-command + FORWARD_SPEED, MAX_SPEED_FWD, -MAX_SPEED_REV);
    set_speed(speed_L, L_motor);
    set_speed(speed_R, R_motor);
#endif
}

void control_set_rotate(int16_t command, motor_t *L_motor, motor_t *R_motor)
{
#ifdef USE_MOTORS
    set_speed(command, L_motor);
    set_speed(-command, R_motor);
#endif
}

void control_set_speed_line(int16_t command, motor_t *L_motor, motor_t *R_motor)
{
#ifdef USE_MOTORS
    int16_t speed;
    speed = constrain(command, MAX_SPEED_FWD, -MAX_SPEED_REV);
    set_speed(speed, L_motor);
    set_speed(-speed, R_motor);
#endif
}

void control_set_speed(int16_t command, motor_t *L_motor, motor_t *R_motor)
{
#ifdef USE_MOTORS
    int16_t speed;
    speed = command;
    set_speed(speed, L_motor);
    set_speed(speed, R_motor);
#endif
}

int16_t constrain(int16_t value, int8_t max, int8_t min)
{
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

int16_t get_oposite_angle_360(int16_t angle)
{
    angle += 180;
    if (angle > 360) {
        return angle - 360;
    } else {
        return angle;
    }
}

void hunt_enemy(motor_t *L_motor, motor_t *R_motor)
{
    uint16_t angle = lidar.target.angle;
    int16_t angle_diff;

    angle_diff = map_360_0_to_100_min100(angle);
    int16_t command = attenuate(angle_diff);
    control_set_speed_line_rotate(constrain(command, MAX_ROTATION_SPEED, -MAX_ROTATION_SPEED), L_motor, R_motor);
}

uint16_t attenuate(uint16_t command)
{
    if (abs(command) < 90 && abs(command) > 45) {
        return (command * 3) / 2;
    }
    if (abs(command) <= 45) {
        return command / 2;
    }
    return command;
}

void avoid_enemy(motor_t *L_motor, motor_t *R_motor)
{
    uint16_t angle = get_oposite_angle_360(lidar.target.angle);
    int16_t angle_diff;
    angle_diff = map_360_0_to_100_min100(angle);
    int16_t command = attenuate(angle_diff);
    control_set_speed_line_rotate(constrain(command, MAX_ROTATION_SPEED, -MAX_ROTATION_SPEED), L_motor, R_motor);
}

int16_t map_360_0_to_100_min100(int16_t x)
{
    int16_t temp_val;
    if (x >= 0 && x < 180) {
        temp_val = x;
    } else {
        temp_val = x - 360;
    }

    const int16_t in_min = -180;
    const int16_t in_max = 180;
    const int16_t out_min = -100;
    const int16_t out_max = 100;

    return (temp_val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
