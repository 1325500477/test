//Tab size: 4;	Indent size: 4

#include <string.h>
#include <stdio.h>

#include "bsp.h"

unsigned char bRecvData=false;  //收到了一包数据
unsigned int txlen=0;           //将要发送的数据的长度
//extern bool usb_cmd_recv_flag;
//extern unsigned int usb_cmd_send_length;

bool device_connected = false;
unsigned int last_cmd_time = 0;						//上次收到数据的时间
unsigned char cmd_num_this_time = 0;
unsigned char cmd_num_last_time = 255;					//让第一个包号不至于相等，上层可以初始化为0
unsigned char flag_package_attr = 0;					//0-其它地方发过来的请求包 ; 1-其它地方发过来的应答包
unsigned char comm_recv_buf[MAX_PACKAGE_LENGTH];		//包含@@（....）
unsigned char comm_send_buf[MAX_PACKAGE_LENGTH];		//包含@@（....）
unsigned char comm_send_buf_last[MAX_PACKAGE_LENGTH];
unsigned int  cmd_tx_length=0;		//只包含cmd_type和cmd_data0~cmd_datan
unsigned int  cmd_rx_length=0;		//只包含cmd_type和cmd_data0~cmd_datan
unsigned char cParamBuf[512]; 		//目前凸字数据包最大，打50个字符最大占用需要50*3+50*4=350字节

//unsigned char comm_mac_addr[6];
//unsigned char comm_ip_addr[4];
//unsigned char comm_net_port[2];

//char WinPcapSendBuf[256];
//char WinPcapRecvBuf[256];
//void WinPcapInLanSendData(unsigned char nId,unsigned char *pBuf,unsigned int nLength)
//{
//	//m_nRetryCounter = 0;
//	//destination mac	//换成控制板的MAC
//	WinPcapSendBuf[0] = 0xAA;
//	WinPcapSendBuf[1] = 0xAA;
//	WinPcapSendBuf[2] = 0xAA;
//	WinPcapSendBuf[3] = 0xAA;
//	WinPcapSendBuf[4] = 0xAA;
//	WinPcapSendBuf[5] = nId;		//控制板的MAC规则
//	//source mac		//换成本机的MAC地址
//	WinPcapSendBuf[6] = m_NetAdapterInfo.Mac[0];
//	WinPcapSendBuf[7] = m_NetAdapterInfo.Mac[1];
//	WinPcapSendBuf[8] = m_NetAdapterInfo.Mac[2];
//	WinPcapSendBuf[9] = m_NetAdapterInfo.Mac[3];
//	WinPcapSendBuf[10] = m_NetAdapterInfo.Mac[4];
//	WinPcapSendBuf[11] = m_NetAdapterInfo.Mac[5];
//
//	memcpy(&WinPcapSendBuf[12], pBuf, nLength);
//	//此处发送数据
//	m_nLengthSend = 12 + nLength;
//	nRet = enc28j60PacketSend(WinPcapSendBuf, m_nLengthSend);
//}

/*
void comm_cmd_make_PACKAGE()
{
	unsigned int i;
	unsigned char data_lrc = 0;
	unsigned int lrc_length;
	unsigned int pos_PACKAGE_lrc;

	cmd_tx_length = txlen+1;	//默认只应答命令，表示已经接收了此事件
	//memset(comm_send_buf, 0 , sizeof(MAX_PACKAGE_LENGTH));      //清除发送的内容
    comm_send_buf[POS_SYN1]	= MSG_SYN1;				
	comm_send_buf[POS_SYN2]	= MSG_SYN2;
	comm_send_buf[POS_STX]		= MSG_STX;
	comm_send_buf[POS_VER]		= MSG_VER;
	comm_send_buf[POS_ADDR_RECV] = sender_addr;
	if(1)
		comm_send_buf[POS_ADDR_SEND] = board_addr;
	else
		comm_send_buf[POS_ADDR_SEND] = board_addr | 0x80; 	//被动回发的数据还是主动发送的数据，这个地方要做区分
	comm_send_buf[POS_CMD_SEQ]   = cmd_num_this_time;
	comm_send_buf[POS_CMD_LEN_H] = cmd_tx_length >> 8;
	comm_send_buf[POS_CMD_LEN_L] = cmd_tx_length & 0xFF;
	comm_send_buf[POS_CMD_TYPE]  = comm_recv_buf[POS_CMD_TYPE];
	
	lrc_length = cmd_tx_length + PACKAGE_INFO_LENGTH;
	data_lrc = comm_send_buf[POS_VER];
	for(i = 1; i < lrc_length; i++)
	{
		data_lrc ^= comm_send_buf[POS_VER + i];
	}      
	pos_PACKAGE_lrc = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + cmd_tx_length;
	comm_send_buf[pos_PACKAGE_lrc] = data_lrc;     
	comm_send_buf[pos_PACKAGE_lrc + 1] = MSG_ETX;
}

//检验数据包有效内容是否正确
int comm_cmd_check_valid()
{
	unsigned char data_lrc = 0;
	unsigned int i;
	unsigned int lrc_length;
	unsigned int pos_PACKAGE_lrc;

	if(comm_recv_buf[POS_SYN1] != MSG_SYN1)
		return PACKAGE_ERR_SYN1;
	if(comm_recv_buf[POS_SYN2] != MSG_SYN2)
		return PACKAGE_ERR_SYN2;
	if(comm_recv_buf[POS_STX] != MSG_STX)
		return PACKAGE_ERR_STX;
	if(comm_recv_buf[POS_VER] != MSG_VER)
		return PACKAGE_ERR_VER;
	//	//暂时忽略该项检测	
	//	if((comm_recv_buf[POS_ADDR_RECV] != board_addr) &&  //是否为本设备的数据，若不是，该如何处理？需要应答吗？
	//		(comm_recv_buf[POS_ADDR_RECV] != ADDR_BOARDCAST))
	//		return PACKAGE_ERR_RECV_ADDR;
	sender_addr = comm_recv_buf[POS_ADDR_SEND]; 
	if(!(sender_addr & 0x80))						
	{
		flag_package_attr = PACKAGE_ATTR_REQUEST; 			//主动请求的数据，Bit7位为0
	}
	else
	{
		flag_package_attr = PACKAGE_ATTR_ACK;				//被动回发的数据，Bit7位为1
		sender_addr = comm_recv_buf[POS_ADDR_SEND] & 0x7F;	//去掉最高位
	}
	cmd_num_this_time = comm_recv_buf[POS_CMD_SEQ];    		//接收的数据包编号

	if(PACKAGE_ATTR_REQUEST == flag_package_attr)
	{
	 	if((cmd_num_this_time != 0) && (cmd_num_this_time == StruCommPool[sender_addr].PackageBackup.rx_cmd_seq))//同上一包数据一样的ID号，说明上次命令PC没有收到，直接回发上次的数据
			return PACKAGE_IS_RETRY_CMD;
//		else
//			cmd_num_last_time = cmd_num_this_time;
	}
	else
	{
		if((cmd_num_this_time !=0) && (cmd_num_this_time != StruCommPool[sender_addr].PackageBackup.m_nTxSeqNum))
		{
			//接受到的应答包和上次主动发出去的请求包不一致
			return PACKAGE_ERR_CMD_SEQ;
		}
		else
		{
			if(comm_recv_buf[POS_CMD_TYPE] != StruCommPool[sender_addr].PackageBackup.m_cSendBuf[POS_CMD_TYPE])
			{
				//strLog = "Recv invalid Data: ";
				for(i=0; i<(unsigned int)(cmd_rx_length); i++)	
				{
					//strTemp.Format(TEXT("0X%.2X "),m_cRecvBuf[i]);
					//strLog = strLog + strTemp;
				}
				//m_Comm.WriteSysFile(strLog);
				//strLog.Format("ACK CMD Type is not same !");
				//m_Comm.WriteSysFile(strLog);
				return PACKAGE_ERR_CMD_TYPE;
			}
		}
	}

	cmd_rx_length = comm_recv_buf[POS_CMD_LEN_H];
	cmd_rx_length <<= 8;
	cmd_rx_length +=  comm_recv_buf[POS_CMD_LEN_L];

//	nTempRxContentLength = pStruCommPackage->m_cRecvBuf[POS_CMD_LEN_H];
//	nTempRxContentLength <<= 8;
//	nTempRxContentLength +=  pStruCommPackage->m_cRecvBuf[POS_CMD_LEN_L];
//	m_nRecvDataLen = nTempRxContentLength + PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + 2;	//修正WinPcap包尾0数据填充

	if((cmd_rx_length < 1) || (cmd_rx_length > MAX_CMD_CONTENT_LENGTH)) //至少包含CMD_TYPE，即DATA0
		return PACKAGE_ERR_LENGTH;

	

	lrc_length = cmd_rx_length + PACKAGE_INFO_LENGTH;
	data_lrc = comm_recv_buf[POS_VER];
	for(i = 1; i < lrc_length; i++)
	{
		data_lrc ^= comm_recv_buf[POS_VER + i];
	}
	pos_PACKAGE_lrc = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + cmd_rx_length;
	if((data_lrc != comm_recv_buf[pos_PACKAGE_lrc])
		&& ('*' != comm_recv_buf[pos_PACKAGE_lrc])
		&& (0x00 != comm_recv_buf[pos_PACKAGE_lrc]))
		return PACKAGE_ERR_LRC;
	if(comm_recv_buf[pos_PACKAGE_lrc + 1] != MSG_ETX)
		return PACKAGE_ERR_ETX;

	return PACKAGE_IS_NEW_AND_OK;
}
*/
//解析命令并应答
//实现以太网总线广播的方式（UDP才能实现广播），所以需要更改为UDP通信的模式
#define CMD_INTERFACE_TYPE_UNKNOW   0
#define CMD_INTERFACE_TYPE_NET      1
#define CMD_INTERFACE_TYPE_SERIAL   2
#define CMD_INTERFACE_TYPE_USB		3

