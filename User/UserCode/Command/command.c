//Tab size: 4;	Indent size: 4
#include "../../BSP/bsp.h"
#include "../Process/process.h"
#include "../Parameter/parameter.h"
#include "command.h"

extern unsigned int txlen;
unsigned char CardStatus[10];
unsigned int cmd_get_mcu_data(void)
{
    unsigned int nLen = 0;
	UNION_MCU_DATA nMCUData;
	unsigned char i;

	nMCUData.strMCUData.m_Input_All = m_Input_All_now;
	memcpy(nMCUData.strMCUData.m_DM_Status,m_DM_Status,16);
	for (i=0;i<8;i++)
	{
		nMCUData.strMCUData.m_SM_Status[i] = StepMotorCtr[i+1].bMotorRunStatus;
	}
	for (i=0;i<8;i++)
	{
		nMCUData.strMCUData.m_ABS_SM_STEP[i] = sm_get_abs(i+1);
	}
	memcpy(&comm_send_buf[POS_DATA_0],nMCUData.uData,sizeof(nMCUData.strMCUData));		//�ѽṹcopy�����ͻ�����	
	nLen = sizeof(nMCUData.strMCUData);
    return nLen;
}

/*****************************************************************************
  * �������ƣ� ProcessUserCmd
  * ���������� ��ȡ�������������
  * ����˵������1���û���Ҫ�޸ĵ��������219,С��255
*****************************************************************************/
void ProcessUserCmd(void)
{
    unsigned char i = 0;
    switch (comm_recv_buf[POS_CMD_TYPE])
    {
    case CMD_JUMP_TO_APP:
        JumpToApp();
        comm_send_buf[POS_DATA_0] = 0xE0;
        txlen = 1;
        break;
    case CMD_JUMP_TO_BOOTLOADER:
        JumpToBootload();
        comm_send_buf[POS_DATA_0] = 0xE0;
        txlen = 1;
        break;
	// �ڴ�����������Զ�������(������ִ�е�����,�������õ�֮�������)
	case CMD_RESET_ALL_SIGNAL:
		ResetAllSignal();
		break;
	case CMD_LIGHT_START_ON:
//		StartLightON();
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
		break;
	case CMD_LIGHT_START_OFF:
//		StartLightOFF();
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
		break;
	case CMD_LIGHT_PAUSE_ON:
//		PauseLightON();
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
		break;
	case CMD_LIGHT_PAUSE_OFF:
//		PauseLightOFF();
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
		break;

	// ������ɫ�ƿ���������Ϊ����
	//case CMD_LIGHT_GREEN_ON:
	//	TriClrStartON();
	//	comm_send_buf[10] = 0xE0;
	//	txlen = 1;
	//	break;
	//case CMD_LIGHT_GREEN_OFF:
	//	TriClrStartOFF();
	//	comm_send_buf[10] = 0xE0;
	//	txlen = 1;
	//	break;
	//case CMD_LIGHT_YELLOW_ON:
	//	TriClrPauseON();
	//	comm_send_buf[10] = 0xE0;
	//	txlen = 1;
	//	break;
	//case CMD_LIGHT_YELLOW_OFF:
	//	TriClrPauseOFF();
	//	comm_send_buf[10] = 0xE0;
	//	txlen = 1;
	//	break;
	//case CMD_LIGHT_RED_ON:
	//	TriClrErrorON();
	//	comm_send_buf[10] = 0xE0;
	//	txlen = 1;
	//	break;
	//case CMD_LIGHT_RED_OFF:
	//	TriClrErrorOFF();
	//	comm_send_buf[10] = 0xE0;
	//	txlen = 1;
	//	break;

	case CMD_GET_CARD_STATUS:
		// ��ȡ��λ״̬���п�/�޿���
		for(i=0; i<10; i++)
		{
			comm_send_buf[POS_DATA_0+i] = CardStatus[i];
		}
		txlen = 10;
		break;
	case CMD_GET_STATUS:   		
		// ��ѯ����״̬
		if ( comm_recv_buf[POS_DATA_0] == 1)
		{
//			comm_send_buf[POS_DATA_0] = gStartBottonPressed;
//			gStartBottonPressed = false;
		}
		if ( comm_recv_buf[POS_DATA_0] == 2)
		{
//			comm_send_buf[POS_DATA_0] = gPauseBottonPressed;
//			gPauseBottonPressed = false;
		}
		txlen = 1;
		break;
    case CMD_GET_MCU_DATA:
        txlen = cmd_get_mcu_data();
        break;
	default:
		break;
    }
}

