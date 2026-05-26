#ifndef INT_IP5305T_H
#define INT_IP5305T_H
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


//q启动电源，防止自动关机
void IP5305T_start(void);

//关机
void IP5305T_shutdown(void);

#endif // INT_IP5305T_H
