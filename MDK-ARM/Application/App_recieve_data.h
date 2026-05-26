#ifndef __APP_RECIEVE_DATA_H__
#define __APP_RECIEVE_DATA_H__

#define FRAME_HEADE_CHECK_1 'Z'
#define FRAME_HEADE_CHECK_2 'W'
#define FRAME_HEADE_CHECK_3 'L'

#include "Com_config.h"
#include "int_SI24R1.h"
#include "Com_debug.h"

#define MAX_RETRY_COUNT 10       //最大重试次数

/**
 * @brief 接收摇杆的数据=》解析为结构体
 * 
 * @return uint8_t 0: success,校验通过  1: fail，没数据或者校验失败
 * 
 */
uint8_t App_recieve_data(void);

/**
 * @brief 处理连接状态位
 * 
 * @param res 上次接收数据的返回值
 */
void App_proccess_connect_state(uint8_t res);

/**
 * @brief 处理飞机的飞行状态
 * 
 */
void App_proccess_flight_state(void);

#endif // __APP_RECIEVE_DATA_H__
