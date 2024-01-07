#pragma once

#include "main.h"
#include "stdio.h"

#define DEBUG_UART huart1

//Computer uart port via st/link
extern UART_HandleTypeDef DEBUG_UART;

#ifdef __GNUC__ /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf    set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

