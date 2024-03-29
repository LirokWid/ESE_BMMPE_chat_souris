#include "sys_helper_task.h"

TaskHandle_t TaskHandle_alive;

TaskHandle_t TaskHandle_print_lidar_point;
extern h_lidar_t 			lidar;

int create_sys_task(int task_priority)
{
	BaseType_t xReturned;

	printf("Loading lidar printing task with priority %d\n\r",task_priority+1);
	xReturned = xTaskCreate(
			print_lidar_point,
			"print points",
			1000,
			NULL,
			task_priority,
			&TaskHandle_print_lidar_point);

    if(xReturned==pdPASS){
		printf("Task loaded\r\n");
	}else{
		printf("Error loading task\r\n");
		return 1;
	}
	return 0;
}

void is_system_alive(void *unused)
{
	printf("Starting is_system_alive task\r\n");
	for(;;){
		printf("Alive\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

#define USE_AVERAGE_POINTS
#define USE_POINTS
#define PRINT_LIDAR_2D_MAP
#define PRINT_TARGET_COORDINATES

void print_lidar_point(void *unused)
{
	printf("Starting print_lidar_point task\r\n");
	for(;;)
	{
#ifdef PRINT_LIDAR_2D_MAP
#ifdef USE_AVERAGE_POINTS
			for(int j=0;j<LIDAR_AVERAGE_DIRECTIONS_NB;j++)
			{
				printf("A%-3d:%5lu|",j,lidar.average[j]);
			}
			printf("\r\n\n\n");
#endif
#ifdef USE_POINTS
			for(int i=0;i<36;i++)
			{
				for(int j=0;j<10;j++)
				{
					printf("S%-3d:%5d|",(i*10)+j,lidar.points[(i*10)+j]);
				}
			printf("\r\n");
			}
			printf("\r\n\n\n");
#endif

#endif

#ifdef PRINT_TARGET_COORDINATES
		printf("TA:%5d|TD:%5d",lidar.target.angle,lidar.target.distance);
		printf("\r\n\n\n");
#endif
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

