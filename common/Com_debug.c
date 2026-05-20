#include "Com_debug.h"

//重定向fputc函数用于串口调试
int fputc (int ch, FILE *f) 
{
    //使用HAL库发送单个字符
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    //返回字符本身
    return ch;
}
