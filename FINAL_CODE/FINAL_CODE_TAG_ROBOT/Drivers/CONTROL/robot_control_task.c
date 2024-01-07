#include "robot_control_task.h"

TaskHandle_t TaskHandle_robot_ctrl;

extern h_lidar_t lidar;


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
	robot_state_t state = CAT;
	motor_t L_motor;
	motor_t R_motor;
	init_motors(&L_motor, &R_motor);
/*
	for(int i = 0;i<100;i++)
	{
		forward_mode(i, &L_motor);
		vTaskDelay(50);
	}

	for(int i = 0;i<100;i++)
	{
		forward_mode(i, &R_motor);
		vTaskDelay(50);
	}
	*/

	vTaskDelay(2000);

	standby_mode(&L_motor);
	standby_mode(&R_motor);

	//vTaskDelete(0);

	for(;;)
	{
		switch(state){
		case IDLE:
			//Wait for a button press to start tracking
			break;
		case CAT:
			//hunt_enemy();
			uint16_t angle = lidar.target.angle;

			int16_t angle_diff;

			if(angle<180)
			{
				angle_diff = angle;
			}else{
				angle_diff = -(angle-180);
			}
			angle_diff/=2;
			set_speed(constrain(angle_diff,-35,35), &L_motor);
			set_speed(constrain(-angle_diff,-35,35), &R_motor);
			break;
		case MOUSE:
			break;
		case AVOID_EDGE:
			break;
		}
		//vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

int16_t constrain(int16_t value, int8_t min, int8_t max)
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

void hunt_enemy()
{
	//Get the target angle deviation

}
