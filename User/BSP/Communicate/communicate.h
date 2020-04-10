//Tab size: 4;	Indent size: 4
#ifndef __MY_COMMAND_H__
#define __MY_COMMAND_H__

#define ADDR_BOARDCAST		0xFF

//通讯格式如下：
//同步头1	同步头2	起始符	版本号		目的地址	源地址	数据包编号	指令长度H	指令长度L	指令类型	数据1	数据2	…	数据n	校验码	结束符
//  @       @      (   		1		ADDR_RECV 	ADDR_SEND  SEQ_NUM   	CMD_LEN_H 	CMD_LEN_L 	CMD_TYPE  	DATA0 	DATA1 	... DATAn   LRC     )
//由于单片机内存容量有限，现规定CMD_LEN_H和CMD_LEN_L组合成双字节表示的长度<=512
//规定LCR校验从“目的地址”开始，到“数据n”结束，LRC算法参考制定的文档


#define PACKAGE_HEAD_LENGTH		3       //HEAD从“同步头1”至“起始符”
#define PACKAGE_INFO_LENGTH		6       //HEAD从“版本号”至“指令长度L”
//#define PACKAGE_DATA_LENGTH
#define PACKAGE_TAIL_LENGTH		2       //TAIL从“校验码”至“结束符”

//从CMD_TYPE到CMD_DATAn，不包含@,@,（,VER,RX_ADDR,TX_ADDR,NUM,LEN_H,LEN_L以及CRC,)
#define MAX_CMD_CONTENT_LENGTH	1461	//UDP最长报长度为1500，出去各种包头包尾得到的结果为1461。
//从CMD_DATA0到CMD_DATAn，不包含@,@,（,VER,RX_ADDR,TX_ADDR,NUM,LEN_H,LEN_L,CMD_TYPE以及CRC,)
#define MAX_CMD_DATA_LENGTH		(MAX_CMD_CONTENT_LENGTH - 1)
//从@开始到)结束，整个数据通讯包
#define MAX_PACKAGE_LENGTH	(PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + MAX_CMD_CONTENT_LENGTH + PACKAGE_TAIL_LENGTH)
//包含TCP头等相关信息
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

#define PACKAGE_ATTR_REQUEST		0		//其它地方发过来的请求包
#define PACKAGE_ATTR_ACK			1		//其它地方发过来的应答包

#define PACKAGE_IS_NEW_AND_OK	0	 		//新的有效数据包
#define PACKAGE_IS_RETRY_CMD	1			//重发的数据包
#define PACKAGE_IS_NOT_VALID	2			//数据包不合法
#define PACKAGE_ERR_SYN1		3			//数据包不合法，SYN1（同步头1）不对
#define PACKAGE_ERR_SYN2		4			//数据包不合法，SYN2（同步头2）不对
#define PACKAGE_ERR_STX			5			//数据包不合法，STX（起始位）不对
#define PACKAGE_ERR_ETX			6			//数据包不合法，ETX（结束位）不对
#define PACKAGE_ERR_VER			7			//数据包不合法，VER（版本号）不对
#define PACKAGE_ERR_RECV_ADDR	8			//数据包不合法，接收地址异常
#define PACKAGE_ERR_LENGTH		9			//数据包不合法，数据包长度异常
#define PACKAGE_ERR_LRC			10			//数据包不合法，校验异常
#define PACKAGE_ERR_CMD_SEQ		11			//数据包不合法，应答数据包流水号不匹配
#define PACKAGE_ERR_CMD_TYPE	12		   	//数据包不合法，应答数据包指令不匹配

/*
20160522:
    经过大家讨论决定：IAR（226R4+227R4）BSP系统固有的命令重新筛选排序，预定义如下：
    1)1~99号命令为单动动作命令
    2)100号命令为指定某命令号的单动动作指令，形式为（100号命令+指定动作命令号+该动作命令附带参数）
    3)101~199号命令为用户自由定义命令，在UserCode的Command下command.c的ProcessSUserCmd进行解析
    4)200~255号命令为系统级别命令，在BSP的Command下bsp_command.c的ProcessSystemCmd进行解析
*/

#define CMD_OPERATION_BASE  1
#define CMD_OPERATION_END   99

#define CMD_DO_OPERATION    100     //单动动作指令，形式为（命令+动作命令号+该动作命令附带参数）

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






#define PKG_LENGTH			128	//NET_PACKAGE_LENGTH	   	//每包数据长度
#define MAX_RETRY_COUNTER	5		//最多重试次数5次
#define COMM_TIMEOUT		2000   	//超时时间2000ms
enum status_comm
{
	STATUS_COMM_FREE = 0,
	STATUS_COMM_NEED_TO_SEND,		//需要发送数据
	STATUS_COMM_SEND_FINISH,		//发送结束
	STATUS_COMM_WAIT_RECV,			//等待接收数据
	STATUS_COMM_RECV_FINISH,		//接收完成
	STATUS_COMM_UPDATE,			   	//解析收到的数据、刷新到全局变量
	STATUS_COMM_END,				//通讯一个流程结束
	STATUS_COMM_ERROR
};

typedef struct
{
	unsigned char m_nRxSeqNum;					//接收到的数据包序列号
	unsigned char m_nTxSeqNum;					//发送的数据包序列号
	unsigned char m_nTxSeqNumLast;				//发送的数据包序列号(上次)
	unsigned int m_nLenTx;						//发送整个数据包长度	
	unsigned int m_nLenRx;						//接收整个数据包长度
	unsigned char m_cSendBuf[PKG_LENGTH];		//发送的数据包
	unsigned char m_cRecvBuf[PKG_LENGTH];		//接收的数据包

	//unsigned int tx_content_length;			//发送有效内容长度
	//unsigned int m_nRecvDataLen;				//接收有效内容长度	
	//unsigned char tx_content_buf[PKG_LENGTH];	//发送的数据包真实有效内容
	//unsigned char rx_content_buf[PKG_LENGTH];	//接收的数据包真实有效内容
}PACKAGE;

typedef struct
{
	unsigned char board_addr;				//控制板地址
	unsigned char mac_addr[6];				//MAC地址
	unsigned char ip_addr[4];				//IP地址

	PACKAGE PackageData;					//数据包缓冲区
	PACKAGE PackageBackup; 					//数据包缓冲区备份

	unsigned char nStatusCommunication;		//通讯状态机
	unsigned char bFlagNeedTxNewPackage;	//需要发送一包新的数据
	unsigned char bFlagHasRxNewPackage;		//接收到一包新的数据

	unsigned char bFlagError;				//错误标志
	unsigned int nCounterSuccess;		 	//通讯成功次数
	unsigned int nCounterFailed;			//通讯失败次数	
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
