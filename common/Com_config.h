#ifndef COM_CONFIG_H

#include "main.h"

typedef enum
{
    REMOTE_CONNECT =0,

    REMOTE_DISCONNECT ,
}Remote_State;      //遥控器连接状态

typedef enum
{
    IDLE = 0,
    NORMAL ,
    FIX_HIGH ,
    FAIL ,
}Flight_State;    //飞机飞行状态

typedef enum
{
    FREE ,
    MAX ,
    LEAVE_MAX ,
    MIN ,
    UNLOCK ,
}Thr_State;    //油门解锁状态

typedef struct {
    int16_t throttle;       //油门
    int16_t yaw;           //偏航
    int16_t pitch;          //俯仰
    int16_t roll;            //横滚
    uint8_t shutdown;       //关机 1  |  不关机 0
    uint8_t fix_height;        //1定高不定高切换 0不定高    
} Remote_Data;   //遥控器数据结构体

#define COM_CONFIG_H
#endif /* COM_CONFIG_H */
