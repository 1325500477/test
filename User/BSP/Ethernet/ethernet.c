//Tab size: 4;	Indent size: 4
#include <string.h>	

#include "bsp.h"
#include "stm32f10x.h"
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "net_protocol.h"
#include "spi.h"

unsigned char cmd_buf[NET_PACKAGE_LENGTH];	//cmd_buf中包含有效数据以及网络帧包头等相关信息
unsigned char net_msg_type;

bool net_cmd_busy_flag = false;     //网络通信（接收-》解析-》应答 完成）
bool net_cmd_recv_flag = false;     //网络已收到一包数据
unsigned int net_cmd_data_len = 0;
bool net_cmd_send_flag = false;
bool net_cmd_error_flag = false;    //网络收到错误命令(系统暂时未使用，保留）
unsigned int net_cmd_send_length = 0;   //网络需要共发送的字节数

//网络接口及协议栈初始化
void init_ethernet(void)
{
	GROUP_NET_RST->BRR = IO_NET_RST;
	delay_ms(100);
	GROUP_NET_RST->BSRR = IO_NET_RST;
	//unsigned char enc28j60_test;
	SPI1_Init();
	enc28j60Init(board_mac);
	delay_ms(12); // 12ms
	//LEDB=yellow LEDA=green
	//0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
	//enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
	//enc28j60PhyWrite(PHLCON,0x476);
	enc28j60PhyWrite(PHLCON,0x470);
	delay_ms(12); // 12ms
	
	//init the ethernet/ip layer:
	init_ip_arp_udp_tcp(board_mac,board_ip,APP_TCP_PORT);
	
	memset(comm_send_buf, 0 , MAX_PACKAGE_LENGTH);
	memset(comm_recv_buf, 0 , MAX_PACKAGE_LENGTH);
	memset(comm_send_buf_last, 0 , MAX_PACKAGE_LENGTH);
	memset(cmd_buf, 0, NET_PACKAGE_LENGTH);	
	//test enc28j60 is OK 
	//enc28j60_test = enc28j60getrev();
	//enc28j60_test = enc28j60linkup();
	//enc28j60_test = enc28j60hasRxPkt();
}

//TCP通信的处理，直接将TCP端口接收缓冲区的数据拷贝至comm_recv_buf，并给与包应答
void ethernet_tcp(unsigned char *buf, unsigned int len)
{
	unsigned int dat_p;
	
	static unsigned char last_seq[4] = {0x00,0x00,0x00,0x00};
	static unsigned char last_ack_seq[4] = {0x00,0x00,0x00,0x00};
	
	//注意：
	//断点调试中发现，电脑端会进行数据的重发（TCP连接存在着未收到接收端返回的ACK，实现自动重发的机制）
	//客户端接收到重复的数据后会进行重复的数据处理过程，并给与应答
	//为了避免此种情况，需要判定本次所接收的TCP数据包
	//加入seq和ack （TCP_SEQ_H_P）的判定，假如是重发的网络数据，应该只发送ACK，而不用进行数据处理和数据应答
	
	if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
	{
		make_tcp_synack_from_syn(buf);
		// make_tcp_synack_from_syn does already send the syn,ack
		return;
	}

	if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
	{
		init_len_info(buf); // init some data structures
		dat_p=get_tcp_data_pointer();
		if (dat_p==0)
		{
			if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
			{
				// finack, answer with ack
				//确认哪一包数据被上位机接收到！
				make_tcp_ack_from_any(buf);
			}
			// just an ack with no data, wait for next packet
			return;
		}
		if((last_seq[0] != buf[TCP_SEQ_H_P+0]) ||
		(last_seq[1] != buf[TCP_SEQ_H_P+1]) ||
		(last_seq[2] != buf[TCP_SEQ_H_P+2]) ||
		(last_seq[3] != buf[TCP_SEQ_H_P+3]) ||
		(last_ack_seq[0] != buf[TCP_SEQACK_H_P+0]) ||
		(last_ack_seq[1] != buf[TCP_SEQACK_H_P+1]) ||
		(last_ack_seq[2] != buf[TCP_SEQACK_H_P+2]) ||
		(last_ack_seq[3] != buf[TCP_SEQACK_H_P+3]) )
		{
			memcpy(last_seq, &buf[TCP_SEQ_H_P],4);
			memcpy(last_ack_seq, &buf[TCP_SEQACK_H_P],4);
			
			memcpy(comm_recv_buf, (char *)&(buf[dat_p]), MAX_PACKAGE_LENGTH);
			//memcpy(comm_recv_buf, (char *)&(buf[dat_p]), len);
			net_msg_type = MSG_TYPE_TCP;
			net_cmd_recv_flag = TRUE; 
			net_cmd_data_len = len;      
			//处理相关数据
			//产生回发的数据
		}
		else
		{
			//若收到对方多包重发的数据包过来，有以下三种方式来处理:
			//1.对第一包回应ACK，并返回正常信息，并对后面的每一包回应ACK，并返回上一次的信息
			//2.对第一包回应ACK，并返回正常信息，对后面的每一包回应ACK，但不返回数据
			//3.对第一包回应ACK，并返回正常信息，对后面的每一包数据都不做任何处理
			
			//对重发的数据不做任何的处理！
			//第1种，只留下处理方式1，屏蔽处理方式2和3
			//memcpy(comm_send_buf,comm_send_buf_last,sizeof(comm_send_buf));  //处理方式1
			
			//第2种，只留下处理方式2，屏蔽处理方式1和3
			//make_tcp_ack_from_any(buf); //处理方式2
			
			//第3种，只留下处理方式3，屏蔽处理方式1和2
			return;//处理方式3
		}
	
		//SENDTCP:
		make_tcp_ack_from_any(buf); // send ack
		
//	    if(net_cmd_error_flag)
//	    {
//			net_cmd_error_flag = 0;
//			for( tmp_i = 0; tmp_i < sizeof(comm_send_buf); tmp_i++)
//			{
//				comm_send_buf[tmp_i] = tmp_i + test_counter;
//			}
//			test_counter ++;
//			memcpy(&buf[TCP_CHECKSUM_L_P+3], comm_send_buf, sizeof(comm_send_buf));
//			plen = sizeof(comm_send_buf);
//			
//			make_tcp_ack_with_data(buf,plen); // send data
//			memcpy(comm_send_buf_last, comm_send_buf, sizeof(comm_send_buf));
//			memset(comm_send_buf, 0, sizeof(comm_send_buf));
//	    }
		return;
	}
}

