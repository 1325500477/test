//Tab size: 4;	Indent size: 4
#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define MSG_TYPE_TCP  1
#define MSG_TYPE_UDP  2  
#define MSG_TYPE_WINPCAP    3

extern bool net_cmd_busy_flag;
extern bool net_cmd_recv_flag;
extern unsigned int net_cmd_data_len;
extern bool net_cmd_send_flag;
extern bool net_cmd_error_flag;
extern unsigned int net_cmd_send_length;

void init_ethernet(void);
void ethernet_send_msg(void);
void ethernet_send_msg_winpcap(unsigned char *src_mac, unsigned char *dst_mac, unsigned char *comm_send_buf, unsigned char nLength);
void process_ethernet(void);

#ifdef __cplusplus
}
#endif

#endif
