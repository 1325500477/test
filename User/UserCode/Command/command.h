//Tab size: 4;	Indent size: 4
#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef __cplusplus
extern "C"
{
#endif


/*
20160522:
    ����������۾�����IAR��226R4+227R4��BSPϵͳ���е���������ɸѡ����Ԥ�������£�
    1)1~99������Ϊ������������
    2)100������Ϊָ��ĳ����ŵĵ�������ָ���ʽΪ��100������+ָ�����������+�ö��������������
    3)101~199������Ϊ�û����ɶ��������UserCode��Command��command.c��ProcessSUserCmd���н���
    4)200~255������Ϊϵͳ���������BSP��Command��bsp_command.c��ProcessSystemCmd���н���
*/
#define CMD_RESET_ALL_SIGNAL		100			// ��λ����IO�źż�״̬

#define CMD_LIGHT_START_ON			110			// ��������ָʾ��
#define CMD_LIGHT_START_OFF			111			// Ϩ������ָʾ��
#define CMD_LIGHT_START_FLASH		112			// ��˸����ָʾ��
#define CMD_LIGHT_PAUSE_ON			113			// ������ָͣʾ��
#define CMD_LIGHT_PAUSE_OFF			114			// Ϩ����ָͣʾ��
#define CMD_LIGHT_PAUSE_FLASH		115			// ��˸��ָͣʾ��

#define CMD_LIGHT_RED_ON			120			// ������ɫ�ƺ��
#define CMD_LIGHT_RED_OFF			121			// Ϩ����ɫ�ƺ��
#define CMD_LIGHT_RED_FLASH			122			// ��˸��ɫ�ƺ��
#define CMD_LIGHT_YELLOW_ON			123			// ������ɫ�ƻƵ�
#define CMD_LIGHT_YELLOW_OFF		124			// Ϩ����ɫ�ƻƵ�
#define CMD_LIGHT_YELLOW_FLASH		125			// ��˸��ɫ�ƻƵ�
#define CMD_LIGHT_GREEN_ON			126			// ������ɫ���̵�
#define CMD_LIGHT_GREEN_OFF			127			// Ϩ����ɫ���̵�
#define CMD_LIGHT_GREEN_FLASH		128			// ��˸��ɫ���̵�

#define CMD_GET_CARD_STATUS		    130			// ��ѯ��Ƭ״̬��1��ʾ�п�,0��ʾû��
#define CMD_GET_STATUS				131			// ��ѯ���״̬����ť�Ƿ��Ѱ��µ�״̬��
#define CMD_GET_MCU_DATA			132			// ��ȡ�ײ�MCU���ݣ��ýṹ����ʵ�֣�

//STM32ר��
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

