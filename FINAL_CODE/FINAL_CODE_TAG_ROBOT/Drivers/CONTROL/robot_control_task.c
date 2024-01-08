#include "robot_control_task.h"

TaskHandle_t TaskHandle_robot_ctrl;

extern h_lidar_t lidar;

robot_state_t robot;

int robot_control_init(int task_priority)
{
	BaseType_t xReturned;

	printf("Loading robot control task with priority %d\n\r",task_priority);
	xReturned = xTaskCreate(
			robot_control_Task,
			"rbt_ctrl_Task",
			500,
			NULL,
			task_priority,
			&TaskHandle_robot_ctrl);

	if(xReturned==pdPASS){
		printf("Task loaded\r\n");
		return 0;
	}
	else {
		printf("Error loading task\r\n");
		return 1;
	}
}
void robot_control_Task(void *unused)
{
	printf("Starting robot_control task\r\n");
	motor_t L_motor;
	motor_t R_motor;
	init_motors(&L_motor, &R_motor);


	standby_mode(&L_motor);
	standby_mode(&R_motor);

	for(;;)
	{
		switch(robot.mode){
		case IDLE:
			vTaskDelay(pdMS_TO_TICKS(DELAY_UNTIL_MOTOR_START));
			robot.type = STARTING_MODE;
			robot.mode = TRACKING;

		case TRACKING:
			if(robot.type == CAT)
			{
				hunt_enemy(&L_motor, &R_motor);
			}else{
				avoid_enemy(&L_motor, &R_motor);
			}
			break;

		case AVOID_EDGE:

			control_set_speed(-AVOID_BACKWARD_SPEED, &L_motor, &R_motor);

			vTaskDelay(pdMS_TO_TICKS(AVOID_BACKWARD_TIME));
			control_set_rotate(AVOID_ROTATION_SPEED,&L_motor, &R_motor);
			vTaskDelay(pdMS_TO_TICKS(AVOID_ROTATION_TIME));
			standby_mode(&L_motor);
			standby_mode(&R_motor);

			robot.mode = TRACKING;
			break;
		}
	}
}

void control_set_speed_line_rotate(int16_t command,motor_t *L_motor, motor_t *R_motor)
{
#ifdef USE_MOTORS
	int16_t speed_L, speed_R;

	speed_L = constrain(command+FORWARD_SPEED,MAX_SPEED_FWD,-MAX_SPEED_REV);//rotation calculation
	speed_R = -constrain(command+FORWARD_SPEED,MAX_SPEED_FWD,-MAX_SPEED_REV);//rotation calculation
	set_speed(speed_L, L_motor);
	set_speed(speed_R, R_motor);
#endif
}
void control_set_rotate(int16_t command,motor_t *L_motor, motor_t *R_motor)
{
#ifdef USE_MOTORS
	set_speed(command, L_motor);
	set_speed(command, R_motor);
#endif
}

void control_set_speed_line(int16_t command,motor_t *L_motor, motor_t *R_motor)
{
#ifdef USE_MOTORS
	int16_t speed;
	speed = constrain(command,MAX_SPEED_FWD,-MAX_SPEED_REV);
	set_speed(speed, L_motor);
	set_speed(-speed, R_motor);
#endif
}
void control_set_speed(int16_t command,motor_t *L_motor, motor_t *R_motor)
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
	if(value<min)
	{
		return min;
	}
	if(value>max)
	{
		return max;
	}
	return value;
}
int16_t get_oposite_angle_360(int16_t angle)
{
	angle+=180;
	if(angle>360)
	{
		return angle-360;
	}else{
		return angle;
	}
}

void hunt_enemy(motor_t *L_motor, motor_t *R_motor)
{
	//Get the target angle deviation
	uint16_t angle = lidar.target.angle;
	int16_t angle_diff;

	/* Angle dif :
	 * max +180
	 * min -180
	 * Map this to +100 -100 with an attenuation
	 */
	angle_diff = map_360_0_to_100_min100(angle);
	angle_diff /= ATTENUATION_FACTOR;
	control_set_speed_line_rotate(angle_diff,L_motor,R_motor);
}

int16_t map_360_0_to_100_min100(int16_t x)
{
	const int16_t in_max = 360;
	const int16_t out_min = -100;
	const int16_t out_max = 100;
	return x * (out_max - out_min) / in_max + out_min;
}
void avoid_enemy(motor_t *L_motor, motor_t *R_motor)
{
	uint16_t angle = get_oposite_angle_360(lidar.target.angle);
	int16_t angle_diff;
	angle_diff = map_360_0_to_100_min100(angle);
	angle_diff /= ATTENUATION_FACTOR;
	control_set_speed_line_rotate(angle_diff,L_motor,R_motor);
}
