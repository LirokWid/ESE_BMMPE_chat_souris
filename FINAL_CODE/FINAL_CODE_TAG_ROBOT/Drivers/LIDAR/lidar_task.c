#include "lidar_task.h"

TaskHandle_t 		TaskHandle_LIDAR;
SemaphoreHandle_t 	lidar_half_DMA_semaphore = NULL;
SemaphoreHandle_t   lidar_full_DMA_semaphore = NULL;

SemaphoreHandle_t   find_target_semaphore = NULL;


UART_HandleTypeDef *lidar_uart_handle;
h_lidar_t 			lidar;


int Init_lidar(int task_priority, UART_HandleTypeDef *lidar_uart)
{
	BaseType_t xReturned;

	lidar_init_driver(&lidar, lidar_uart);
	printf("Loading lidar task with priority %d\n\r",task_priority);

	lidar_half_DMA_semaphore = xSemaphoreCreateBinary();
	lidar_full_DMA_semaphore = xSemaphoreCreateBinary();

	xReturned = xTaskCreate(
			Lidar_ISR_triggered_Task,
			"Lidar_ISR_trig",
			800,
			NULL,
			task_priority,
			&TaskHandle_LIDAR);

    if(xReturned==pdPASS){
		printf("Task loaded\r\n");
	}else{
		printf("Error loading task\r\n");
	}

	printf("Loading Lidar_find_target task with priority %d\n\r",task_priority-1);

	find_target_semaphore = xSemaphoreCreateBinary();

	xReturned = xTaskCreate(
			Lidar_compute_average_Task,
			"Lidar_find_target",
			1000,
			NULL,
			task_priority-1,
			&TaskHandle_LIDAR);

    if(xReturned==pdPASS){
		printf("Task loaded\r\n");
	}else{
		printf("Error loading task\r\n");
	}
	return 0;

}

void Lidar_compute_average_Task(void *unused)
{
	printf("Starting Lidar_find_target task\n\r");
	uint32_t circular_buffer[MOVING_AVERAGE_DEPTH][LIDAR_AVERAGE_DIRECTIONS_NB];
	uint8_t buffer_index = 0;
	uint8_t buffer_index_min1 = 0;


	for(;;)
	{
		//Average the 2d map over x times,
		xSemaphoreTake(find_target_semaphore,portMAX_DELAY);
		// Copy the latest Lidar values to the circular buffer it is different than the last one
		for (int i = 0; i < LIDAR_AVERAGE_DIRECTIONS_NB; ++i)
		{
			circular_buffer[buffer_index][i]=0;
			for(int j=0; j< POINTS_PER_AVERAGE;j++)
			{
				if(
(circular_buffer[buffer_index_min1][i]==lidar.points[(i*POINTS_PER_AVERAGE)+j]) ||
(lidar.points[(i*POINTS_PER_AVERAGE)+j]<10)											   )
				{
					circular_buffer[buffer_index][i] += 2500;
				}else{
					circular_buffer[buffer_index][i] += lidar.points[(i*POINTS_PER_AVERAGE)+j];
				}
			}
			circular_buffer[buffer_index][i] /= POINTS_PER_AVERAGE;
		}
		// Update the buffer index for the next iteration
		buffer_index = (buffer_index + 1) % MOVING_AVERAGE_DEPTH;
		if(buffer_index==0){
			buffer_index_min1 = MOVING_AVERAGE_DEPTH;
		}else{
			buffer_index_min1++;
		}

		uint16_t min_dist=25000;
		uint16_t min_angle=0;
		// Calculate the moving average and find the closest point
		for (int i = 0; i < LIDAR_AVERAGE_DIRECTIONS_NB; ++i)
		{
			lidar.average[i] = 0;
			for (int j = 0; j < MOVING_AVERAGE_DEPTH; ++j)
			{
				lidar.average[i] += circular_buffer[j][i];
			}
			lidar.average[i] /= MOVING_AVERAGE_DEPTH;
			if(min_dist>lidar.average[i])
			{
				min_dist = lidar.average[i];
				min_angle= i*POINTS_PER_AVERAGE;
			}
		}
		lidar.target.distance = min_dist;
		lidar.target.angle = min_angle;
	}
}


void Lidar_ISR_triggered_Task(void *unused)
{
	printf("Starting lidar ISR task\n\r");
	for(;;){
		//If the DMA triggers a mid or full transfer callback, launch the lidar data parsing function
		xSemaphoreTake(lidar_half_DMA_semaphore,portMAX_DELAY);
		lidar_parse(&lidar);
 	}
}


