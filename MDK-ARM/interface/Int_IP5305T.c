#include "Int_IP5305T.h"


//q启动电源，防止自动关机
void IP5305T_start(void)
{
    
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET); // 设置Pin0为低电平，启动电源
    vTaskDelay(100); // 延时100毫秒，确保电源稳定
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
}

void IP5305T_shutdown(void)
{
    //1s之内短按两次关机
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET); // 设置Pin0为低电平，启动电源
    vTaskDelay(100); // 延时100毫秒，确保电源稳定
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);

    vTaskDelay (200); // 延时200毫秒，模拟短按

    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET); // 设置Pin0为低电平，启动电源
    vTaskDelay(100); // 延时100毫秒，确保电源稳定
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
}
