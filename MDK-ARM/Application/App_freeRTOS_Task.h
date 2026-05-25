#ifndef APP_FREERTOS_TASK_H
#define APP_FREERTOS_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "Com_debug.h"
#include "Int_IP5305T.h"
#include "int_motor.h"
#include "int_led.h"
#include "Com_config.h"
#include "int_SI24R1.h"
#include "App_recieve_data.h"

 
void App_freeRTOS_start (void); // @brief 启动freeRTOS操作系统

#endif /* APP_FREERTOS_TASK_H */