//void comm_cmd_execution2(void)
//{
//	if(!StruCommPool[COMPUTER].bFlagHasRxNewPackage)
//		return;
//
//	StruCommPool[BOARD1].bFlagNeedTxNewPackage = TRUE;
//	StruCommPool[BOARD1].PACKAGE.m_nTxSeqNum++;
//	StruCommPool[BOARD1].PACKAGE.tx_content_buf[0] = 1;
//	StruCommPool[BOARD1].PACKAGE.tx_content_length = 1;
//
//	StruCommPool[COMPUTER].bFlagHasRxNewPackage = 0;
//	return;
//}


int PackageMsg(unsigned char bTypePackage, unsigned char nID, unsigned char *pBuf, unsigned int nLength)
{
	int nRes = 0;
	unsigned int i = 0;
	char nValueCrc = 0;

	PACKAGE * pStruCommPackage = &StruCommPool[nID].PackageData;
	memset(pStruCommPackage->m_cSendBuf, 0, sizeof(pStruCommPackage->m_cSendBuf));

	pStruCommPackage->m_cSendBuf[POS_SYN1] = MSG_SYN1;		//0x40	同步头1
	pStruCommPackage->m_cSendBuf[POS_SYN2] = MSG_SYN2;		//0x40	同步头2
	pStruCommPackage->m_cSendBuf[POS_STX] = MSG_STX;		//0x28	起始符
	pStruCommPackage->m_cSendBuf[POS_VER] = MSG_VER;		//0x41	协议版本号
	pStruCommPackage->m_cSendBuf[POS_ADDR_RECV] = nID;			//m_nAddr;
	if(bTypePackage)	//应答
	{
		pStruCommPackage->m_cSendBuf[POS_ADDR_SEND] = board_addr | 0x80;	//源址址
		pStruCommPackage->m_cSendBuf[POS_CMD_SEQ] = pStruCommPackage->m_nRxSeqNum;		//数据包编号
	}
	else		//主动发送
	{
		pStruCommPackage->m_cSendBuf[POS_ADDR_SEND] = board_addr;		//源址址
		if(pStruCommPackage->m_nTxSeqNumLast == 0xFF)
			pStruCommPackage->m_nTxSeqNum = 1;
		pStruCommPackage->m_cSendBuf[POS_CMD_SEQ] = pStruCommPackage->m_nTxSeqNum;		//数据包编号
		pStruCommPackage->m_nTxSeqNumLast = pStruCommPackage->m_nTxSeqNum;
		pStruCommPackage->m_nTxSeqNum++;
	}

	pStruCommPackage->m_cSendBuf[POS_CMD_LEN_H] = nLength/256;
	pStruCommPackage->m_cSendBuf[POS_CMD_LEN_L] = nLength%256;

	for(i=0; i<nLength; i++)
	{
		pStruCommPackage->m_cSendBuf[POS_CMD_TYPE + i] = pBuf[POS_CMD_TYPE+i];
	}

	nValueCrc = pStruCommPackage->m_cSendBuf[POS_VER];

	for(i=1; i<(nLength+PACKAGE_INFO_LENGTH); i++)
	{
		nValueCrc = nValueCrc ^ pStruCommPackage->m_cSendBuf[POS_VER+i];
	}

	pStruCommPackage->m_cSendBuf[PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + nLength] = nValueCrc;
	pStruCommPackage->m_cSendBuf[PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + 1 + nLength] = MSG_ETX;

	nRes = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + PACKAGE_TAIL_LENGTH + nLength;
	return nRes;
}

