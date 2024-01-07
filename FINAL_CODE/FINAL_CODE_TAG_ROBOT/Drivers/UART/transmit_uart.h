#pragma once

#include "usart.h"
//Redirect the printf function to HAL_uart transmit on uart1
#include "printf_uart.h"

#include "cmsis_os.h"


#include "../LIDAR/lidar_driver.h"

#define LIDAR_UART USART2

void uart_transmit();
void LIDAR_transmit();


int Lidar_uart_transmit(uint8_t *p_data, uint16_t size);
int Lidar_uart_receive(uint8_t *p_data);

//Uart callbacks on ISR
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart);
