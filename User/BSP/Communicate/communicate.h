//Tab size: 4;	Indent size: 4
#ifndef __MY_COMMAND_H__
#define __MY_COMMAND_H__

#define ADDR_BOARDCAST		0xFF

//ͨѶ��ʽ���£�
//ͬ��ͷ1	ͬ��ͷ2	��ʼ��	�汾��		Ŀ�ĵ�ַ	Դ��ַ	���ݰ����	ָ���H	ָ���L	ָ������	����1	����2	��	����n	У����	������
//  @       @      (   		1		ADDR_RECV 	ADDR_SEND  SEQ_NUM   	CMD_LEN_H 	CMD_LEN_L 	CMD_TYPE  	DATA0 	DATA1 	... DATAn   LRC     )
//���ڵ�Ƭ���ڴ��������ޣ��ֹ涨CMD_LEN_H��CMD_LEN_L��ϳ�˫�ֽڱ�ʾ�ĳ���<=512
//�涨LCRУ��ӡ�Ŀ�ĵ�ַ����ʼ����������n��������LRC�㷨�ο��ƶ����ĵ�


#define PACKAGE_HEAD_LENGTH		3       //HEAD�ӡ�ͬ��ͷ1��������ʼ����
#define PACKAGE_INFO_LENGTH		6       //HEAD�ӡ��汾�š�����ָ���L��
//#define PACKAGE_DATA_LENGTH
#define PACKAGE_TAIL_LENGTH		2       //TAIL�ӡ�У���롱������������

//��CMD_TYPE��CMD_DATAn��������@,@,��,VER,RX_ADDR,TX_ADDR,NUM,LEN_H,LEN_L�Լ�CRC,)
#define MAX_CMD_CONTENT_LENGTH	1461	//UDP�������Ϊ1500����ȥ���ְ�ͷ��β�õ��Ľ��Ϊ1461��
//��CMD_DATA0��CMD_DATAn��������@,@,��,VER,RX_ADDR,TX_ADDR,NUM,LEN_H,LEN_L,CMD_TYPE�Լ�CRC,)
#define MAX_CMD_DATA_LENGTH		(MAX_CMD_CONTENT_LENGTH - 1)
//��@��ʼ��)��������������ͨѶ��
#define MAX_PACKAGE_LENGTH	(PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + MAX_CMD_CONTENT_LENGTH + PACKAGE_TAIL_LENGTH)
//����TCPͷ�������Ϣ
#define NET_PACKAGE_LENGTH	(MAX_PACKAGE_LENGTH + 58)

#define MSG_SYN1	0x40	//'@'
#define MSG_SYN2	0x40	//'@'
#define MSG_STX		0x28	//'('
#define MSG_ETX		0x29	//')'
#define MSG_VER		0x41	//'A'

#define POS_SYN1				0
#define POS_SYN2				1
#define POS_STX					2
#define POS_VER					3
#define POS_ADDR_RECV			4
#define POS_ADDR_SEND		    5
#define POS_CMD_SEQ			    6
#define POS_CMD_LEN_H		    7
#define POS_CMD_LEN_L			8
#define POS_CMD_TYPE			9
#define POS_DATA_0				10
#define POS_DATA_1				11

//#define POS_LRC (POS_DATA_START + cmd_length)

#define PACKAGE_ATTR_REQUEST		0		//�����ط��������������
#define PACKAGE_ATTR_ACK			1		//�����ط���������Ӧ���

#define PACKAGE_IS_NEW_AND_OK	0	 		//�µ���Ч���ݰ�
#define PACKAGE_IS_RETRY_CMD	1			//�ط������ݰ�
#define PACKAGE_IS_NOT_VALID	2			//���ݰ����Ϸ�
#define PACKAGE_ERR_SYN1		3			//���ݰ����Ϸ���SYN1��ͬ��ͷ1������
#define PACKAGE_ERR_SYN2		4			//���ݰ����Ϸ���SYN2��ͬ��ͷ2������
#define PACKAGE_ERR_STX			5			//���ݰ����Ϸ���STX����ʼλ������
#define PACKAGE_ERR_ETX			6			//���ݰ����Ϸ���ETX������λ������
#define PACKAGE_ERR_VER			7			//���ݰ����Ϸ���VER���汾�ţ�����
#define PACKAGE_ERR_RECV_ADDR	8			//���ݰ����Ϸ������յ�ַ�쳣
#define PACKAGE_ERR_LENGTH		9			//���ݰ����Ϸ������ݰ������쳣
#define PACKAGE_ERR_LRC			10			//���ݰ����Ϸ���У���쳣
#define PACKAGE_ERR_CMD_SEQ		11			//���ݰ����Ϸ���Ӧ�����ݰ���ˮ�Ų�ƥ��
#define PACKAGE_ERR_CMD_TYPE	12		   	//���ݰ����Ϸ���Ӧ�����ݰ�ָ�ƥ��

