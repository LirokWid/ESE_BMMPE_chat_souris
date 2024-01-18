/**
 * @file lidar_driver.h
 * @brief Header file for the Lidar driver.
 * @date Oct 5, 2023
 * @author LirokWid
 */

#ifndef INC_LIDAR_DRIVER_H_
#define INC_LIDAR_DRIVER_H_

#include "gpio.h"
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../UART/transmit_uart.h"

#define SERIAL_DEBUG

#define DEF_PREFIX 0xA5
#define DEF_COMMAND_START_SCAN 0x60
#define DEF_COMMAND_STOP_SCAN 0x65
#define DEF_COMMAND_GET_ID 0x90
#define DEF_COMMAND_GET_STATUS 0x91
#define DEF_COMMAND_SOFT_RESTART 0x80

#define DMA_BUFFER_SIZE 50*10
#define POINT_BUFFER_SIZE 100
#define LIDAR_SERIAL_SPEED 128000
#define CLEARED_POINT 65533
#define VALUE_TOO_CLOSE 65534

typedef enum {
    PREFIX = 0xA5,
    COMMAND_START_SCAN = 0xA560,
    COMMAND_STOP_SCAN = 0xA565,
    COMMAND_GET_ID = 0xA590,
    COMMAND_GET_STATUS = 0xA591,
    COMMAND_SOFT_RESTART = 0xA580
} LidarCommand;

typedef struct {
    uint8_t prefix;
    uint8_t command;
} LidarCommandStruct;

typedef int (* lidar_transmit_drv_t)(uint8_t *p_data, uint16_t size);
typedef int (* lidar_receive_drv_t)(uint8_t *p_data);

typedef struct {
    lidar_transmit_drv_t transmit;
    lidar_receive_drv_t receive;
} lidar_serial_drv_t;

typedef int (* set_motor_speed_t)(uint8_t speed);

typedef struct {
    uint8_t speed;
    set_motor_speed_t set_speed;
} lidar_motor_drv_t;

typedef enum {
    WAITING_FOR_HEADER,
    PARSING_TYPE,
    PARSING_LENGHT,
    PARSING_START_ANGLE,
    PARSING_END_ANGLE,
    PARSING_CHECKSUM,
    PARSING_DATA
} lidar_state_t;

typedef enum {
    START_BYTE_LOW  = 0xAA,
    START_BYTE_HIGH = 0x55
} h_lidar_parsing_bytes_t;

typedef struct {
    uint8_t CT;
    uint8_t LSN;
    uint16_t FSA;
    uint16_t LSA;
    uint16_t CS;
    uint16_t frame_end;
    uint16_t start_angle;
    uint16_t end_angle;
} lidar_frame_header_t;

#define ANGLE_CIRCLE_SUBDIVISION 360

typedef struct {
    uint16_t angle;
    uint16_t distance;
} lidar_point_data_t;

typedef struct {
    lidar_serial_drv_t serial_drv;
    lidar_motor_drv_t motor;
    UART_HandleTypeDef *uart;
    LidarCommand cmd;
    lidar_point_data_t target;
    lidar_state_t state;
    uint8_t model;
    uint8_t hardware;
    uint8_t serial_number[16];
    struct firmware {
        uint8_t major;
        uint8_t minor;
    } firmware;
    struct health {
        uint8_t status;
        uint8_t error_code[2];
    } health;
    uint8_t is_sending;
    lidar_frame_header_t frame;
    uint8_t  dma_buffer[DMA_BUFFER_SIZE];
    uint16_t dma_head;
    uint16_t dist_buff[DMA_BUFFER_SIZE];
    uint16_t points[ANGLE_CIRCLE_SUBDIVISION];
    uint32_t average[72];
} h_lidar_t;

#define POINT_SAMPLES_MAX_NB 50

typedef struct {
    uint16_t angle;
    uint16_t distance;
} points_sample_t;

typedef struct {
    uint8_t   status;
    uint16_t  error_code;
} device_health_t;

int Lidar_set_motor_speed(int speed);
int lidar_init_driver(h_lidar_t *lidar, UART_HandleTypeDef *lidar_uart);
int lidar_parse(h_lidar_t *lidar);
void start_motor();
void stop_motor();
int lidar_parse(h_lidar_t *lidar);
int lidar_stop(h_lidar_t * lidar);
int lidar_info(h_lidar_t * lidar);
int lidar_scan(h_lidar_t * lidar);
int lidar_restart(h_lidar_t * lidar);
void aggregate_points_data(h_lidar_t *lidar, points_sample_t *points, uint8_t point_nb, uint8_t LSB, uint8_t MSB);
void store_points_data(h_lidar_t *lidar, points_sample_t *points, uint8_t point_nb);
void lidar_set_motor_speed(int speed);

#endif /* INC_LIDAR_DRIVER_H_ */