//UDP通信的处理，直接将UDP端口接收缓冲区的数据拷贝至comm_recv_buf
void ethernet_udp(unsigned char *buf, unsigned int len)
{
	memcpy(comm_recv_buf, (char *)&(buf[UDP_DATA_P]), len);
	net_msg_type = MSG_TYPE_UDP;
	net_cmd_recv_flag = 1;
	net_cmd_data_len = len;
}

//Winpcap通信的处理，直接将原始数据拷贝至comm_recv_buf
void ethernet_winpcap(unsigned char *buf, unsigned int len)
{
    //Winpcap通讯方式，前面12个字节默认为发送方和接收方的MAC地址
	memcpy(comm_recv_buf, (char *)&(buf[12]), len);
	net_msg_type = MSG_TYPE_WINPCAP;
	net_cmd_recv_flag = 1;
	net_cmd_data_len = len;
}

//ARP和ICMP包的处理
void ethernet_arp_icmp(unsigned char *buf,unsigned int len)
{

}


//网络接口数据接收并分析
void process_ethernet(void)
{
	unsigned int plen;
//	char cBufferInfo[128];
//  unsigned int i;

    if(net_cmd_busy_flag)
      return;
	
	//对于TCP连接，嵌入式控制器发出去的数据，需要收到ACK的确认
	//若一定的时间内没有收到此确认，需要重发数据
	//目前，本代码中似乎没有此功能
	//20160502  底层不用TCP功能，只用UDP和WinPcap功能

	//get the next new packet:
	plen = enc28j60PacketReceive(NET_PACKAGE_LENGTH, cmd_buf);
	if(plen==0)
	{
		return;
	}

    if((0 == eth_type_is_my_mac(cmd_buf,plen))          //非本网卡的数据包
       &&(0 == eth_type_is_boardcast(cmd_buf,plen)))    //非广播包
	{
//        debug_info(".");
//        debug_info("It's not myboard mac!\n") ;
//        memset(cBufferInfo,0,sizeof(cBufferInfo));
//        sprintf(cBufferInfo,"length=%d:\t",plen);
//        debug_info(cBufferInfo);
//
//        memset(cBufferInfo,0,sizeof(cBufferInfo));
//        if(plen > 128)
//            plen = 128; //防止溢出
//        for(i = 0; i < plen; i++)
//        {
//            memset(cBufferInfo,0,sizeof(cBufferInfo));
//            sprintf(cBufferInfo,"0x%x\t",cmd_buf[i]);
//            debug_info(cBufferInfo);
//        }
//        debug_info("\n");
        return;
    }

	// arp is broadcast if unknown but a host may also
	// verify the mac address by sending it to
	// a unicast address.
	if(eth_type_is_arp_and_my_ip(cmd_buf,plen))     //ARP
	{
		make_arp_answer_from_request(cmd_buf);
		return;
	}
	
	if((cmd_buf[IP_PROTO_P] == IP_PROTO_ICMP_V) &&
	(cmd_buf[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V))  //ICMP
	{
		// a ping packet, let's send pong
		make_echo_reply_from_request(cmd_buf,plen);
		return;
	}
	
	// tcp port start, compare only the lower byte
	if((cmd_buf[IP_PROTO_P] == IP_PROTO_TCP_V) &&
	(cmd_buf[TCP_DST_PORT_H_P] == (APP_TCP_PORT >> 8)) &&
	(cmd_buf[TCP_DST_PORT_L_P] == (APP_TCP_PORT & 0xFF)))   //TCP
	{
		ethernet_tcp(cmd_buf,plen);
		return;
	}
	// tcp port end
	
	// udp start, we listen on udp port 8080
	if((cmd_buf[IP_PROTO_P] == IP_PROTO_UDP_V) &&
	(cmd_buf[UDP_DST_PORT_H_P] == (APP_UDP_PORT >> 8)) &&
	(cmd_buf[UDP_DST_PORT_L_P] == (APP_UDP_PORT & 0xFF)))   //UDP
	{
		ethernet_udp(cmd_buf,plen);
		return;
	}

	// check if ip packets are for us, if not ip package, use winpcap
	if(eth_type_is_my_mac(cmd_buf,plen))      //WINPCAP
	{
        //use winpcap to analysis
//        debug_info("*");
//        debug_info("It's myboard mac with winpcap!\n") ;
//        memset(cBufferInfo,0,sizeof(cBufferInfo));
//        sprintf(cBufferInfo,"length=%d:\t",plen);
//        debug_info(cBufferInfo);
//
//        memset(cBufferInfo,0,sizeof(cBufferInfo));
//        if(plen > 128)
//            plen = 128; //防止溢出
//        for(i = 0; i < plen; i++)
//        {
//            memset(cBufferInfo,0,sizeof(cBufferInfo));
//            sprintf(cBufferInfo,"0x%x\t",cmd_buf[i]);
//            debug_info(cBufferInfo);
//        }
//        debug_info("\n");
        ethernet_winpcap(cmd_buf,plen);
		return;
	}
}

//网络接口发送数据
void ethernet_send_msg(void)
{
	if(net_cmd_send_flag)
	{
		if(net_msg_type == MSG_TYPE_UDP)		//UDP数据的发送
		{
			make_udp_reply_from_request(cmd_buf, (char *)comm_send_buf, net_cmd_send_length, APP_UDP_PORT);
			memcpy(comm_send_buf_last, comm_send_buf, net_cmd_send_length);
			//memset(comm_send_buf, 0, MAX_PACKAGE_LENGTH);
			//debug_info((char*)comm_send_buf);
		}
		else if(net_msg_type == MSG_TYPE_TCP)	//TCP数据的发送
		{
			memcpy(&cmd_buf[TCP_CHECKSUM_L_P+3], comm_send_buf, net_cmd_send_length);
			make_tcp_ack_with_data(cmd_buf, net_cmd_send_length); // send data
			memcpy(comm_send_buf_last, comm_send_buf, net_cmd_send_length);
			//memset(comm_send_buf, 0, MAX_PACKAGE_LENGTH); 	
		}
        else if(net_msg_type == MSG_TYPE_WINPCAP)
        {
            memcpy(&cmd_buf[12], comm_send_buf, net_cmd_send_length);
			make_winpcap_ack_with_data(cmd_buf, net_cmd_send_length); // send data
			memcpy(comm_send_buf_last, comm_send_buf, net_cmd_send_length);
        }
		net_cmd_send_flag = 0;  //TCP需要等到对方ACK吗？
	}
}

void ethernet_send_msg_winpcap(unsigned char *src_mac, unsigned char *dst_mac, unsigned char *comm_send_buf, unsigned char send_length)
{
    unsigned char i=0;
    //copy the destination mac from the source and fill my mac into src
    for(i=0; i< 6; i++)
	{
        cmd_buf[ETH_DST_MAC +i] = dst_mac[i];
        cmd_buf[ETH_SRC_MAC +i] = src_mac[i];
    }
	memcpy(&cmd_buf[12], comm_send_buf, send_length); 
    enc28j60PacketSend(12 + send_length, cmd_buf);			
}
