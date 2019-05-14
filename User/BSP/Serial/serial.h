//Tab size: 4;	Indent size: 4
#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "stdio.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif

//#define DEBUG

extern bool serial_cmd_busy_flag;
extern bool serial_cmd_recv_flag;
extern bool serial_send_finished_flag;
extern bool serial_cmd_error_flag;
extern unsigned int serial_cmd_send_length;
extern unsigned int serial_cmd_data_len;

void init_uart(unsigned char port_no, unsigned int baudrate_set);  //初始化串口
void serial_send_msg(void); //发送数据
void process_serial(void);

#ifdef __cplusplus
}
#endif

#endif