int	CheckPackageValid(unsigned char nID)
{
	UINT i;
	UCHAR nDataLrc = 0;
	UINT nLrcLength;
	UINT nPosPACKAGELrc;
	UINT nTempRxContentLength;
	UCHAR nRxSeqNum;
//	CString strLog;
//	CString strTemp;

	PACKAGE * pStruCommPackage = &StruCommPool[nID].PackageData;

	if(pStruCommPackage->m_cRecvBuf[POS_SYN1] != MSG_SYN1)
		return PACKAGE_ERR_SYN1;
	if(pStruCommPackage->m_cRecvBuf[POS_SYN2] != MSG_SYN2)
		return PACKAGE_ERR_SYN2;
	if(pStruCommPackage->m_cRecvBuf[POS_STX] != MSG_STX)
		return PACKAGE_ERR_STX;  
	if(pStruCommPackage->m_cRecvBuf[POS_VER] != MSG_VER)
		return PACKAGE_ERR_VER; 	
	//if((pStruCommPackage->m_cRecvBuf[POS_ADDR_RECV] != m_nAddr) &&		//是否为本设备的数据，若不是则丢弃
	//	(pStruCommPackage->m_cRecvBuf[POS_ADDR_RECV] != ADDR_BOARDCAST))
	//	return PACKAGE_ERR_RECV_ADDR; 

	//m_nAddrSender = pStruCommPackage->m_cRecvBuf[POS_ADDR_SEND];			//发送方地址
	//m_nCmdNumThisTime = pStruCommPackage->m_cRecvBuf[POS_CMD_NUM]; 
	 
	sender_addr = pStruCommPackage->m_cRecvBuf[POS_ADDR_SEND]; 
	if(!(sender_addr & 0x80))						
	{
		flag_package_attr = PACKAGE_ATTR_REQUEST; 			//主动请求的数据，Bit7位为0
	}
	else
	{
		flag_package_attr = PACKAGE_ATTR_ACK;				//被动回发的数据，Bit7位为1
		sender_addr = pStruCommPackage->m_cRecvBuf[POS_ADDR_SEND] & 0x7F;	//去掉最高位
	}

	nRxSeqNum = pStruCommPackage->m_cRecvBuf[POS_CMD_SEQ];		
	if(PACKAGE_ATTR_REQUEST == flag_package_attr)
	{
	 	if((nRxSeqNum != 0) && (nRxSeqNum == pStruCommPackage->m_nRxSeqNum))//同上一包数据一样的ID号，说明上次命令PC没有收到，直接回发上次的数据
			return PACKAGE_IS_RETRY_CMD;
		else
		{
			pStruCommPackage->m_nRxSeqNum = nRxSeqNum;
			//pStruCommPackage->m_nRxSeqNumLast = nRxSeqNum;
		}
	}
	else
	{
		if((nRxSeqNum !=0) && (nRxSeqNum != pStruCommPackage->m_nTxSeqNumLast))
		{
			//接受到的应答包和上次主动发出去的请求包不一致
			return PACKAGE_ERR_CMD_SEQ;
		}
		else
		{
			if(pStruCommPackage->m_cRecvBuf[POS_CMD_TYPE] != pStruCommPackage->m_cSendBuf[POS_CMD_TYPE])
			{
				//strLog = "Recv invalid Data: ";
				//for(i=0; i<(unsigned int)(cmd_rx_length); i++)	
				{
					//strTemp.Format(TEXT("0X%.2X "),m_cRecvBuf[i]);
					//strLog = strLog + strTemp;
				}
				//m_Comm.WriteSysFile(strLog);
				//strLog.Format("ACK CMD Type is not same !");
				//m_Comm.WriteSysFile(strLog);
				return PACKAGE_ERR_CMD_TYPE;
			}
		}
	}		 

	nTempRxContentLength = pStruCommPackage->m_cRecvBuf[POS_CMD_LEN_H];
	nTempRxContentLength <<= 8;
	nTempRxContentLength +=  pStruCommPackage->m_cRecvBuf[POS_CMD_LEN_L];

	pStruCommPackage->m_nLenRx = nTempRxContentLength + PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + 2;	//修正WinPcap包尾0数据填充

	if((nTempRxContentLength < 1) || (nTempRxContentLength > MAX_CMD_CONTENT_LENGTH)) //至少包含CMD_TYPE，即DATA0
		return PACKAGE_ERR_LENGTH;

	if(PACKAGE_ATTR_ACK == flag_package_attr)
	{
		if(pStruCommPackage->m_cRecvBuf[POS_CMD_TYPE] != pStruCommPackage->m_cSendBuf[POS_CMD_TYPE])
		{
			//strLog = "Recv invalid Data: ";
			//for(i=0; i<(unsigned int)(pStruCommPackage->m_nLenRx); i++)	
			{
				//strTemp.Format(TEXT("0X%.2X "),m_cRecvBuf[i]);
				//strLog = strLog + strTemp;
			}
			//m_Comm.WriteSysFile(strLog);
			//strLog.Format("ACK CMD Type is not same !");
			//m_Comm.WriteSysFile(strLog);
			return PACKAGE_ERR_CMD_TYPE;
		}
	}

	nLrcLength = nTempRxContentLength + PACKAGE_INFO_LENGTH;
	nDataLrc = pStruCommPackage->m_cRecvBuf[POS_VER];
	for(i = 1; i < nLrcLength; i++)
	{
		nDataLrc ^= pStruCommPackage->m_cRecvBuf[POS_VER + i];
	}

	nPosPACKAGELrc = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + nTempRxContentLength;
	if((nDataLrc != pStruCommPackage->m_cRecvBuf[nPosPACKAGELrc])
		&& ('*' != pStruCommPackage->m_cRecvBuf[nPosPACKAGELrc])
		&& (0x00 != pStruCommPackage->m_cRecvBuf[nPosPACKAGELrc]))
		return PACKAGE_ERR_LRC;

	if(pStruCommPackage->m_cRecvBuf[nPosPACKAGELrc + 1] != MSG_ETX)
		return PACKAGE_ERR_ETX;

	return PACKAGE_IS_NEW_AND_OK;
}


unsigned char DistrubDataToGroup(unsigned char nConnectType)
{
	unsigned char nAddr;
	unsigned int nLen = 0;
	if (nConnectType==CMD_INTERFACE_TYPE_NET)
	{
		nLen = net_cmd_data_len;
	}
	else if (nConnectType==CMD_INTERFACE_TYPE_SERIAL)
	{
		nLen = serial_cmd_data_len;
	}
	nAddr = comm_recv_buf[POS_ADDR_SEND] & 0x7F;
	memcpy(&StruCommPool[nAddr].PackageData.m_cRecvBuf,comm_recv_buf,nLen);
	StruCommPool[nAddr].PackageData.m_nLenRx = nLen; 
	return nAddr;
}


