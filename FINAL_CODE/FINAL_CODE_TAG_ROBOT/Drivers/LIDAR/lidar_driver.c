/**
 * @file lidar_driver.c
 * @brief Lidar driver implementation.
 * @date Oct 5, 2023
 * @author Baptiste
 */

#include "lidar_driver.h"

extern SemaphoreHandle_t lidar_full_DMA_semaphore;
extern SemaphoreHandle_t find_target_semaphore;

int Lidar_set_motor_speed(int speed)
{
    return 0;
}

int lidar_init_driver(h_lidar_t *lidar, UART_HandleTypeDef *lidar_uart)
{
    lidar->serial_drv.transmit = Lidar_uart_transmit;
    lidar->serial_drv.receive = Lidar_uart_receive;
    lidar->uart = lidar_uart;
    lidar->is_sending = 0;

    HAL_GPIO_WritePin(LIDAR_MESURE_ENABLE_GPIO_Port, LIDAR_MESURE_ENABLE_Pin, SET);
    HAL_GPIO_WritePin(LIDAR_MOTOR_ENABLE_GPIO_Port, LIDAR_MOTOR_ENABLE_Pin, SET);
    lidar->motor.set_speed(100);
    lidar_stop(lidar);
    lidar->dma_head = 0;
    lidar->serial_drv.receive(lidar->dma_buffer);
    lidar->state = WAITING_FOR_HEADER;
    lidar_scan(lidar);
    return SUCCESS;
}

int lidar_parse(h_lidar_t *lidar)
{
    points_sample_t points[POINT_SAMPLES_MAX_NB];

    uint8_t *ptr_dma = lidar->dma_buffer;

    int point_nb = 0;
    for (int i = 0; i < DMA_BUFFER_SIZE; i++)
    {
        if (lidar->dma_head > (DMA_BUFFER_SIZE))
        {
            lidar->dma_head = lidar->frame.frame_end = (lidar->dma_head + lidar->frame.LSN) - DMA_BUFFER_SIZE;
        }
        switch (lidar->state)
        {
        case WAITING_FOR_HEADER:
            if (ptr_dma[i] == START_BYTE_LOW && ptr_dma[i + 1] == START_BYTE_HIGH)
            {
                lidar->is_sending = 1;
                lidar->dma_head = i + 2;
                lidar->state = PARSING_TYPE;
            }
            else
            {
                lidar->dma_head++;
            }
            break;

        case PARSING_TYPE:
            lidar->frame.CT = ptr_dma[lidar->dma_head] & 0x1;
            lidar->dma_head++;
            lidar->state = PARSING_LENGHT;
            break;

        case PARSING_LENGHT:
            lidar->frame.LSN = ptr_dma[lidar->dma_head];
            if ((lidar->frame.LSN) + (lidar->dma_head) > DMA_BUFFER_SIZE)
            {
                lidar->frame.frame_end = (lidar->dma_head + lidar->frame.LSN) - DMA_BUFFER_SIZE;
            }
            else
            {
                lidar->frame.frame_end = lidar->dma_head + lidar->frame.LSN;
            }
            lidar->dma_head++;
            lidar->state = PARSING_START_ANGLE;
            break;

        case PARSING_START_ANGLE:
            lidar->frame.FSA = (ptr_dma[lidar->dma_head]) | (ptr_dma[lidar->dma_head + 1] << 8);
            lidar->dma_head++;
            lidar->dma_head++;
            lidar->state = PARSING_END_ANGLE;
            break;

        case PARSING_END_ANGLE:
            lidar->frame.LSA = (ptr_dma[lidar->dma_head]) | (ptr_dma[lidar->dma_head + 1] << 8);
            lidar->dma_head++;
            lidar->dma_head++;
            lidar->state = PARSING_CHECKSUM;
            break;

        case PARSING_CHECKSUM:
            lidar->frame.CS = ptr_dma[lidar->dma_head];
            lidar->dma_head++;
            lidar->state = PARSING_DATA;
            break;

        case PARSING_DATA:
            if (point_nb > 18)
            {
                lidar->state = WAITING_FOR_HEADER;
                return 1;
            }
            aggregate_points_data(lidar, points, point_nb++, ptr_dma[lidar->dma_head], ptr_dma[lidar->dma_head + 1]);
            lidar->dma_head++;
            lidar->dma_head++;
            if (lidar->dma_head > lidar->frame.frame_end)
            {
                store_points_data(lidar, points, point_nb);
                point_nb = 0;
                lidar->state = WAITING_FOR_HEADER;
            }
            break;
        }
    }
    return 0;
}

void aggregate_points_data(h_lidar_t *lidar, points_sample_t *points, uint8_t point_nb, uint8_t LSB, uint8_t MSB)
{
    uint16_t start_angle_undivided = (lidar->frame.FSA >> 1);
    uint16_t end_angle_undivided = (lidar->frame.LSA >> 1);

    lidar->frame.start_angle = start_angle_undivided / 64;
    lidar->frame.end_angle = end_angle_undivided / 64;
    int point_length = (lidar->frame.LSN) - 1;

    uint16_t angle = 0;
    uint16_t angle_diff = (end_angle_undivided - start_angle_undivided) / 64;
    if (angle_diff < 0)
    {
        angle_diff += 360;
    }
    angle = start_angle_undivided / 64 + point_nb * angle_diff / (point_length);

    if (angle > 360)
    {
        angle -= 360;
    }
    uint16_t distance = (MSB | (LSB << 8)) / 4;

    points[point_nb].distance = distance;
    points[point_nb].angle = angle;
}

#define ANGLE_SHIFT 180
#define MIN_STORE_DISTANCE 100
#define MAX_STORE_DISTANCE 2500

void store_points_data(h_lidar_t *lidar, points_sample_t *points, uint8_t point_nb)
{
    for (int i = 0; i < point_nb; i++)
    {
        uint16_t current_angle = points[i].angle;
        if (current_angle < 360)
        {
#ifdef ANGLE_SHIFT
            current_angle = current_angle + ANGLE_SHIFT;
            if (current_angle > 359)
            {
                current_angle -= 360;
            };
            if (current_angle < 0)
            {
                current_angle += 360;
            };
#endif
            if ((points[i].distance < MIN_STORE_DISTANCE) || (points[i].distance > MAX_STORE_DISTANCE))
            {
                lidar->points[current_angle] = MAX_STORE_DISTANCE;
            }
            else
            {
                lidar->points[current_angle] = points[i].distance;
            }
        }
    }
    if (points[0].angle > points[point_nb].angle)
    {
        xSemaphoreGive(find_target_semaphore);
    }
}

int lidar_stop(h_lidar_t *lidar)
{
    lidar->cmd = COMMAND_STOP_SCAN;
    lidar->serial_drv.transmit((uint8_t *)&(lidar->cmd), 2);
    return 0;
}

int lidar_info(h_lidar_t *lidar)
{
    lidar->cmd = COMMAND_GET_ID;
    lidar->serial_drv.transmit((uint8_t *)&(lidar->cmd), 2);
    return 0;
}

int lidar_scan(h_lidar_t *lidar)
{
    lidar->cmd = COMMAND_START_SCAN;
    lidar->serial_drv.transmit((uint8_t *)&(lidar->cmd), 2);
    return 0;
}

int lidar_restart(h_lidar_t *lidar)
{
    lidar->cmd = COMMAND_SOFT_RESTART;
    lidar->serial_drv.transmit((uint8_t *)&(lidar->cmd), 2);
    return 0;
}
