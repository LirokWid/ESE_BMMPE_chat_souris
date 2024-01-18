#include "mpu6050_task.h"

TaskHandle_t 		TaskHandle_MPU6050;
I2C_HandleTypeDef * MPU_i2c_handle;
MPU6050_t 			MPU6050_data;
SemaphoreHandle_t 	mpu_semaphore;

int Init_mpu6050(int priority,I2C_HandleTypeDef *MPU_i2c_handle_param)
{
	MPU_i2c_handle = MPU_i2c_handle_param;

	//Starting by waking up the MPU by writing corresponding registers
	while (MPU6050_Init(MPU_i2c_handle) == 1);

	//Create a mutex to protect the i2c bus when using it
	mpu_semaphore = xSemaphoreCreateMutex();

	//Then create the main task
	BaseType_t xReturned =	xTaskCreate(
								Mpu6050_Task,
								"accel_gyro_Task",
								configMINIMAL_STACK_SIZE,
								NULL,
								priority,
								&TaskHandle_MPU6050
							);

	return xReturned;
}

void Mpu6050_Task(void *pvParameters){

	while (1){
		if(xSemaphoreTake(mpu_semaphore,10000)==pdTRUE){
			MPU6050_Read_All(MPU_i2c_handle,&MPU6050_data);
			xSemaphoreGive(mpu_semaphore);
			HAL_GPIO_TogglePin(LED_STATE_GPIO_Port, LED_STATE_Pin);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}
