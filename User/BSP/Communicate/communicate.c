//Tab size: 4;	Indent size: 4

#include <string.h>
#include <stdio.h>

#include "bsp.h"

unsigned char bRecvData=false;  //�յ���һ������
unsigned int txlen=0;           //��Ҫ���͵����ݵĳ���
//extern bool usb_cmd_recv_flag;
//extern unsigned int usb_cmd_send_length;

bool device_connected = false;
unsigned int last_cmd_time = 0;						//�ϴ��յ����ݵ�ʱ��
unsigned char cmd_num_this_time = 0;
unsigned char cmd_num_last_time = 255;					//�õ�һ�����Ų�������ȣ��ϲ���Գ�ʼ��Ϊ0
unsigned char flag_package_attr = 0;					//0-�����ط�������������� ; 1-�����ط���������Ӧ���
unsigned char comm_recv_buf[MAX_PACKAGE_LENGTH];		//����@@��....��
unsigned char comm_send_buf[MAX_PACKAGE_LENGTH];		//����@@��....��
unsigned char comm_send_buf_last[MAX_PACKAGE_LENGTH];
unsigned int  cmd_tx_length=0;		//ֻ����cmd_type��cmd_data0~cmd_datan
unsigned int  cmd_rx_length=0;		//ֻ����cmd_type��cmd_data0~cmd_datan
unsigned char cParamBuf[512]; 		//Ŀǰ͹�����ݰ���󣬴�50���ַ����ռ����Ҫ50*3+50*4=350�ֽ�

//unsigned char comm_mac_addr[6];
//unsigned char comm_ip_addr[4];
//unsigned char comm_net_port[2];

//char WinPcapSendBuf[256];
//char WinPcapRecvBuf[256];
//void WinPcapInLanSendData(unsigned char nId,unsigned char *pBuf,unsigned int nLength)
//{
//	//m_nRetryCounter = 0;
//	//destination mac	//���ɿ��ư��MAC
//	WinPcapSendBuf[0] = 0xAA;
//	WinPcapSendBuf[1] = 0xAA;
//	WinPcapSendBuf[2] = 0xAA;
//	WinPcapSendBuf[3] = 0xAA;
//	WinPcapSendBuf[4] = 0xAA;
//	WinPcapSendBuf[5] = nId;		//���ư��MAC����
//	//source mac		//���ɱ�����MAC��ַ
//	WinPcapSendBuf[6] = m_NetAdapterInfo.Mac[0];
//	WinPcapSendBuf[7] = m_NetAdapterInfo.Mac[1];
//	WinPcapSendBuf[8] = m_NetAdapterInfo.Mac[2];
//	WinPcapSendBuf[9] = m_NetAdapterInfo.Mac[3];
//	WinPcapSendBuf[10] = m_NetAdapterInfo.Mac[4];
//	WinPcapSendBuf[11] = m_NetAdapterInfo.Mac[5];
//
//	memcpy(&WinPcapSendBuf[12], pBuf, nLength);
//	//�˴���������
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

	cmd_tx_length = txlen+1;	//Ĭ��ֻӦ�������ʾ�Ѿ������˴��¼�
	//memset(comm_send_buf, 0 , sizeof(MAX_PACKAGE_LENGTH));      //������͵�����
    comm_send_buf[POS_SYN1]	= MSG_SYN1;				
	comm_send_buf[POS_SYN2]	= MSG_SYN2;
	comm_send_buf[POS_STX]		= MSG_STX;
	comm_send_buf[POS_VER]		= MSG_VER;
	comm_send_buf[POS_ADDR_RECV] = sender_addr;
	if(1)
		comm_send_buf[POS_ADDR_SEND] = board_addr;
	else
		comm_send_buf[POS_ADDR_SEND] = board_addr | 0x80; 	//�����ط������ݻ����������͵����ݣ�����ط�Ҫ������
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

