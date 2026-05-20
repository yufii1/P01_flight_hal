#include "int_led.h"

//
void LED_turn_on(LED_Struct *led)
{
    //直接修改引脚电平为 低电平 - 开灯 
    HAL_GPIO_WritePin(led->GPIOX, led->GPIO_Pin, GPIO_PIN_RESET);       
}

void LED_turn_off(LED_Struct *led)
{
    //直接修改引脚电平为 高电平 - 关灯 
    HAL_GPIO_WritePin(led->GPIOX, led->GPIO_Pin, GPIO_PIN_SET);
}

void Int_led_toggle(LED_Struct *led)
{
    //直接修改引脚电平为 取反 - 切换状态 
    HAL_GPIO_TogglePin(led->GPIOX, led->GPIO_Pin);
}