void comm_cmd_execution(void)
{
	int cmd_is_valid_flag = 0;

//	static unsigned char need_send_msg = 0;
	static unsigned char cmd_interface_type = CMD_INTERFACE_TYPE_UNKNOW;
	static unsigned long baseTime;
	static unsigned char operation = STEP1;		//将此函数拆分成几步，提高CPU对外设的响应速度
	static unsigned char addr;

	char tmp_str[10];	

//	if(check_time_delay(last_cmd_time, 2000))	//超过2秒无新的数据包过来，认为设备断线
//	{
//		device_status = DEVICE_STATUS_DISCONNECT;
//		device_connected = FALSE;
//        //此处可以加入网卡重新初始化，防止网络芯片死掉
//	}
	switch(operation)
	{
		case STEP1:				
			if(net_cmd_recv_flag == TRUE)		//网络TCP或UDP接收到有效的数据包或者串口收到有效数据包
			{
			    net_cmd_busy_flag = true;
			    net_cmd_recv_flag = 0;
			    cmd_interface_type = CMD_INTERFACE_TYPE_NET;    
				last_cmd_time = systemTime;					
				device_connected = TRUE;					
				operation = STEP2;							
			}
			else if(serial_cmd_recv_flag == TRUE) //串口
			{
			    serial_cmd_busy_flag = true;
			    serial_cmd_recv_flag = 0;
			    cmd_interface_type = CMD_INTERFACE_TYPE_SERIAL;
				last_cmd_time = systemTime;
				device_connected = TRUE;
				operation = STEP2;				    
			}
//			if(usb_cmd_recv_flag)
//			{
//			    usb_cmd_busy_flag = true;
//			    usb_cmd_recv_flag = 0;
//			    cmd_interface_type = CMD_INTERFACE_TYPE_USB; 
//				last_cmd_time = systemTime;
//				device_connected = TRUE;
//				operation = STEP2;   
//			}												
		break;
		case STEP2:
			//先讲数据转移至相关的缓冲组DistrubDataToGroup();
			//然后再调用新的CheckPackageValid来判定函数
			addr = DistrubDataToGroup(cmd_interface_type);
			cmd_is_valid_flag = CheckPackageValid(addr);

			//cmd_is_valid_flag = comm_cmd_check_valid();

			if(cmd_is_valid_flag == PACKAGE_IS_NEW_AND_OK)		//新的数据包
			{
				//debug_info("A new PACKAGE!\n");
                //memset(tmp_str, 0, sizeof(tmp_str));
                //sprintf(tmp_str,"%d",cmd_num_this_time);
                //debug_info(tmp_str);                
                operation = STEP3;
			}
			else if(cmd_is_valid_flag == PACKAGE_IS_RETRY_CMD)   //重发的数据包（因为comm_send_buf仍保留上次发送的数据，所以直接再次发送即可）
			{
				// do nothing;
				//这里不做任何解释，因为底层已经做过一次并且应答过一次了，只是上层没收到应答
				//底层不重复解析，只把上次解析的结果给上层再应答一次。
				//debug_info("\nA Re_send PACKAGE\n");
				//need_send_msg = 1;                
				//operation = STEP5;
		
				memcpy(comm_send_buf, StruCommPool[sender_addr].PackageBackup.m_cSendBuf, StruCommPool[sender_addr].PackageBackup.m_nLenTx);
				operation = STEP5;
			}
			else //if(cmd_is_valid_flag >= PACKAGE_IS_NOT_VALID)	//数据包不合法，如何处理？不做任何响应！
			{              
				//debug_info("CMD is not valid:\n");
				memset(tmp_str, 0, sizeof(tmp_str));
				sprintf(tmp_str, "E%d!\n", cmd_is_valid_flag);
				//debug_info(tmp_str);
				if(cmd_interface_type == CMD_INTERFACE_TYPE_NET)
				    net_cmd_busy_flag = FALSE;
				else if(cmd_interface_type == CMD_INTERFACE_TYPE_SERIAL)
				    serial_cmd_busy_flag = FALSE;
//				else if(cmd_interface_type == CMD_INTERFACE_TYPE_USB)
//				    usb_cmd_busy_flag = FALSE;								    
				operation = STEP1;
			}	
			break;
		case STEP3:	//新的数据包，要进行解析并做相关处理
			if(PACKAGE_ATTR_REQUEST == flag_package_attr)
			{
				//外部发过来的请求包
				//memset(comm_send_buf, 0 , PACKAGE_SIZE);
//				need_send_msg = 1;
				bRecvData = true;
	        	//ProcessCmd();     //放在外面去执行
				operation = STEP4;
			}
			else
			{
				//外部发过来的应答包
			  	if(!StruCommPool[sender_addr].bFlagHasRxNewPackage) 	//上次接收的数据已处理
				{
					StruCommPool[sender_addr].bFlagHasRxNewPackage = TRUE;
					//StruCommPool[sender_addr].PackageData.m_nRxSeqNum = cmd_num_this_time;
					//比较应答的数据包和发送的数据包是否包号一致
					
//					if(cmd_is_valid_flag == PACKAGE_IS_NEW_AND_OK)
//					{
//						//memcpy(&StruCommPool[sender_addr].rx_content_buf, &comm_recv_buf[POS_CMD_TYPE], cmd_rx_length);	//拷贝内容到适当的内存空间存储
//						StruCommPool[sender_addr].PackageData.m_nRxSeqNum = cmd_num_this_time;
//						StruCommPool[sender_addr].PackageData.m_nRecvDataLen = cmd_rx_length;
//						memcpy(StruCommPool[sender_addr].PackageData.rx_content_buf, &comm_recv_buf[POS_CMD_TYPE], cmd_rx_length);	//拷贝内容到适当的内存空间存储					
//						if(sender_addr == 0)	//COMPUTER发过来的数据，需要保存其MAC地址和IP地址
//						{
//							last_cmd_time = systemTime;
//							device_connected = TRUE;
//							memcpy(StruCommPool[COMPUTER].mac_addr, comm_mac_addr, 6);
//							memcpy(StruCommPool[COMPUTER].ip_addr, comm_ip_addr, 4);
//							memcpy(StruCommPool[COMPUTER].net_port, comm_net_port, 2);							
//						}
//					}
					net_cmd_busy_flag = FALSE;
					operation = STEP1;
				}
				else //if(timer(baseTime, COMM_TIMEOUT))
				{
					//为什么上次的数据还没有处理？理论上应该不会出现这个问题!			
				}
			}
			break;
		case STEP4:
			//if(need_send_msg)				//对于新的需要应答的数据包，则需要进行组包流程
			{
				//comm_cmd_make_PACKAGE();
				//net_cmd_send_length = serial_cmd_send_length = usb_cmd_send_length = PackageMsg(1, addr, comm_send_buf, txlen+1);
				StruCommPool[sender_addr].PackageData.m_nLenTx = PackageMsg(1, addr, comm_send_buf, txlen+1);
				memcpy(comm_send_buf, StruCommPool[sender_addr].PackageData.m_cSendBuf, StruCommPool[sender_addr].PackageData.m_nLenTx);
				memcpy(&StruCommPool[sender_addr].PackageBackup, &StruCommPool[sender_addr].PackageData, sizeof(PACKAGE) );

				net_cmd_send_length = serial_cmd_send_length = StruCommPool[sender_addr].PackageData.m_nLenTx; 
			}
			operation = STEP5;
			break;
		case STEP5:
			//need_send_msg = 0;
			baseTime = systemTime;
			if(cmd_interface_type == CMD_INTERFACE_TYPE_NET)	
				operation = STEP6;
			else if(cmd_interface_type == CMD_INTERFACE_TYPE_SERIAL)
				operation = STEP7;
			else if(cmd_interface_type == CMD_INTERFACE_TYPE_USB)
				operation = STEP8;				
			else
			{
				//debug_info("Error!");
				net_cmd_busy_flag = FALSE;
				operation = STEP1;
			}
			break;
		case STEP6:	//网络应答
			net_cmd_send_flag = 1;
			//net_cmd_send_length = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + cmd_tx_length + PACKAGE_TAIL_LENGTH;
			ethernet_send_msg();			
			net_cmd_busy_flag = FALSE;
			//debug_info("Ack!");
			operation = STEP1;
			break;
		case STEP7:	//串口应答
			if(serial_send_finished_flag)
			{
				//serial_cmd_send_length = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + cmd_tx_length + PACKAGE_TAIL_LENGTH;
				serial_send_msg();				
				operation = STEP1;
			}
			else if(check_time_delay(baseTime, 1000))
			{
				operation = STEP1;
				debug_info("timeout1\n");
			}
			break;
		case STEP8:	//USB应答
//			if(usb_send_finished_flag)
//			{
//				//usb_cmd_send_length = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + cmd_tx_length + PACKAGE_TAIL_LENGTH;
//				usb_send_msg();
//				operation = STEP1;
//			}
//			else if(timer(baseTime, 1000))
//			{
//				operation = STEP1;
//				debug_info("timeout2\n");
//			}
			break;
		default:
			break;
	}
}

