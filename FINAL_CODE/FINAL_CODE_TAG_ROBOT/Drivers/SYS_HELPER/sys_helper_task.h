#ifndef INC_SYS_HELPER_TASK_H_
#define INC_SYS_HELPER_TASK_H_

#include <stdio.h>
#include "main.h"
#include "cmsis_os.h"

#include "../LIDAR/lidar_driver.h"
#include "../LIDAR/lidar_task.h"


int create_sys_task(int task_priority);
void is_system_alive(void *unused);
void print_lidar_point(void *unused);

#endif