//�������ݰ���Ч�����Ƿ���ȷ
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
	//	//��ʱ���Ը�����	
	//	if((comm_recv_buf[POS_ADDR_RECV] != board_addr) &&  //�Ƿ�Ϊ���豸�����ݣ������ǣ�����δ�����ҪӦ����
	//		(comm_recv_buf[POS_ADDR_RECV] != ADDR_BOARDCAST))
	//		return PACKAGE_ERR_RECV_ADDR;
	sender_addr = comm_recv_buf[POS_ADDR_SEND]; 
	if(!(sender_addr & 0x80))						
	{
		flag_package_attr = PACKAGE_ATTR_REQUEST; 			//������������ݣ�Bit7λΪ0
	}
	else
	{
		flag_package_attr = PACKAGE_ATTR_ACK;				//�����ط������ݣ�Bit7λΪ1
		sender_addr = comm_recv_buf[POS_ADDR_SEND] & 0x7F;	//ȥ�����λ
	}
	cmd_num_this_time = comm_recv_buf[POS_CMD_SEQ];    		//���յ����ݰ����

	if(PACKAGE_ATTR_REQUEST == flag_package_attr)
	{
	 	if((cmd_num_this_time != 0) && (cmd_num_this_time == StruCommPool[sender_addr].PackageBackup.rx_cmd_seq))//ͬ��һ������һ����ID�ţ�˵���ϴ�����PCû���յ���ֱ�ӻط��ϴε�����
			return PACKAGE_IS_RETRY_CMD;
//		else
//			cmd_num_last_time = cmd_num_this_time;
	}
	else
	{
		if((cmd_num_this_time !=0) && (cmd_num_this_time != StruCommPool[sender_addr].PackageBackup.m_nTxSeqNum))
		{
			//���ܵ���Ӧ������ϴ���������ȥ���������һ��
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
//	m_nRecvDataLen = nTempRxContentLength + PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + 2;	//����WinPcap��β0�������

	if((cmd_rx_length < 1) || (cmd_rx_length > MAX_CMD_CONTENT_LENGTH)) //���ٰ���CMD_TYPE����DATA0
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
//�������Ӧ��
//ʵ����̫�����߹㲥�ķ�ʽ��UDP����ʵ�ֹ㲥����������Ҫ����ΪUDPͨ�ŵ�ģʽ
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

	pStruCommPackage->m_cSendBuf[POS_SYN1] = MSG_SYN1;		//0x40	ͬ��ͷ1
	pStruCommPackage->m_cSendBuf[POS_SYN2] = MSG_SYN2;		//0x40	ͬ��ͷ2
	pStruCommPackage->m_cSendBuf[POS_STX] = MSG_STX;		//0x28	��ʼ��
	pStruCommPackage->m_cSendBuf[POS_VER] = MSG_VER;		//0x41	Э��汾��
	pStruCommPackage->m_cSendBuf[POS_ADDR_RECV] = nID;			//m_nAddr;
	if(bTypePackage)	//Ӧ��
	{
		pStruCommPackage->m_cSendBuf[POS_ADDR_SEND] = board_addr | 0x80;	//Դַַ
		pStruCommPackage->m_cSendBuf[POS_CMD_SEQ] = pStruCommPackage->m_nRxSeqNum;		//���ݰ����
	}
	else		//��������
	{
		pStruCommPackage->m_cSendBuf[POS_ADDR_SEND] = board_addr;		//Դַַ
		if(pStruCommPackage->m_nTxSeqNumLast == 0xFF)
			pStruCommPackage->m_nTxSeqNum = 1;
		pStruCommPackage->m_cSendBuf[POS_CMD_SEQ] = pStruCommPackage->m_nTxSeqNum;		//���ݰ����
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
	//if((pStruCommPackage->m_cRecvBuf[POS_ADDR_RECV] != m_nAddr) &&		//�Ƿ�Ϊ���豸�����ݣ�����������
	//	(pStruCommPackage->m_cRecvBuf[POS_ADDR_RECV] != ADDR_BOARDCAST))
	//	return PACKAGE_ERR_RECV_ADDR; 

	//m_nAddrSender = pStruCommPackage->m_cRecvBuf[POS_ADDR_SEND];			//���ͷ���ַ
	//m_nCmdNumThisTime = pStruCommPackage->m_cRecvBuf[POS_CMD_NUM]; 
	 
	sender_addr = pStruCommPackage->m_cRecvBuf[POS_ADDR_SEND]; 
	if(!(sender_addr & 0x80))						
	{
		flag_package_attr = PACKAGE_ATTR_REQUEST; 			//������������ݣ�Bit7λΪ0
	}
	else
	{
		flag_package_attr = PACKAGE_ATTR_ACK;				//�����ط������ݣ�Bit7λΪ1
		sender_addr = pStruCommPackage->m_cRecvBuf[POS_ADDR_SEND] & 0x7F;	//ȥ�����λ
	}

	nRxSeqNum = pStruCommPackage->m_cRecvBuf[POS_CMD_SEQ];		
	if(PACKAGE_ATTR_REQUEST == flag_package_attr)
	{
	 	if((nRxSeqNum != 0) && (nRxSeqNum == pStruCommPackage->m_nRxSeqNum))//ͬ��һ������һ����ID�ţ�˵���ϴ�����PCû���յ���ֱ�ӻط��ϴε�����
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
			//���ܵ���Ӧ������ϴ���������ȥ���������һ��
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

	pStruCommPackage->m_nLenRx = nTempRxContentLength + PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + 2;	//����WinPcap��β0�������

	if((nTempRxContentLength < 1) || (nTempRxContentLength > MAX_CMD_CONTENT_LENGTH)) //���ٰ���CMD_TYPE����DATA0
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
	static unsigned char operation = STEP1;		//���˺�����ֳɼ��������CPU���������Ӧ�ٶ�
	static unsigned char addr;

	char tmp_str[10];	

//	if(check_time_delay(last_cmd_time, 2000))	//����2�����µ����ݰ���������Ϊ�豸����
//	{
//		device_status = DEVICE_STATUS_DISCONNECT;
//		device_connected = FALSE;
//        //�˴����Լ����������³�ʼ������ֹ����оƬ����
//	}
	switch(operation)
	{
		case STEP1:				
			if(net_cmd_recv_flag == TRUE)		//����TCP��UDP���յ���Ч�����ݰ����ߴ����յ���Ч���ݰ�
			{
			    net_cmd_busy_flag = true;
			    net_cmd_recv_flag = 0;
			    cmd_interface_type = CMD_INTERFACE_TYPE_NET;    
				last_cmd_time = systemTime;					
				device_connected = TRUE;					
				operation = STEP2;							
			}
			else if(serial_cmd_recv_flag == TRUE) //����
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
			//�Ƚ�����ת������صĻ�����DistrubDataToGroup();
			//Ȼ���ٵ����µ�CheckPackageValid���ж�����
			addr = DistrubDataToGroup(cmd_interface_type);
			cmd_is_valid_flag = CheckPackageValid(addr);

			//cmd_is_valid_flag = comm_cmd_check_valid();

			if(cmd_is_valid_flag == PACKAGE_IS_NEW_AND_OK)		//�µ����ݰ�
			{
				//debug_info("A new PACKAGE!\n");
                //memset(tmp_str, 0, sizeof(tmp_str));
                //sprintf(tmp_str,"%d",cmd_num_this_time);
                //debug_info(tmp_str);                
                operation = STEP3;
			}
			else if(cmd_is_valid_flag == PACKAGE_IS_RETRY_CMD)   //�ط������ݰ�����Ϊcomm_send_buf�Ա����ϴη��͵����ݣ�����ֱ���ٴη��ͼ��ɣ�
			{
				// do nothing;
				//���ﲻ���κν��ͣ���Ϊ�ײ��Ѿ�����һ�β���Ӧ���һ���ˣ�ֻ���ϲ�û�յ�Ӧ��
				//�ײ㲻�ظ�������ֻ���ϴν����Ľ�����ϲ���Ӧ��һ�Ρ�
				//debug_info("\nA Re_send PACKAGE\n");
				//need_send_msg = 1;                
				//operation = STEP5;
		
				memcpy(comm_send_buf, StruCommPool[sender_addr].PackageBackup.m_cSendBuf, StruCommPool[sender_addr].PackageBackup.m_nLenTx);
				operation = STEP5;
			}
			else //if(cmd_is_valid_flag >= PACKAGE_IS_NOT_VALID)	//���ݰ����Ϸ�����δ��������κ���Ӧ��
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
		case STEP3:	//�µ����ݰ���Ҫ���н���������ش���
			if(PACKAGE_ATTR_REQUEST == flag_package_attr)
			{
				//�ⲿ�������������
				//memset(comm_send_buf, 0 , PACKAGE_SIZE);
//				need_send_msg = 1;
				bRecvData = true;
	        	//ProcessCmd();     //��������ȥִ��
				operation = STEP4;
			}
			else
			{
				//�ⲿ��������Ӧ���
			  	if(!StruCommPool[sender_addr].bFlagHasRxNewPackage) 	//�ϴν��յ������Ѵ���
				{
					StruCommPool[sender_addr].bFlagHasRxNewPackage = TRUE;
					//StruCommPool[sender_addr].PackageData.m_nRxSeqNum = cmd_num_this_time;
					//�Ƚ�Ӧ������ݰ��ͷ��͵����ݰ��Ƿ����һ��
					
//					if(cmd_is_valid_flag == PACKAGE_IS_NEW_AND_OK)
//					{
//						//memcpy(&StruCommPool[sender_addr].rx_content_buf, &comm_recv_buf[POS_CMD_TYPE], cmd_rx_length);	//�������ݵ��ʵ����ڴ�ռ�洢
//						StruCommPool[sender_addr].PackageData.m_nRxSeqNum = cmd_num_this_time;
//						StruCommPool[sender_addr].PackageData.m_nRecvDataLen = cmd_rx_length;
//						memcpy(StruCommPool[sender_addr].PackageData.rx_content_buf, &comm_recv_buf[POS_CMD_TYPE], cmd_rx_length);	//�������ݵ��ʵ����ڴ�ռ�洢					
//						if(sender_addr == 0)	//COMPUTER�����������ݣ���Ҫ������MAC��ַ��IP��ַ
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
					//Ϊʲô�ϴε����ݻ�û�д���������Ӧ�ò�������������!			
				}
			}
			break;
		case STEP4:
			//if(need_send_msg)				//�����µ���ҪӦ������ݰ�������Ҫ�����������
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
		case STEP6:	//����Ӧ��
			net_cmd_send_flag = 1;
			//net_cmd_send_length = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + cmd_tx_length + PACKAGE_TAIL_LENGTH;
			ethernet_send_msg();			
			net_cmd_busy_flag = FALSE;
			//debug_info("Ack!");
			operation = STEP1;
			break;
		case STEP7:	//����Ӧ��
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
		case STEP8:	//USBӦ��
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

//�Խ��յ������ݽ��д���
//    1)1~99������Ϊ������������
//    2)100������Ϊָ��ĳ����ŵĵ�������ָ���ʽΪ��100������+ָ�����������+�ö��������������
//    3)101~199������Ϊ�û����ɶ��������UserCode��Command��command.c��ProcessSUserCmd���н���
//    4)200~255������Ϊϵͳ���������BSP��Command��bsp_command.c��ProcessSystemCmd���н���
void ProcessCmd(void)
{
	unsigned char i;
	unsigned char crc=0;

	if(bRecvData)
	{
		bRecvData = false;
//		comm_recv_buf[POS_CMD_TYPE] = 210;
		if(comm_recv_buf[POS_CMD_TYPE] <= CMD_OPERATION_END)    //1~99,��������
		{
            memcpy(cParamBuf,comm_recv_buf,sizeof(cParamBuf));  //���ݲ��������еĲ������Ǵ�cParamBuf[POS_DATA_0]��ʼ��ȡ��
			gMachineOperation[comm_recv_buf[POS_CMD_TYPE]]->nParameter = comm_recv_buf[POS_DATA_0]<<8 + comm_recv_buf[POS_DATA_0+1];
            if(StartOp(gMachineOperation[comm_recv_buf[POS_CMD_TYPE]]))   //�����ɹ�ִ��
			{
				comm_send_buf[10]=0xE0;
			}
			else   //����ִ��ʧ��
			{
				comm_send_buf[10]=0xE1;
			}
			txlen=1;
		}
		else if(CMD_DO_OPERATION == comm_recv_buf[POS_CMD_TYPE])  //100��������ָ���ʽΪ������+���������+�ö��������������
		{
            memcpy(cParamBuf,&comm_recv_buf[1],sizeof(cParamBuf));  //Ϊ�˺����汣�ּ��ݣ��˴�����ƫ��1�����еĲ������Ǵ�cParamBuf[POS_DATA_0]��ʼ��ȡ��

			if(StartOp(gMachineOperation[comm_recv_buf[POS_DATA_0]]))   //�����ɹ�ִ��
			{
				comm_send_buf[10]=0xE0;
			}
			else   //����ִ��ʧ��
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
			if(comm_recv_buf[POS_CMD_TYPE] >= CMD_SYSTEM_BASE)   //200~255ϵͳ��������
			{
				ProcessSystemCmd();
			}
			else   //�û�Ҫ�޸ĵĲ���
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
			crc=crc^comm_send_buf[3+i];  //�������
		}
		comm_send_buf[10+txlen]=crc;
		comm_send_buf[11+txlen]=MSG_ETX;   //����
	}
}

struct StruTarget
{
	char m_nId;
	unsigned char mac_addr[6];				//MAC��ַ
	unsigned char ip_addr[4];				//IP��ַ
	unsigned char net_port;					//�˿ڵ�ַ
};

//#define MAX_LENGTH	128
//typedef struct  
//{
//	unsigned char m_nSeqNum;			//ͨѶ���к�
//	unsigned char m_nSeqNumLast;		//ͨѶ���к���ʷ
//	//unsigned int m_nVersionLength;	//�̼�����
//	unsigned int m_nRecvDataLen;				//�������ݳ���
//	unsigned char m_cSendBuf[MAX_LENGTH];		//���ͻ�����
//	unsigned char m_cSendBufSave[MAX_LENGTH];	//���ͻ���������ʧ��ʱ�ı���
//	unsigned char m_cSendBufLast[MAX_LENGTH];	//���ͻ�������ʷ
//	unsigned char m_cRecvBuf[MAX_LENGTH];		//���ջ�����
//	unsigned char m_cRecvBufLast[MAX_LENGTH];	//���ջ�������ʷ
//	unsigned char m_nCommRetryCnt;			//ͨ������
//	unsigned int m_nTotalFailure;	//��ʧ�ܴ���
//	unsigned int m_nReadTimeOut;	//��ȡ����ʱ��ʱʱ��
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
//		case STEP1:		//����Ϣ�����ͣ�
//			operation = STEP2;
//			break;
//		case STEP2:
//			pStruCommPackage = &StruCommPACKAGE[nID];  	//�������ݰ�
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
////			else if(check_time_delay(baseTime, COMM_TIMEOUT))	//��ʱδ�յ�Ӧ��
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
//		case STEP5:		//��λͨѶ��ɱ�־
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
////	//������Ϣ
////	m_nReadTimeOut = 100; 
////	nRet = SendData((const char *)m_cSendBuf, nLen, m_nReadTimeOut);
////
////	
////	//��ʼ��������
////	memset(pStruCommPackage->m_cRecvBuf, 0, sizeof(pStruCommPackage->m_cRecvBuf));
////	pStruCommPackage->m_nRecvDataLen = 0;//��ȡ���ֽ���
////
////	m_nReadTimeOut = 100;
////	if((unsigned char)(pStruCommPackage->m_cSendBuf[POS_CMD_TYPE]) == CMD_SET_PARAMETER) //���ز������ݣ�CPU�ȱ��������ٷ��أ�ʱ���Գ������⴦��
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
		case STEP1:		//����Ϣ�����ͣ�
			pStru_Board->nStatusCommunication = STATUS_COMM_FREE;
			if(pStru_Board->bFlagNeedTxNewPackage)
			{
				operation = STEP2;
				pStru_Board->nStatusCommunication = STATUS_COMM_NEED_TO_SEND;
			}
			break;
		case STEP2:		//������Ϣ
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
		case STEP3:		//������Ϣ
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
		case STEP1:		//����Ϣ�����ͣ�
			pStru_Board->nStatusCommunication = STATUS_COMM_FREE;
			if(pStru_Board->bFlagNeedTxNewPackage)
			{
				operation = STEP2;
				pStru_Board->nStatusCommunication = STATUS_COMM_NEED_TO_SEND;
			}
			break;
		case STEP2:		//������Ϣ
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
		case STEP3:		//������Ϣ
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
		case STEP1:		//����Ϣ�����ͣ�
			pStru_Board->nStatusCommunication = STATUS_COMM_FREE;
			if(pStru_Board->bFlagNeedTxNewPackage)
			{
				operation = STEP2;
				pStru_Board->nStatusCommunication = STATUS_COMM_NEED_TO_SEND;
			}
			break;
		case STEP2:		//������Ϣ
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
		case STEP3:		//������Ϣ
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
//			if(pStru_Board->bFlagRequestByOther)	//���Է�����
//			{
//				//���Է�������µ����ݰ�
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
//		case STEP1:		//����Ϣ�����ͣ�
//			operation = STEP2;
//			break;
//		case STEP2:
//			//pStruCommPackage = &StruCommPACKAGE[nID];  	//�������ݰ�
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
//			else if(check_time_delay(baseTime, COMM_TIMEOUT))	//��ʱδ�յ�Ӧ��
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
//		case STEP5:		//��λͨѶ��ɱ�־
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
