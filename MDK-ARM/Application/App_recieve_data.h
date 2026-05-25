#ifndef __APP_RECIEVE_DATA_H__
#define __APP_RECIEVE_DATA_H__

#define FRAME_HEADE_CHECK_1 'Z'
#define FRAME_HEADE_CHECK_2 'W'
#define FRAME_HEADE_CHECK_3 'L'

#include "Com_config.h"
#include "int_SI24R1.h"
#include "Com_debug.h"


/**
 * @brief 接收摇杆的数据=》解析为结构体
 * 
 * @return uint8_t 0: success,校验通过  1: fail，没数据或者校验失败
 * 
 */
uint8_t App_recieve_data(void);

#endif // __APP_RECIEVE_DATA_H__
