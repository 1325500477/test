//Tab size: 4;	Indent size: 4
#ifndef __BSP_COMMAND_H__
#define __BSP_COMMAND_H__

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
//如下是系统级别命令，若需升级请大家同步维护！
#define CMD_PROGRAM         		200			// 进入Bootloader更新程序
#define CMD_SET_DC_MOTOR			201			// 设置直流马达输出
#define CMD_SET_STEP_MOTOR			202			// 设置步进马达测试
#define CMD_GET_FIRMWARE		    203			// 获取APP固件信息（包含BSP固件版本信息）
#define CMD_GET_ALL_INFO			204			// --（可不用）获取所有IO状态（输入、输出以及步进电机）
#define CMD_GET_SENSOR				205			// 获取传感器信号
#define CMD_GET_PARAMETER			206			// 获取设备参数配置
#define CMD_SET_PARAMETER			207			// 设置设备参数配置
#define CMD_GET_DEFAULT_VALUE		208			// 恢复出厂值
#define CMD_SET_DEFAULT_VALUE		209			// 设置成出厂值

#define CMD_GET_OP_INFO 		    210			// 获取某个指定的流程动作结果，时间，状态等相关信息
#define CMD_GET_ALL_OP_RESULT       211         // --（可不用）获取流程结果（批量上传所有动作结果，可能用于需要提速的场合）
#define CMD_GET_ALL_OP_TIME         212         // --（可不用）获取流程时间（批量上传所有动作时间）
#define CMD_RESUME_OP				213			// 恢复流程(设备错误后人工恢复继续运行)
#define CMD_START_OP_BY_MODE        214			// 启动流程从START步开始运行，带有参数（0为直接运行，1为单节拍方式运行)
#define CMD_DO_OP_NEXT_STEP			215			// 执行流程下一步
#define CMD_SET_OP_MODE             216         // 设置流程模式

#define CMD_SUB_NET					254			// 内部控制板通讯指令（第一个字节为M表示主机，为S表示从机）
#define CMD_HAND_SHAKE				255			// 握手指令

//STM32专用
#define CMD_UPDATE_FLASH            197
#define CMD_JUMP_TO_APP             198
#define CMD_JUMP_TO_BOOTLOADER      199

void JumpToApp(void);
void JumpToBootload(void);

extern void ProcessSystemCmd(void);

#ifdef __cplusplus
}
#endif

#endif
