#ifndef INT_MOTOR_H
#define INT_MOTOR_H

#include "Com_debug.h"
#include "tim.h"

typedef struct {
    TIM_HandleTypeDef *tim; // 定时器句柄
    uint16_t channel; // 定时器通道
    uint16_t speed; // 电机速度

} Motor_Struct;

void Int_motor_set_speed(Motor_Struct *motor); // @brief 设置电机速度，speed范围200-1000 默认值是200

void motor_start(Motor_Struct *motor); // @brief 启动电机

#endif // INT_MOTOR_H
