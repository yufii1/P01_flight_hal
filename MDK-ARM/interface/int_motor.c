#include "int_motor.h"


void Int_motor_set_speed(Motor_Struct *motor) 
{
    if (motor->speed > 1000)
    {
        debug_print("speed too high\r\n");
        return;
    }
    __HAL_TIM_SET_COMPARE( motor->tim, motor->channel, motor->speed);
    
}

void motor_start(Motor_Struct *motor) // @brief 启动电机
{
    HAL_TIM_PWM_Start(motor->tim, motor->channel);
}

