#include "App_recieve_data.h"

Remote_Data remote_data = {0};

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