//对接收到的数据进行处理
//    1)1~99号命令为单动动作命令
//    2)100号命令为指定某命令号的单动动作指令，形式为（100号命令+指定动作命令号+该动作命令附带参数）
//    3)101~199号命令为用户自由定义命令，在UserCode的Command下command.c的ProcessSUserCmd进行解析
//    4)200~255号命令为系统级别命令，在BSP的Command下bsp_command.c的ProcessSystemCmd进行解析
void ProcessCmd(void)
{
	unsigned char i;
	unsigned char crc=0;

	if(bRecvData)
	{
		bRecvData = false;
//		comm_recv_buf[POS_CMD_TYPE] = 210;
		if(comm_recv_buf[POS_CMD_TYPE] <= CMD_OPERATION_END)    //1~99,单动命令
		{
            memcpy(cParamBuf,comm_recv_buf,sizeof(cParamBuf));  //备份参数，所有的参数都是从cParamBuf[POS_DATA_0]开始读取的
			gMachineOperation[comm_recv_buf[POS_CMD_TYPE]]->nParameter = comm_recv_buf[POS_DATA_0]<<8 + comm_recv_buf[POS_DATA_0+1];
            if(StartOp(gMachineOperation[comm_recv_buf[POS_CMD_TYPE]]))   //动作成功执行
			{
				comm_send_buf[10]=0xE0;
			}
			else   //动作执行失败
			{
				comm_send_buf[10]=0xE1;
			}
			txlen=1;
		}
		else if(CMD_DO_OPERATION == comm_recv_buf[POS_CMD_TYPE])  //100单动动作指令，形式为（命令+动作命令号+该动作命令附带参数）
		{
            memcpy(cParamBuf,&comm_recv_buf[1],sizeof(cParamBuf));  //为了和上面保持兼容，此处加了偏移1，所有的参数都是从cParamBuf[POS_DATA_0]开始读取的

			if(StartOp(gMachineOperation[comm_recv_buf[POS_DATA_0]]))   //动作成功执行
			{
				comm_send_buf[10]=0xE0;
			}
			else   //动作执行失败
			{
				comm_send_buf[10]=0xE1;
			}
			txlen=1;
		}
/*		else if (comm_recv_buf[POS_CMD_TYPE] == 205)
		{
			comm_send_buf[POS_DATA_0+0] = m_Input_All_now & 0xFF;
			comm_send_buf[POS_DATA_0+1] = (m_Input_All_now>>8) & 0xFF;
			comm_send_buf[POS_DATA_0+2] = (m_Input_All_now>>16) & 0xFF;
			comm_send_buf[POS_DATA_0+3] = (m_Input_All_now>>24) & 0xFF;
			txlen = 4;
		}*/
		else
		{
			if(comm_recv_buf[POS_CMD_TYPE] >= CMD_SYSTEM_BASE)   //200~255系统固有命令
			{
				ProcessSystemCmd();
			}
			else   //用户要修改的部分
			{
				ProcessUserCmd();
			}
		}
		comm_send_buf[0]=MSG_SYN1;
		comm_send_buf[1]=MSG_SYN2;
		comm_send_buf[2]=MSG_STX;
		comm_send_buf[3]=MSG_VER;
		comm_send_buf[4]=sender_addr;
		comm_send_buf[5]=board_addr;
		comm_send_buf[6]=cmd_num_this_time;
		comm_send_buf[7]=(txlen+1)/256;
		comm_send_buf[8]=(txlen+1)%256;
		comm_send_buf[POS_CMD_TYPE]  = comm_recv_buf[POS_CMD_TYPE];
		for(i=0;i<(txlen+7);i++)
		{
			crc=crc^comm_send_buf[3+i];  //计算检验
		}
		comm_send_buf[10+txlen]=crc;
		comm_send_buf[11+txlen]=MSG_ETX;   //结束
	}
}

struct StruTarget
{
	char m_nId;
	unsigned char mac_addr[6];				//MAC地址
	unsigned char ip_addr[4];				//IP地址
	unsigned char net_port;					//端口地址
};

//#define MAX_LENGTH	128
//typedef struct  
//{
//	unsigned char m_nSeqNum;			//通讯序列号
//	unsigned char m_nSeqNumLast;		//通讯序列号历史
//	//unsigned int m_nVersionLength;	//固件长度
//	unsigned int m_nRecvDataLen;				//接收数据长度
//	unsigned char m_cSendBuf[MAX_LENGTH];		//发送缓冲区
//	unsigned char m_cSendBufSave[MAX_LENGTH];	//发送缓冲区发送失败时的备份
//	unsigned char m_cSendBufLast[MAX_LENGTH];	//发送缓冲区历史
//	unsigned char m_cRecvBuf[MAX_LENGTH];		//接收缓冲区
//	unsigned char m_cRecvBufLast[MAX_LENGTH];	//接收缓冲区历史
//	unsigned char m_nCommRetryCnt;			//通信重试
//	unsigned int m_nTotalFailure;	//总失败次数
//	unsigned int m_nReadTimeOut;	//读取数据时超时时间
//
//	DWORD m_dwQuerySensorTimeLast;
//	//DWORD m_dwQueryStatusTimeLast;
//	DWORD m_dwQueryActionTimeLast;
//}STRU_COMM_PCAKAGE;

//STRU_COMM_PCAKAGE StruCommPACKAGE[2];

