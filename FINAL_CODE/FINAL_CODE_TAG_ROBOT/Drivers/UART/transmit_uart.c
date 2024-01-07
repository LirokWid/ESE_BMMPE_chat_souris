#include "transmit_uart.h"

extern SemaphoreHandle_t lidar_half_DMA_semaphore;
extern SemaphoreHandle_t lidar_full_DMA_semaphore;

void uart_transmit(){

}

void LIDAR_transmit(){

}

int Lidar_uart_transmit(uint8_t *p_data, uint16_t size)
{
	HAL_UART_Transmit(&huart2,p_data, size, HAL_MAX_DELAY);
	return 0;
}

int Lidar_uart_receive(uint8_t *p_data)
{
	HAL_UART_Receive_DMA(&huart2,p_data,DMA_BUFFER_SIZE);
	return 0;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{//UART half transfer callback interrupt
	if(huart->Instance == LIDAR_UART){
		BaseType_t xHigherPriorityTaskToken = pdFALSE;
	    xSemaphoreGiveFromISR(lidar_half_DMA_semaphore, &xHigherPriorityTaskToken);
		portYIELD_FROM_ISR(xHigherPriorityTaskToken);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{//UART full transfer callback interrupt
	HAL_GPIO_TogglePin(LED_STATE_GPIO_Port, LED_STATE_Pin);
	if(huart->Instance == USART1){

	}
	if(huart->Instance == LIDAR_UART){
		BaseType_t xHigherPriorityTaskToken = pdFALSE;
	    xSemaphoreGiveFromISR(lidar_full_DMA_semaphore, &xHigherPriorityTaskToken);
		portYIELD_FROM_ISR(xHigherPriorityTaskToken);
	}
}
