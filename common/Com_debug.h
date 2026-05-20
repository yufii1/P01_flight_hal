#ifndef COM_DEBUG_H
#define COM_DEBUG_H

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

//日志输出打印非常占用CPU资源 ， 115200 波特率下，10 字节日志发送时间约 0.9ms。非常影响飞机飞行姿态计算
//所以后续飞机需要正常飞行时，需要关闭打印功能

//设计一个日志输出打印开关
#define DEBUG_LOG_ENABLE 1

#ifdef DEBUG_LOG_ENABLE

//使用宏定义方式，只打印文件名称，不打印路径

#define __FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1:__FILE__)

//使用宏定义的方式，实现打印输出日志之前，打印函数名和行号
#define debug_print(format , ...) printf("[%s:%d]  " format , __FILE_NAME__ , __LINE__, ##__VA_ARGS__) 

#else
//如果没有打开日志输出打印
#define debug_print(format , ...) 
#endif



#endif // COM_DEBUG_H