//int SendData(unsigned char nID, unsigned char *pBuf, unsigned int nLength)
//{
//	unsigned int i = 0;
//	int nRet = 0;
//	int nLen = 0;
//	//CString strLog, strTemp;
//	BOOL bNeedRetry = FALSE;
//	unsigned int nLenSave = 0;
//
////	if((m_bNetIsConnect == FALSE) && (COM_TYPE_NET == m_nCommunicationMode))
////		return -1;
//
//	BOOL  bSame=TRUE;
//
//	static unsigned char operation = STEP1;
//	static unsigned long baseTime;
//
//	STRU_COMM_PCAKAGE * pStruCommPackage;
//
//	switch(operation)
//	{
//		case STEP1:		//有信息待发送？
//			operation = STEP2;
//			break;
//		case STEP2:
//			pStruCommPackage = &StruCommPACKAGE[nID];  	//产生数据包
//			nLen = PackageMsg(nID, pBuf, nLength);
//			operation = STEP3;
//			break;
//		case STEP3:
////	   		ethernet_send_msg_winpcap(
////					pStru_Board->mac_addr,
////					dst_mac2,
////					pStru_Board->tx_content_buf,
////					pStru_Board->tx_content_length);
//
//			baseTime = systemTime;
//			operation = STEP4;
//			break;
//		case STEP4:
////			pStru_Board->nStatusCommunication = STATUS_COMM_WAIT_RECV;
////			if(pStru_Board->bFlagHasRxNewPackage == TRUE)
////			{
////				operation = STEP5;
////			}
////			else if(check_time_delay(baseTime, COMM_TIMEOUT))	//超时未收到应答
////			{
////				pStru_Board->retry_comm_counter ++;
////				if(pStru_Board->retry_comm_counter >= MAX_RETRY_COUNTER)
////				{
////					pStru_Board->bFlagError = 1;
////					pStru_Board->retry_comm_counter = 0;
////					pStru_Board->nStatusCommunication = STATUS_COMM_ERROR;
////					operation = STEP1;	
////				}
////			}
//			break;
//		case STEP5:		//置位通讯完成标志
////			pStru_Board->nStatusCommunication = STATUS_COMM_RECV_FINISH;
////			operation = STEP6;
//			break;
//		}
//
//		
////	pStruCommPackage->m_nRecvDataLen = 0;
////	//m_nCommRetryCnt = 0;
////
////
////	//发送消息
////	m_nReadTimeOut = 100; 
////	nRet = SendData((const char *)m_cSendBuf, nLen, m_nReadTimeOut);
////
////	
////	//开始接收数据
////	memset(pStruCommPackage->m_cRecvBuf, 0, sizeof(pStruCommPackage->m_cRecvBuf));
////	pStruCommPackage->m_nRecvDataLen = 0;//读取的字节数
////
////	m_nReadTimeOut = 100;
////	if((unsigned char)(pStruCommPackage->m_cSendBuf[POS_CMD_TYPE]) == CMD_SET_PARAMETER) //下载参数数据，CPU先保存数据再返回，时间稍长，特殊处理
////		m_nReadTimeOut = 1000;
////	if(COM_TYPE_NET == m_nCommunicationMode)
////		pStruCommPackage->m_nRecvDataLen =  ReadData((char *)m_cRecvBuf, MAX_LENGTH, m_nReadTimeOut);
////	else
////		return -1;
////
//	return nRet;
//}


//int SendData2(unsigned char nID, unsigned char *pBuf, unsigned int nLength)
//{
//	int nLen = 0;
//	nLen = PackageMsg(0, nID, pBuf, nLength);
//	//SendData((const char *)m_cSendBuf, nLen, m_nReadTimeOut);
//	return nLen;
//}

//unsigned char dst_computer[] = {0xAA,0xAA,0xAA,0xAA,0xAA,10};
//unsigned char dst_mac1[] = {0xAA,0xAA,0xAA,0xAA,0xAA,11};
//unsigned char dst_mac2[] = {0xAA,0xAA,0xAA,0xAA,0xAA,12};
//unsigned char dst_mac3[] = {0xAA,0xAA,0xAA,0xAA,0xAA,13};
//unsigned char dst_mac4[] = {0xAA,0xAA,0xAA,0xAA,0xAA,14};
//unsigned char dst_mac5[] = {0xAA,0xAA,0xAA,0xAA,0xAA,15};
//unsigned char dst_mac6[] = {0xAA,0xAA,0xAA,0xAA,0xAA,16};
//unsigned char dst_mac7[] = {0xAA,0xAA,0xAA,0xAA,0xAA,17};
//unsigned char dst_mac8[] = {0xAA,0xAA,0xAA,0xAA,0xAA,18};
//unsigned char dst_mac9[] = {0xAA,0xAA,0xAA,0xAA,0xAA,19};

unsigned char dst_mac[][6] ={
	{0xAA,0xAA,0xAA,0xAA,0xAA,10},
	{0xAA,0xAA,0xAA,0xAA,0xAA,11},
	{0xAA,0xAA,0xAA,0xAA,0xAA,12},
	{0xAA,0xAA,0xAA,0xAA,0xAA,13},
	{0xAA,0xAA,0xAA,0xAA,0xAA,14},
	{0xAA,0xAA,0xAA,0xAA,0xAA,15},
	{0xAA,0xAA,0xAA,0xAA,0xAA,16},
	{0xAA,0xAA,0xAA,0xAA,0xAA,17},
	{0xAA,0xAA,0xAA,0xAA,0xAA,18},
	{0xAA,0xAA,0xAA,0xAA,0xAA,19}
};

unsigned char ip_addr[][4] ={
	{192,168,200,10},
	{192,168,200,11},
	{192,168,200,12},
	{192,168,200,13},
	{192,168,200,14},
	{192,168,200,15},
	{192,168,200,16},
	{192,168,200,17},
	{192,168,200,18},
	{192,168,200,19}
};

STRU_COMMUNICATE_WITH_BOARD StruCommPool[TOTAL_BOARD];



void process_computer()
{

}

#define LED_MAX_NUM	6
#define TIME_TEST_GAP 100
#define TIME_RECV_OUT 500
#define RETRY_MAX_TIME  1
	

