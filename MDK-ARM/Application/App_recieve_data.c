#include "App_recieve_data.h"

extern Remote_Data remote_data;//飞行器姿态数据

extern Flight_State flight_state;//飞行器飞行状态位

extern Remote_State remote_state; //连接状态

uint8_t retry_count = 0; // 定义一个重试计数器，记录连续接收失败的次数


uint8_t rx_buf[TX_PLOAD_WIDTH] = {0}; // 定义一个静态接收缓冲区，存储从遥控器接收到的数据

uint8_t App_recieve_data(void)
{
    //debug_print("[App_recieve_data] Start\r\n");
    memset(rx_buf, 0, sizeof(rx_buf));
    //debug_print("[App_recieve_data] Calling int_SI24R1_RxPacket\r\n");
    if (int_SI24R1_RxPacket(rx_buf) == 1)
    {
        //debug_print("[App_recieve_data] No data received\r\n");
        return 1; // RX_DR flag not set, no data in FIFO
    }

    //debug_print("Received NO.0 success\n");

    //1.校验帧头
    if (rx_buf[0] != FRAME_HEADE_CHECK_1 || rx_buf[1] != FRAME_HEADE_CHECK_2 || rx_buf[2] != FRAME_HEADE_CHECK_3)
    {
        return 1; //帧头校验失败
    }
    //debug_print("Received NO.1 success\n");

    //2.帧尾校验  (校验和)
    uint32_t sum = 0;
    uint32_t check_sum = 0;
    for (int i = 0; i < 13; i++)
    {
        sum += rx_buf[i]; // 累加前13个字节的值，得到校验和
    }

    //高位在前
    check_sum = rx_buf[13]<<24 | rx_buf[14]<<16 | rx_buf[15]<<8 | rx_buf[16];
    if (sum != check_sum)
    {
        return 1; //校验和校验失败
    }
    //debug_print("Received NO.3 success\n");

    //3.解析数据
    remote_data.throttle = (rx_buf[3] << 8 | rx_buf[4]);
    remote_data.yaw = (rx_buf[5] << 8 | rx_buf[6]);
    remote_data.pitch = (rx_buf[7] << 8 | rx_buf[8]);
    remote_data.roll = (rx_buf[9] << 8 | rx_buf[10]);
    remote_data.shutdown = rx_buf[11];
    remote_data.fix_height = rx_buf[12];

    debug_print(":%d,%d,%d,%d,%d,%d\r\n",remote_data.throttle,remote_data.yaw,remote_data.pitch,remote_data.roll,remote_data.shutdown,remote_data.fix_height);

    return 0;
}

void App_proccess_connect_state(uint8_t res)


{
    if (res == 0)
    {
        //
        //
        remote_state = REMOTE_CONNECT;
        retry_count = 0; //接收成功，重置重试计数器
    }
    else if (res == 1)
    {
        retry_count++; //接收失败，重试计数器加1
        if (retry_count >= MAX_RETRY_COUNT)
        {
            remote_state = REMOTE_DISCONNECT; //连续接收失败达到最大重试次数，认为遥控器断开连接
            retry_count = 0; //重置重试计数器
        }

    }
}


/**
 * @brief 空闲状态=》正常状态  解锁
 * 
 * @return uint8_t 0：解锁成功      1：解锁失败
 */
static uint8_t App_proccess_unlock(void)
{

    return 0;
}

/**
 * @brief 处理飞机的飞行状态
 * 
 */
void App_proccess_flight_state(void)
{
    //使用状态机逻辑实现
    //1.轮询调用当前所处状态
    switch (flight_state)
    {
        case IDLE://解锁成功 =》 正常飞行状态
            //2.只需要编写指向其他状态的转移条件和转移动作
            if(App_proccess_unlock() == 0) //如果解锁成功
            {
                flight_state = NORMAL; //状态转移到正常飞行状态
            }

            break;
        case NORMAL:
            //3.判断进入定高
            if(remote_data.fix_height == 1) 
            {
                flight_state = FIX_HIGH; //状态转移到定高状态
                remote_data.fix_height = 0; 
            }
            //4.判断进入故障状态
            if(remote_state == REMOTE_DISCONNECT) //遥控器断开连接
            {
                flight_state = FAIL; //转移到故障状态
            }
            
            break;
        case FIX_HIGH:
            //5.取消定高
            if(remote_data.fix_height == 1) 
            {
                flight_state = NORMAL; //状态转移到正常状态
                remote_data.fix_height = 0; 
            }
            //6.判断进入故障状态
            if(remote_state == REMOTE_DISCONNECT) //遥控器断开连接
            {
                flight_state = FAIL; //转移到故障状态
            }

            break;
        case FAIL:
            //7.处理失联状态，缓慢停止电机
            //TODO
            flight_state = NORMAL; //转移到正常状态

            break;
        default:
             break;

    }

}

