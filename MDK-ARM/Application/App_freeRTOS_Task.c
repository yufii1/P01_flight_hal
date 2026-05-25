#include "App_freeRTOS_Task.h"

//stm32f103c8t6 => SRAM 20KB => 分配12k给操作系统   
//内存管理 =》 c语言中结构体通常保存在堆中  不会自动垃圾回收   所以始终只用1个结构体（ 不断循环使用 ）

//电机结构体
Motor_Struct left_top_motor = {.tim = &htim3, .channel = TIM_CHANNEL_1, .speed = 200}; //左上电机
Motor_Struct left_bottom_motor = {.tim = &htim4, .channel = TIM_CHANNEL_4, .speed = 200}; //左下电机
Motor_Struct right_top_motor = {.tim = &htim2, .channel = TIM_CHANNEL_2, .speed = 200}; //右上电机
Motor_Struct right_bottom_motor = {.tim = &htim1, .channel = TIM_CHANNEL_3, .speed = 200}; //右下电机

//LED结构体
LED_Struct left_top_led = {.GPIOX = LED1_GPIO_Port, .GPIO_Pin = LED1_Pin}; //LED1  左上LED
LED_Struct right_top_led = {.GPIOX = LED2_GPIO_Port, .GPIO_Pin = LED2_Pin}; //LED2  右上LED
LED_Struct right_bottom_led = {.GPIOX = LED3_GPIO_Port, .GPIO_Pin = LED3_Pin}; //LED3  右下LED
LED_Struct left_bottom_led = {.GPIOX = LED4_GPIO_Port, .GPIO_Pin = LED4_Pin}; //LED4  左下LED

//电源管理任务
void power_task (void *args);
#define power_task_STACK_SIZE 128  //128*4 = 512B
#define power_task_PRIORITY 2       //优先级，数值越大优先级越高 
TaskHandle_t power_task_handle;
//定义任务周期
#define POWER_TASK_PERIOD 10000 //10s


//飞控任务
void flight_task (void *args);
#define flight_task_STACK_SIZE 128  //128*4 = 512B
#define flight_task_PRIORITY 3       //优先级，数值越大优先级越高 
TaskHandle_t flight_task_handle;
//定义任务周期
#define FLIGHT_TASK_PERIOD 6 //6ms


//LED灯控任务
void LED_task (void *args);
#define LED_task_STACK_SIZE 128  //128*4 = 512B
#define LED_task_PRIORITY 1       //优先级，数值越大优先级越高 
TaskHandle_t LED_task_handle;
//定义任务周期
#define LED_TASK_PERIOD 100 //100ms


//通讯任务
void com_task (void *args);
#define COM_TASK_STACK_SIZE 512  //512*4 = 2KB (从256增加到512，防止栈溢出)
#define COM_TASK_PRIORITY 4       //优先级，数值越大优先级越高 
TaskHandle_t com_task_handle;
#define COM_TASK_PERIOD 15           //定义任务周期



//遥控器连接状态
Remote_State remote_state = REMOTE_CONNECT; 

//飞行器飞行状态
Flight_State flight_state = IDLE;

void App_freeRTOS_start (void) // @brief 启动freeRTOS操作系统
{
    //创建电源管理任务
    xTaskCreate(power_task, "power_task", power_task_STACK_SIZE, NULL, power_task_PRIORITY, &power_task_handle);

    //创建飞控任务
    xTaskCreate(flight_task, "flight_task", flight_task_STACK_SIZE, NULL, flight_task_PRIORITY, &flight_task_handle);

    //创建LED灯控任务
    xTaskCreate(LED_task, "LED_task", LED_task_STACK_SIZE, NULL, LED_task_PRIORITY, &LED_task_handle);

    //创建通讯任务

    xTaskCreate(com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);
    //启动调度器
    vTaskStartScheduler();

}

void power_task (void *args)
{
    //获取当前基准时间
     TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        //每十秒执行一次，避免自动关机
       vTaskDelayUntil(&xLastWakeTime, POWER_TASK_PERIOD); //延时10秒
        //启动电源
        IP5305T_start();

        vTaskDelay(pdMS_TO_TICKS(1000)); //延时1秒
    }
}

void flight_task (void *args)
{
    //获取当前基准时间
     TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        //1.设置电机转速
            Int_motor_set_speed(&right_top_motor);
            Int_motor_set_speed(&right_bottom_motor);
            Int_motor_set_speed(&left_bottom_motor);

        //2.启动电机    
        Int_motor_set_speed(&left_top_motor);
        vTaskDelayUntil(&xLastWakeTime, FLIGHT_TASK_PERIOD); //延时6ms
    }
}

void LED_task (void *args)
{
    //获取当前基准时间
     TickType_t xLastWakeTime = xTaskGetTickCount();
     uint8_t count = 0;
    while(1)
    {
        count++;
        // 遥控器连接状态--由上方两个LED表示
        if(remote_state == REMOTE_CONNECT)
        {
            //遥控器连接 - 上方两个LED常亮
            LED_turn_on(&left_top_led);
            LED_turn_on(&right_top_led);
        }
        else if(remote_state == REMOTE_DISCONNECT)
        {
            //遥控器断开 - 上方两个LED关闭
            LED_turn_off(&left_top_led);
            LED_turn_off(&right_top_led);
        }

        // 飞行状态--由下方两个LED表示
        if (flight_state == IDLE)
        {
            // 空闲状态  灯慢闪烁 => 500ms亮 500ms灭
            if (count % 5 == 0)
            {
                // 循环5次  一次是100ms  5次等于500ms
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);
            }
        }
        else if (flight_state == NORMAL)
        {
            // 正常飞行  灯快闪  =>  200ms亮 200ms灭
            if (count % 2 == 0)
            {
                // 循环2次  一次是100ms  2次等于200ms
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);
            }
        }
        else if(flight_state == FIX_HIGH)
        {
            //高空定高 - 下方两个LED常亮
            LED_turn_on(&left_bottom_led);
            LED_turn_on(&right_bottom_led);
        }
        else if(flight_state == FAIL)
        {
            //故障 - 下方两个LED关闭
            LED_turn_off(&left_bottom_led);
            LED_turn_off(&right_bottom_led);
        }
        // 将count计数重置
        if (count == 10)
        {
            count = 0;
        }
        vTaskDelayUntil(&xLastWakeTime, LED_TASK_PERIOD); //延时100ms
    }
}


void com_task (void *args)
{
    //获取当前基准时间
     TickType_t xLastWakeTime = xTaskGetTickCount();
     taskENTER_CRITICAL();
    while(1)
    {
        //debug_print("[com_task] Before App_recieve_data\r\n");
        //接收遥控器数据，根据返回值更新连接状态
        uint8_t rx_result = App_recieve_data();
        //debug_print("[com_task] After App_recieve_data, result=%d\r\n", rx_result);
        if (rx_result == 0)
        {
            remote_state = REMOTE_CONNECT;
            //debug_print("Remote connected\r\n");
        }
        else if (rx_result == 1)
        {
            remote_state = REMOTE_DISCONNECT;
            //debug_print("Remote disconnected\r\n");
        }
        taskEXIT_CRITICAL();
        vTaskDelayUntil(&xLastWakeTime, COM_TASK_PERIOD); //6ms执行一次  发送 接收 的频率都设置为6ms  避免数据积压
    }
}

        

