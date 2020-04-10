/********************************************* 
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * IP/ARP/UDP/TCP functions
 *
 *********************************************/
//@{
#ifndef IP_ARP_UDP_TCP_H
#define IP_ARP_UDP_TCP_H

// you must call this function once before you use any of the other functions:
extern void init_ip_arp_udp_tcp(unsigned char *mymac,unsigned char *myip,unsigned int wwwp);
//
extern unsigned char eth_type_is_my_mac(unsigned char *buf,unsigned int len);
extern unsigned char eth_type_is_boardcast(unsigned char *buf,unsigned int len);
extern unsigned char eth_type_is_arp_and_my_ip(unsigned char *buf,unsigned int len);
extern unsigned char eth_type_is_ip_and_my_ip(unsigned char *buf,unsigned int len);
extern void make_arp_answer_from_request(unsigned char *buf);
extern void make_echo_reply_from_request(unsigned char *buf,unsigned int len);
//extern void make_udp_reply_from_request(unsigned char *buf,char *data,unsigned char datalen,unsigned int port);
extern void make_udp_reply_from_request(unsigned char *buf,char *data,unsigned int datalen,unsigned int port);


extern void make_tcp_synack_from_syn(unsigned char *buf);
extern void init_len_info(unsigned char *buf);
extern unsigned int get_tcp_data_pointer(void);
//extern unsigned int fill_tcp_data_p(unsigned char *buf,unsigned int pos, const prog_char *progmem_s);
extern unsigned int fill_tcp_data_p(unsigned char *buf,unsigned int pos, const char *progmem_s);
extern unsigned int fill_tcp_data(unsigned char *buf,unsigned int pos, const char *s);
extern void make_tcp_ack_from_any(unsigned char *buf);
extern void make_tcp_ack_with_data(unsigned char *buf,unsigned int dlen);
extern void make_winpcap_ack_with_data(unsigned char *buf,unsigned int dlen);


#endif /* IP_ARP_UDP_TCP_H */
//@}