/*
20160522:
    ����������۾�����IAR��226R4+227R4��BSPϵͳ���е���������ɸѡ����Ԥ�������£�
    1)1~99������Ϊ������������
    2)100������Ϊָ��ĳ����ŵĵ�������ָ���ʽΪ��100������+ָ�����������+�ö��������������
    3)101~199������Ϊ�û����ɶ��������UserCode��Command��command.c��ProcessSUserCmd���н���
    4)200~255������Ϊϵͳ���������BSP��Command��bsp_command.c��ProcessSystemCmd���н���
*/

#define CMD_OPERATION_BASE  1
#define CMD_OPERATION_END   99

#define CMD_DO_OPERATION    100     //��������ָ���ʽΪ������+���������+�ö��������������

#define CMD_USER_BASE       101
#define CMD_USER_END        199
#define CMD_SYSTEM_BASE     200
#define CMD_SYSTEM_END      255


extern void ProcessCmd(void);
extern void ProcessSystemCmd(void);
extern void ProcessUserCmd(void);

extern unsigned char dst_mac[][6];
extern unsigned char ip_addr[][4];

extern unsigned char cmd_num_this_time;
extern unsigned char cmd_num_last_time;
extern unsigned char comm_recv_buf[];
extern unsigned char comm_send_buf[];
extern unsigned char comm_send_buf_last[];
extern unsigned int  cmd_tx_length;

void comm_cmd_execution(void);
void process_all_cmd(void);






#define PKG_LENGTH			128	//NET_PACKAGE_LENGTH	   	//ÿ�����ݳ���
#define MAX_RETRY_COUNTER	5		//������Դ���5��
#define COMM_TIMEOUT		2000   	//��ʱʱ��2000ms
enum status_comm
{
	STATUS_COMM_FREE = 0,
	STATUS_COMM_NEED_TO_SEND,		//��Ҫ��������
	STATUS_COMM_SEND_FINISH,		//���ͽ���
	STATUS_COMM_WAIT_RECV,			//�ȴ���������
	STATUS_COMM_RECV_FINISH,		//�������
	STATUS_COMM_UPDATE,			   	//�����յ������ݡ�ˢ�µ�ȫ�ֱ���
	STATUS_COMM_END,				//ͨѶһ�����̽���
	STATUS_COMM_ERROR
};

typedef struct
{
	unsigned char m_nRxSeqNum;					//���յ������ݰ����к�
	unsigned char m_nTxSeqNum;					//���͵����ݰ����к�
	unsigned char m_nTxSeqNumLast;				//���͵����ݰ����к�(�ϴ�)
	unsigned int m_nLenTx;						//�����������ݰ�����	
	unsigned int m_nLenRx;						//�����������ݰ�����
	unsigned char m_cSendBuf[PKG_LENGTH];		//���͵����ݰ�
	unsigned char m_cRecvBuf[PKG_LENGTH];		//���յ����ݰ�

	//unsigned int tx_content_length;			//������Ч���ݳ���
	//unsigned int m_nRecvDataLen;				//������Ч���ݳ���	
	//unsigned char tx_content_buf[PKG_LENGTH];	//���͵����ݰ���ʵ��Ч����
	//unsigned char rx_content_buf[PKG_LENGTH];	//���յ����ݰ���ʵ��Ч����
}PACKAGE;

typedef struct
{
	unsigned char board_addr;				//���ư��ַ
	unsigned char mac_addr[6];				//MAC��ַ
	unsigned char ip_addr[4];				//IP��ַ

	PACKAGE PackageData;					//���ݰ�������
	PACKAGE PackageBackup; 					//���ݰ�����������

	unsigned char nStatusCommunication;		//ͨѶ״̬��
	unsigned char bFlagNeedTxNewPackage;	//��Ҫ����һ���µ�����
	unsigned char bFlagHasRxNewPackage;		//���յ�һ���µ�����

	unsigned char bFlagError;				//�����־
	unsigned int nCounterSuccess;		 	//ͨѶ�ɹ�����
	unsigned int nCounterFailed;			//ͨѶʧ�ܴ���	
}STRU_COMMUNICATE_WITH_BOARD, *pSTRU_COMMUNICATE_WITH_BOARD;

#define COMPUTER	0
#define BOARD1		1
#define BOARD2		2
#define BOARD3		3
#define BOARD4		4
#define BOARD5		5
#define BOARD6		6
#define BOARD7		7
#define BOARD8		8
#define BOARD9		9
#define TOTAL_BOARD	10
extern STRU_COMMUNICATE_WITH_BOARD StruCommPool[TOTAL_BOARD];
#endif
