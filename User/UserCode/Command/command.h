//Tab size: 4;	Indent size: 4
#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif


/*
20160522:
    经过大家讨论决定：IAR（226R4+227R4）BSP系统固有的命令重新筛选排序，预定义如下：
    1)1~99号命令为单动动作命令
    2)100号命令为指定某命令号的单动动作指令，形式为（100号命令+指定动作命令号+该动作命令附带参数）
    3)101~199号命令为用户自由定义命令，在UserCode的Command下command.c的ProcessSUserCmd进行解析
    4)200~255号命令为系统级别命令，在BSP的Command下bsp_command.c的ProcessSystemCmd进行解析
*/
#define CMD_RESET_ALL_SIGNAL		100			// 复位所有IO信号及状态

#define CMD_LIGHT_START_ON			110			// 点亮启动指示灯
#define CMD_LIGHT_START_OFF			111			// 熄灭启动指示灯
#define CMD_LIGHT_START_FLASH		112			// 闪烁启动指示灯
#define CMD_LIGHT_PAUSE_ON			113			// 点亮暂停指示灯
#define CMD_LIGHT_PAUSE_OFF			114			// 熄灭暂停指示灯
#define CMD_LIGHT_PAUSE_FLASH		115			// 闪烁暂停指示灯

#define CMD_LIGHT_RED_ON			120			// 点亮三色灯红灯
#define CMD_LIGHT_RED_OFF			121			// 熄灭三色灯红灯
#define CMD_LIGHT_RED_FLASH			122			// 闪烁三色灯红灯
#define CMD_LIGHT_YELLOW_ON			123			// 点亮三色灯黄灯
#define CMD_LIGHT_YELLOW_OFF		124			// 熄灭三色灯黄灯
#define CMD_LIGHT_YELLOW_FLASH		125			// 闪烁三色灯黄灯
#define CMD_LIGHT_GREEN_ON			126			// 点亮三色灯绿灯
#define CMD_LIGHT_GREEN_OFF			127			// 熄灭三色灯绿灯
#define CMD_LIGHT_GREEN_FLASH		128			// 闪烁三色灯绿灯

#define CMD_GET_CARD_STATUS		    130			// 查询卡片状态，1表示有卡,0表示没有
#define CMD_GET_STATUS				131			// 查询相关状态（按钮是否已按下等状态）
#define CMD_GET_MCU_DATA			132			// 获取底层MCU数据（用结构体来实现）

//STM32专用
#define CMD_UPDATE_FLASH            197
#define CMD_JUMP_TO_APP             198
#define CMD_JUMP_TO_BOOTLOADER      199

typedef struct
{
    unsigned long m_Input_All;			//4
    unsigned char m_DM_Status[12];		//12
    unsigned char m_SM_Status[8];		//8
    signed long m_ABS_SM_STEP[8];		//32
    signed long m_Debug;				//4
}
MCU_DATA,*pMCU_DATA;

typedef union
{
    MCU_DATA strMCUData;
    unsigned char uData[70];
}
UNION_MCU_DATA,*pUNION_MCU_DATA;

extern void ProcessUserCmd(void);

#ifdef __cplusplus
}
#endif

#endif