void process_board2()
{
	static unsigned char operation = STEP1;
	static unsigned long baseTime;
	unsigned long NowTime;
	unsigned char cBuf[100];
	unsigned char nID = 2;
	static unsigned char bStartFlag = 1;
	static bool bFlag = 0;
	static unsigned char nTestState = DM_CW;
	static unsigned char nTestNum = 0;
	static bool bFlagChange = 0;
	static unsigned char nRetry = 0;

	STRU_COMMUNICATE_WITH_BOARD *pStru_Board = &StruCommPool[nID];

	pStru_Board->bFlagNeedTxNewPackage = bStartFlag;
	switch(operation)
	{
		case STEP1:		//有信息待发送？
			pStru_Board->nStatusCommunication = STATUS_COMM_FREE;
			if(pStru_Board->bFlagNeedTxNewPackage)
			{
				operation = STEP2;
				pStru_Board->nStatusCommunication = STATUS_COMM_NEED_TO_SEND;
			}
			break;
		case STEP2:		//发送信息
			cBuf[POS_CMD_TYPE] = CMD_SET_DC_MOTOR;
			if(bFlagChange)
				cBuf[POS_DATA_0] = (nTestNum % LED_MAX_NUM) + 1;
			else
				cBuf[POS_DATA_0] = (LED_MAX_NUM+1) - ((nTestNum % LED_MAX_NUM) + 1);
			
			cBuf[POS_DATA_1] = nTestState;
			
			nTestNum ++;		
			if(nTestNum == LED_MAX_NUM)
			{
				bFlagChange = !bFlagChange;
				nTestNum = 0;

				if(DM_CW == nTestState)
					nTestState = DM_CCW;
				else
					nTestState = DM_CW;
			}
			pStru_Board->PackageData.m_nLenTx = PackageMsg(0, nID, cBuf,3);
			operation = STEP3;
			break;
		case STEP3:		//发送信息
			pStru_Board->bFlagHasRxNewPackage = 0;
	   		ethernet_send_msg_winpcap(
					//pStru_Board->mac_addr,
					board_mac,
					dst_mac[nID],
					pStru_Board->PackageData.m_cSendBuf,
					pStru_Board->PackageData.m_nLenTx);
			pStru_Board->bFlagNeedTxNewPackage = 0;
			pStru_Board->nStatusCommunication = STATUS_COMM_SEND_FINISH;
			baseTime = systemTime;
			operation = STEP4;
			break;
		case STEP4:
			if(pStru_Board->bFlagHasRxNewPackage)
			{
				NowTime = systemTime;
				NowTime = NowTime - baseTime;
				//debug_info("t=%dus",NowTime);
				pStru_Board->bFlagHasRxNewPackage = 0;
				//sprintf(cBuf,"%d",pStru_Board->PackageData.rx_content_buf[0]);
				//debug_info("%d",pStru_Board->PackageData.rx_content_buf[0]);

				pStru_Board->nCounterSuccess ++;
				pStru_Board->nStatusCommunication = STATUS_COMM_END;
				baseTime = systemTime;
				operation = STEP5;
				nRetry = 0;
			}
			else if(check_time_delay(baseTime, TIME_RECV_OUT))
			{
				pStru_Board->nCounterFailed ++;
				nRetry++;

				if(nRetry > RETRY_MAX_TIME)
				{
					nRetry = 0;
					pStru_Board->nStatusCommunication = STATUS_COMM_ERROR;
					led_blue_on();
				
					if(bFlag)
						dm_ctrl_one(nID, DM_CW);
					else
						dm_ctrl_one(nID, DM_CCW);
					bFlag = ! bFlag;
	
					operation = STEP5;
				}
				else
				{
					operation = STEP3;
				}				
			}
			break;
		case STEP5:
			if(check_time_delay(baseTime, TIME_TEST_GAP))
			//if(1)
			{
				debug_info("%d:S[%d]-F[%d]",nID, pStru_Board->nCounterSuccess, pStru_Board->nCounterFailed);
				operation = STEP1;
			}
			break;
		default:
			break;
	}
}

void process_board3()
{
	static unsigned char operation = STEP1;
	static unsigned long baseTime;
	unsigned long NowTime;
	unsigned char cBuf[100];
	unsigned char nID = 3;
	static unsigned char bStartFlag = 1;
	static bool bFlag = 0;
	static unsigned char nTestState = DM_CW;
	static unsigned char nTestNum = 0;
	static bool bFlagChange = 0;
	static unsigned char nRetry = 0;

	STRU_COMMUNICATE_WITH_BOARD *pStru_Board = &StruCommPool[nID];

	pStru_Board->bFlagNeedTxNewPackage = bStartFlag;
	switch(operation)
	{
		case STEP1:		//有信息待发送？
			pStru_Board->nStatusCommunication = STATUS_COMM_FREE;
			if(pStru_Board->bFlagNeedTxNewPackage)
			{
				operation = STEP2;
				pStru_Board->nStatusCommunication = STATUS_COMM_NEED_TO_SEND;
			}
			break;
		case STEP2:		//发送信息
			cBuf[POS_CMD_TYPE] = CMD_SET_DC_MOTOR;
			if(bFlagChange)
				cBuf[POS_DATA_0] = (nTestNum % LED_MAX_NUM) + 1;
			else
				cBuf[POS_DATA_0] = (LED_MAX_NUM+1) - ((nTestNum % LED_MAX_NUM) + 1);
			
			cBuf[POS_DATA_1] = nTestState;
			
			nTestNum ++;		
			if(nTestNum == LED_MAX_NUM)
			{
				bFlagChange = !bFlagChange;
				nTestNum = 0;

				if(DM_CW == nTestState)
					nTestState = DM_CCW;
				else
					nTestState = DM_CW;
			}
			pStru_Board->PackageData.m_nLenTx = PackageMsg(0, nID, cBuf,3);
			operation = STEP3;
			break;
		case STEP3:		//发送信息
			pStru_Board->bFlagHasRxNewPackage = 0;
	   		ethernet_send_msg_winpcap(
					//pStru_Board->mac_addr,
					board_mac,
					dst_mac[nID],
					pStru_Board->PackageData.m_cSendBuf,
					pStru_Board->PackageData.m_nLenTx);
			pStru_Board->bFlagNeedTxNewPackage = 0;
			pStru_Board->nStatusCommunication = STATUS_COMM_SEND_FINISH;
			baseTime = systemTime;
			operation = STEP4;
			break;
		case STEP4:
			if(pStru_Board->bFlagHasRxNewPackage)
			{
				NowTime = systemTime;
				NowTime = NowTime - baseTime;
				//debug_info("t=%dus",NowTime);
				pStru_Board->bFlagHasRxNewPackage = 0;
				//sprintf(cBuf,"%d",pStru_Board->PackageData.rx_content_buf[0]);
				//debug_info("%d",pStru_Board->PackageData.rx_content_buf[0]);

				pStru_Board->nCounterSuccess ++;
				pStru_Board->nStatusCommunication = STATUS_COMM_END;
				baseTime = systemTime;
				operation = STEP5;
				nRetry = 0;
			}
			else if(check_time_delay(baseTime, TIME_RECV_OUT))
			{
				pStru_Board->nCounterFailed ++;
				nRetry++;

				if(nRetry > RETRY_MAX_TIME)
				{
					nRetry = 0;
					pStru_Board->nStatusCommunication = STATUS_COMM_ERROR;
					led_blue_on();
				
					if(bFlag)
						dm_ctrl_one(nID, DM_CW);
					else
						dm_ctrl_one(nID, DM_CCW);
					bFlag = ! bFlag;
	
					operation = STEP5;
				}
				else
				{
					operation = STEP3;
				}				
			}
			break;
		case STEP5:
			if(check_time_delay(baseTime, TIME_TEST_GAP))
			//if(1)
			{
				debug_info("%d:S[%d]-F[%d]",nID, pStru_Board->nCounterSuccess, pStru_Board->nCounterFailed);
				operation = STEP1;
			}
			break;
		default:
			break;
	}
}

void process_board4()
{
	static unsigned char operation = STEP1;
	static unsigned long baseTime;
	unsigned long NowTime;
	unsigned char cBuf[100];
	unsigned char nID = 4;
	static unsigned char bStartFlag = 1;
	static bool bFlag = 0;
	static unsigned char nTestState = DM_CW;
	static unsigned char nTestNum = 0;
	static bool bFlagChange = 0;
	static unsigned char nRetry = 0;

	STRU_COMMUNICATE_WITH_BOARD *pStru_Board = &StruCommPool[nID];

	pStru_Board->bFlagNeedTxNewPackage = bStartFlag;
	switch(operation)
	{
		case STEP1:		//有信息待发送？
			pStru_Board->nStatusCommunication = STATUS_COMM_FREE;
			if(pStru_Board->bFlagNeedTxNewPackage)
			{
				operation = STEP2;
				pStru_Board->nStatusCommunication = STATUS_COMM_NEED_TO_SEND;
			}
			break;
		case STEP2:		//发送信息
			cBuf[POS_CMD_TYPE] = CMD_SET_DC_MOTOR;
			if(bFlagChange)
				cBuf[POS_DATA_0] = (nTestNum % LED_MAX_NUM) + 1;
			else
				cBuf[POS_DATA_0] = (LED_MAX_NUM+1) - ((nTestNum % LED_MAX_NUM) + 1);
			
			cBuf[POS_DATA_1] = nTestState;
			
			nTestNum ++;		
			if(nTestNum == LED_MAX_NUM)
			{
				bFlagChange = !bFlagChange;
				nTestNum = 0;

				if(DM_CW == nTestState)
					nTestState = DM_CCW;
				else
					nTestState = DM_CW;
			}
			pStru_Board->PackageData.m_nLenTx = PackageMsg(0, nID, cBuf,3);
			operation = STEP3;
			break;
		case STEP3:		//发送信息
			pStru_Board->bFlagHasRxNewPackage = 0;
	   		ethernet_send_msg_winpcap(
					//pStru_Board->mac_addr,
					board_mac,
					dst_mac[nID],
					pStru_Board->PackageData.m_cSendBuf,
					pStru_Board->PackageData.m_nLenTx);
			pStru_Board->bFlagNeedTxNewPackage = 0;
			pStru_Board->nStatusCommunication = STATUS_COMM_SEND_FINISH;
			baseTime = systemTime;
			operation = STEP4;
			break;
		case STEP4:
			if(pStru_Board->bFlagHasRxNewPackage)
			{
				NowTime = systemTime;
				NowTime = NowTime - baseTime;
				//debug_info("t=%dus",NowTime);
				pStru_Board->bFlagHasRxNewPackage = 0;
				//sprintf(cBuf,"%d",pStru_Board->PackageData.rx_content_buf[0]);
				//debug_info("%d",pStru_Board->PackageData.rx_content_buf[0]);

				pStru_Board->nCounterSuccess ++;
				pStru_Board->nStatusCommunication = STATUS_COMM_END;
				baseTime = systemTime;
				operation = STEP5;
				nRetry = 0;
			}
			else if(check_time_delay(baseTime, TIME_RECV_OUT))
			{
				pStru_Board->nCounterFailed ++;
				nRetry++;

				if(nRetry > RETRY_MAX_TIME)
				{
					nRetry = 0;
					pStru_Board->nStatusCommunication = STATUS_COMM_ERROR;
					led_blue_on();
				
					if(bFlag)
						dm_ctrl_one(nID, DM_CW);
					else
						dm_ctrl_one(nID, DM_CCW);
					bFlag = ! bFlag;
	
					operation = STEP5;
				}
				else
				{
					operation = STEP3;
				}				
			}
			break;
		case STEP5:
			if(check_time_delay(baseTime, TIME_TEST_GAP))
			//if(1)
			{
				debug_info("%d:S[%d]-F[%d]",nID, pStru_Board->nCounterSuccess, pStru_Board->nCounterFailed);
				operation = STEP1;
			}
			break;
		default:
			break;
	}
}

//void process_board1_rx()
//{
//	static unsigned char operation = STEP1;
//	static unsigned long baseTime;
//	int nRet = 0;
//
//	static STRU_COMMUNICATE_WITH_BOARD *pStru_Board = &StruCommPool[1];
//
//	switch(operation)
//	{
//		case STEP1:
//			if(pStru_Board->bFlagRequestByOther)	//被对方请求
//			{
//				//被对方请求的新的数据包
//					
//			}
//			break;	
//		default:
//			break;
//	}
//}
//
//void process_board1_send()
//{
//	static unsigned char operation = STEP1;
//	static unsigned long baseTime;
//	int nRet = 0;
//	static unsigned char nID = 1;
//
// 	//PACKAGE * pStruCommPackage = &StruCommPool[nID];
//	static STRU_COMMUNICATE_WITH_BOARD *pStru_Board = &StruCommPool[1];
//
//	switch(operation)
//	{
//		case STEP1:		//有信息待发送？
//			operation = STEP2;
//			break;
//		case STEP2:
//			//pStruCommPackage = &StruCommPACKAGE[nID];  	//产生数据包
//			PackageMsg(0, nID, pStru_Board->PackageData.tx_content_buf, pStru_Board->PackageData.tx_content_length);
//			operation = STEP3;
//			break;
//		case STEP3:
//			pStru_Board->bFlagAckByOther = 0;
//
//	   		ethernet_send_msg_winpcap(
//					pStru_Board->mac_addr,
//					dst_mac2,
//					pStru_Board->PackageData.m_cSendBuf,
//					pStru_Board->PackageData.m_nLenTx);
//
//			baseTime = systemTime;
//			operation = STEP4;
//			break;
//		case STEP4:
//			pStru_Board->nStatusCommunication = STATUS_COMM_WAIT_RECV;
//			if(pStru_Board->bFlagAckByOther == TRUE)
//			{
//				operation = STEP5;
//			}
//			else if(check_time_delay(baseTime, COMM_TIMEOUT))	//超时未收到应答
//			{
//				pStru_Board->retry_comm_counter ++;
//				if(pStru_Board->retry_comm_counter >= MAX_RETRY_COUNTER)
//				{
//					pStru_Board->bFlagError = 1;
//					pStru_Board->retry_comm_counter = 0;
//					pStru_Board->nStatusCommunication = STATUS_COMM_ERROR;
//					operation = STEP1;	
//				}
//			}
//			break;
//		case STEP5:		//置位通讯完成标志
//			pStru_Board->nStatusCommunication = STATUS_COMM_RECV_FINISH;
//			operation = STEP6;
//			break;	
//		default:
//			break;
//	}
////	return nRet;
//}

void process_all_cmd(void)
{
	process_computer();
//	process_board1();

//	process_board2();
//	process_board3();
//	process_board4(); 

//	process_board5();
}
