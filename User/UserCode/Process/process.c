
#include "../Parameter/Parameter.h"
#include "../Command/command.h"
#include "../IOCfg.h"
#include "../../BSP/bsp.h"
#include "../../BSP/Command/bsp_command.h"
#include "process.h"
#include <string.h>
#include <stdio.h>


/***************���������Ϊ��*****************/
/******** ��ض����ṹ��������� ******************************/
stOpCtrl OpResetModule;	            //��λ
stOpCtrl OpResetChannal;   			//��λ�Ϳ�ͨ��
stOpCtrl OpSendCardToWarm; 			//��������ѹ��λ
stOpCtrl OpSendCardToCool; 			//��������ѹ��λ
stOpCtrl OpChannalBackORG; 			//ͨ���ص�ԭ��
stOpCtrl OpWarmDown; 			 	//�����½�
stOpCtrl OpWarmRise; 			 	//��������
stOpCtrl OpCoolDown; 			 	//��ȴ�½�
stOpCtrl OpCoolRise; 			 	//��ȴ����
stOpCtrl OpPresDown; 			 	//��ѹ�½�
stOpCtrl OpPresRise; 			 	//��ѹ����
stOpCtrl OpOpeningPressure;	 		//ѹ����
stOpCtrl OpClosingPressure; 		//ѹ����
stOpCtrl OpMakeCardStart;			//�ƿ�����
stOpCtrl OpWarmEnforceRise;			//��ѹǿ������
stOpCtrl OpWarmEnforceDown;			//��ѹǿ���½�
stOpCtrl OpCoolEnforceRise;			//��ѹǿ������
stOpCtrl OpCoolEnforceDown;			//��ѹǿ���½�
stOpCtrl OpCutCardPosition;			//���ü�����λ��
stOpCtrl OpCutCard;					//�ü���

stOpCtrl OpRightClampClose; 		//�ұ߼��ӱպ�
stOpCtrl OpRightClampLoose; 		//�ұ߼����ɿ�
stOpCtrl OpLeftClampClose; 			//��߼��ӱպ�
stOpCtrl OpLeftClampLoose; 			//��߼����ɿ�
stOpCtrl OpClampClose; 				//���ӱպ�
stOpCtrl OpClampLoose; 				//�����ɿ�
stOpCtrl OpClampReset;              //���Ӹ�λ
stOpCtrl OpShiftTrolley;			//С���ƶ�
stOpCtrl OpReverseFront;			//��ת������
/******** ��������*********************************/

stOpCtrl *gMachineOperation[50]; // ����һ��֧��50�����̵�ָ������
unsigned char gnOperationNo = 1; // ִ�еĶ�����ţ���1��ʼ�Ը��������ṹ����������ʼ����
extern unsigned char cParamBuf[];

//unsigned char gFlagStopSimulation = false; // ģ���ܻ�����ִ��״̬���
unsigned char gStartBottonPressed = false; // ��ʼ������״̬��ǣ�0-δ���°�����1-���°�����
unsigned char gPauseBottonPressed = false; // ��ͣ������״̬��ǣ�0-δ���°�����1-���°�����

struct sTrolleyPara
{
	int iaAbsPosLiftCar[5];  //С���ƶ�������λ�ľ��Բ�����ʼֵ	
	unsigned int uiSpeed;     
}g_sTrolleyPara =
{
	{0,0,0,0,0},
	100,
};


unsigned char nLocation;	//�洢�п�Ƭ�ļ���λ��
/****************************************************
Function Name: InitializeMachine
*****************************************************
Descriptions:  
 ���豸���г�ʼ������
 �������豸��������֮ǰҪ�Ƚ��г�ʼ������

*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void InitializeMachine(void)
{
	unsigned char i;

	gMachineOperation[gnOperationNo++] = &OpResetModule;					 //1 ��λ
	gMachineOperation[gnOperationNo++] = &OpResetChannal; 			 		 //2 ��λ�Ϳ�ͨ��
	gMachineOperation[gnOperationNo++] = &OpSendCardToWarm;			 		 //3 ��������ѹ��λ
	gMachineOperation[gnOperationNo++] = &OpSendCardToCool;					 //4 ��������ѹ��λ
	gMachineOperation[gnOperationNo++] = &OpChannalBackORG;			 		 //5 ͨ���ص�ԭ��
	gMachineOperation[gnOperationNo++] = &OpWarmDown;						 //6 �����½�
	gMachineOperation[gnOperationNo++] = &OpWarmRise;						 //7 ��������
	gMachineOperation[gnOperationNo++] = &OpCoolDown;						 //8 ��ȴ�½�
	gMachineOperation[gnOperationNo++] = &OpCoolRise;						 //9 ��ȴ����
	gMachineOperation[gnOperationNo++] = &OpPresDown;						 //10 ��ѹ�½�
	gMachineOperation[gnOperationNo++] = &OpPresRise;						 //11 ��ѹ����
	gMachineOperation[gnOperationNo++] = &OpOpeningPressure;			 	 //12 ��Դ��
	gMachineOperation[gnOperationNo++] = &OpClosingPressure;			 	 //13 ��Դ��
	gMachineOperation[gnOperationNo++] = &OpMakeCardStart;					 //14 �ƿ�����
	gMachineOperation[gnOperationNo++] = &OpWarmEnforceRise;				 //15 ��ѹ����������
	gMachineOperation[gnOperationNo++] = &OpWarmEnforceDown;				 //16 ��ѹ�������½�
	gMachineOperation[gnOperationNo++] = &OpCoolEnforceRise;				 //17 ��ѹ����������
	gMachineOperation[gnOperationNo++] = &OpCoolEnforceDown;				 //18 ��ѹ�������½�
	gMachineOperation[gnOperationNo++] = &OpCutCardPosition;		 		 //19 ���ü�����λ��
	gMachineOperation[gnOperationNo++] = &OpCutCard;						 //20 �ü���
	gMachineOperation[gnOperationNo++] = &OpRightClampClose; 		         //21 �ұ߼��ӱպ�
	gMachineOperation[gnOperationNo++] = &OpRightClampLoose; 				 //22 �ұ߼����ɿ�
	gMachineOperation[gnOperationNo++] = &OpLeftClampClose; 				 //23 ��߼��ӱպ�
	gMachineOperation[gnOperationNo++] = &OpLeftClampLoose; 				 //24 ��߼����ɿ�
	gMachineOperation[gnOperationNo++] = &OpClampClose; 					 //25 ���ӱպ�
	gMachineOperation[gnOperationNo++] = &OpClampLoose; 					 //26 �����ɿ�
	gMachineOperation[gnOperationNo++] = &OpClampReset; 					 //27 ���Ӹ�λ
	gMachineOperation[gnOperationNo++] = &OpShiftTrolley;					 //28 С���ƶ� 
	gMachineOperation[gnOperationNo++] = &OpReverseFront;					 //29 ��ת������


	for (i = 1; i < gnOperationNo; i++)
	{
		InitOpStruct(gMachineOperation[i], i, 0xFFFE);
	}
	/**************���������λ�ٶȶ���*******************/
	StepMotorCtr[SM_CHANNEL].nResetSpeed = 5; // ͨ�������λ�ٶ�
}
/****************************************************
Function Name: ResetAllSiganl
*****************************************************
Descriptions:  
 ��λ���б�־
 ��λ���нṹ��,��Ȼ�Ḵλ��λ��������ṹ��,
 ���Ե�������������,������������

*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void ResetAllSignal(void)
{
	unsigned char i;

	for (i = 2; i < gnOperationNo; i++)
	{
		// ��ʼ�����ж����ṹ�壻���Ը�λ��ǰģ�鶯����������
		InitOpStruct(gMachineOperation[i], i, 0xFFFF);
	}
	for (i = 1; i <= 12; i++)
	{
		// �ر�����ֱ�����
		dm_ctrl_one(i, 0);
	}
	for (i = 1; i <= 8; i++)
	{
		// �ر����в������
		sm_stop(i);
		sm_set_enable(i, SM_ENABLE);
		StepMotorCtr[i].bRunNoStop = FALSE;
	}
}

/****************************************************************************************
  * �������ƣ� Op_ResetModule
  * ���������� ��λ�����豸
  * ����˵����
****************************************************************************************/
/* void Op_ResetModule(void)
{
	stOpCtrl *CurrentOp = &OpResetModule;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->bEn = false;
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		CurrentOp->nStep = STEP2;
		break;

	case STEP2:
		CurrentOp->nResult = 0xFFFF;
		break;

	default:
		break;
	}
	DealResult(CurrentOp);
} */

/****************************************************
Function Name: Op_ResetChannal
*****************************************************
Descriptions:  ��λͨ�����
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_ResetChannal(void)
{
	stOpCtrl *CurrentOp = &OpResetChannal;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if ((nLocation == ENUM_INSIDE_WARE) || (nLocation == ENUM_INSIDE_COOL)) //����ѹ����ѹ��λ
			{
				StartChildOp(CurrentOp, &OpChannalBackORG); //����ԭ��
				CurrentOp->nStep = STEP7;
			}
			else
			{
				if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //������ѹ��λ
				{
					sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED], 3000);
					CurrentOp->nStep = STEP2;
				}
				else
				{
					CurrentOp->nStep = STEP5;
				}
			}
		break;
		case STEP2:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //������ѹ��λ
			{
				sm_stop(SM_CHANNEL);  //Ҫ��ֹͣȻ�������з������в�����
				sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED], parameter[WARM_STEP]); //��ǰ����һ��(����)
				CurrentOp->nStep = STEP3;
			}
			else
			{
				if (IsSmRunFinish(SM_CHANNEL)) //����������
				{
					sm_stop(SM_CHANNEL);					  //ֹͣ���
					SetOpErrorInfo(CurrentOp, 20001, ERROR_END); //ͨ��������ѹ������ʧ��
				}
			}
		break;
		case STEP3:
			if (IsSmRunFinish(SM_CHANNEL)) //����������
			{
				SetOpTimeDelay(CurrentOp, 100);
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], parameter[BACK_STEP]); //����������һ�ξ���
			CurrentOp->nStep = STEP6;
			break;
		case STEP6:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				nLocation = ENUM_OUTSIDE; //�ӿ�����������
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP7:
			if (IsChildOpOK(CurrentOp, &OpChannalBackORG))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_SendCardToWarm
*****************************************************
Descriptions:  ��������ѹ��λ
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: void
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_SendCardToWarm(void)
{
	stOpCtrl *CurrentOp = &OpSendCardToWarm;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;

		case STEP1:
			if (nLocation == ENUM_OUTSIDE)
			{
				sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED],5000);
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, 20002, ERROR_RETRY | ERROR_END); //ͨ��������ѹʧ��
			}
		break;
		case STEP2:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //������ѹ��λ
			{
				sm_stop(SM_CHANNEL);  //Ҫ��ֹͣȻ�������з������в�����
				sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED], parameter[WARM_STEP]); //��ǰ����һ��(����)
				CurrentOp->nStep = STEP3;
			}
			else
			{
				if (IsSmRunFinish(SM_CHANNEL)) //����������
				{
					sm_stop(SM_CHANNEL);											//ֹͣ���
					SetOpErrorInfo(CurrentOp, 20003, ERROR_END); //ͨ��������ѹ������ʧ��
				}
			}
		break;
		case STEP3:
			if (IsSmRunFinish(SM_CHANNEL)) //����������
			{
				nLocation = ENUM_INSIDE_WARE; //�ӿ�����������
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_SendCardToCool
*****************************************************
Descriptions:  ��������ѹ��λ
*****************************************************
Calls:
*****************************************************
Input  parameter: void
*****************************************************
Output parameter: void
*****************************************************
Return value: void
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_SendCardToCool(void)
{
	stOpCtrl *CurrentOp = &OpSendCardToCool;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if (nLocation == ENUM_OUTSIDE) //������ԭ��
			{
				StartChildOp(CurrentOp, &OpSendCardToWarm); //�����е���ѹ��λ
				CurrentOp->nStep = STEP2;
			}
			else if (nLocation == ENUM_INSIDE_WARE) //�Ѿ�����ѹ��λ
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, 20004, ERROR_RETRY | ERROR_END); //��������ԭ��,���еĻ�����.
			}
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp, &OpSendCardToWarm)) //����ѹ��λ�Ļ�����������ѹ��λ����
			{
				sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED],(2000 + parameter[COOL_STEP]));
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				nLocation = ENUM_INSIDE_COOL; //�ӿ�����������
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_ChannalBackORG
*****************************************************
Descriptions:  ����ԭ��
*****************************************************
Calls:
*****************************************************
Input  parameter: void
*****************************************************
Output parameter: void
*****************************************************
Return value: void
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ChannalBackORG(void)
{
	stOpCtrl *CurrentOp = &OpChannalBackORG;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:
			if ((nLocation == ENUM_INSIDE_WARE) || (nLocation == ENUM_INSIDE_COOL)) //�������ѹ������ѹ��λ�ſ����������
			{
				sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], 8000); //������
				CurrentOp->nStep = STEP4;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, 20005, ERROR_RETRY | ERROR_END); //�������б���
			}
		break;
		case STEP4:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //������е���ѹ��λ
			{
				sm_stop(SM_CHANNEL);  //Ҫ��ֹͣȻ�������з������в�����
				sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], parameter[BACK_STEP]); //����������һ�ξ���
				CurrentOp->nStep = STEP5;
			}
			else
			{
				if (IsSmRunFinish(SM_CHANNEL)) //�������Ϊ���е���ѹ��λ����
				{
					sm_set_enable(SM_CHANNEL, SM_DISABLE);
					SetOpErrorInfo(CurrentOp, 20006, ERROR_RETRY | ERROR_END); //ͨ��������ѹʧ��
				}
			}
		break;
		case STEP5:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				nLocation = ENUM_OUTSIDE; //�ӿ�����������
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_WarmDown
*****************************************************
Descriptions:  �����½�
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_WarmDown(void)
{
	stOpCtrl *CurrentOp = &OpWarmDown;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_OFF)
			{
				if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)
				{
					CurrentOp->nStep = STEP2;
				}
				else
				{
					SetOpErrorInfo(CurrentOp, ERROR_WARMING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //���Ȳ��ִ������쳣
				}
			}
			else
			{
				CurrentOp->nResult = 0xFFFF;
			}
		break;

		case STEP2:
			dm_ctrl_one(DM_WARM_CONTROL, RELAY5_ON);
			SetOpTimeDelay(CurrentOp, 2000);
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = 0xFFFF;
			}
			else if (CheckOpTimeDelay(CurrentOp))
			{
				if (CurrentOp->nResumeMode == ERROR_RETRY)
				{
					CurrentOp->nStep = STEP2;
				}
				else
				{
					SetOpErrorInfo(CurrentOp, ERROR_WARMING_DOWN_FAIL, ERROR_RETRY | ERROR_END); //�����½�ʧ��
				}
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_WarmRise
*****************************************************
Descriptions:  ��������
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_WarmRise(void)
{
	stOpCtrl *CurrentOp = &OpWarmRise;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_OFF)
		{
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_WARMING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //���Ȳ��ִ������쳣
			}
		}
		else
		{
			CurrentOp->nResult = 0xFFFF;
		}
		break;

	case STEP2:
		dm_ctrl_one(DM_WARM_CONTROL, RELAY5_OFF);
		SetOpTimeDelay(CurrentOp, 2000);
		CurrentOp->nStep = STEP3;
		break;
	case STEP3:
		if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)
		{
			CurrentOp->nResult = 0xFFFF;
		}
		else if (CheckOpTimeDelay(CurrentOp))
		{
			if (CurrentOp->nResumeMode == ERROR_RETRY)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_WARMING_DOWN_FAIL, ERROR_RETRY | ERROR_END); //���Ȳ��ִ������쳣
			}
		}
		break;
	default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_CoolDown
*****************************************************
Descriptions:  ��ȴ�½�
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_CoolDown(void)
{
	stOpCtrl *CurrentOp = &OpCoolDown;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_OFF)
		{
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //��ȴλ�ô������쳣
			}
		}
		else
		{
			CurrentOp->nResult = 0xFFFF;
		}
		break;

	case STEP2:
		dm_ctrl_one(DM_COOL_CONTROL, RELAY6_ON);
		SetOpTimeDelay(CurrentOp, 2000);
		CurrentOp->nStep = STEP3;
		break;
	case STEP3:
		if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_ON)
		{
			CurrentOp->nResult = 0xFFFF;
		}
		else if (CheckOpTimeDelay(CurrentOp))
		{
			if (CurrentOp->nResumeMode == ERROR_RETRY)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_DOWN_FAIL, ERROR_RETRY | ERROR_END); //��ȴ�½�ʧ��
			}
		}
		break;
	default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_CoolRise
*****************************************************
Descriptions:  ��������
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_CoolRise(void)
{
	stOpCtrl *CurrentOp = &OpCoolRise;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_OFF)
		{
			if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //��ȴλ�ô������쳣
			}
		}
		else
		{
			CurrentOp->nResult = 0xFFFF;
		}
		break;

	case STEP2:
		dm_ctrl_one(DM_COOL_CONTROL, RELAY6_OFF);
		SetOpTimeDelay(CurrentOp, 2000);
		CurrentOp->nStep = STEP3;
		break;
	case STEP3:
		if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON)
		{
			CurrentOp->nResult = 0xFFFF;
		}
		else if (CheckOpTimeDelay(CurrentOp))
		{
			if (CurrentOp->nResumeMode == ERROR_RETRY)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_RISE_FAIL, ERROR_RETRY | ERROR_END); //��ȴ����ʧ��
			}
		}
		break;
	default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_PresDown
*****************************************************
Descriptions:  ��ѹ�½�
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_PresDown(void)
{
	stOpCtrl *CurrentOp = &OpPresDown;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_OFF)
		{
			if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //��ѹλ�ô������쳣
			}
		}
		else
		{
			CurrentOp->nResult = 0xFFFF;
		}
		break;

	case STEP2:
		dm_ctrl_one(DM_PRES_CONTROL, RELAY7_ON);
		SetOpTimeDelay(CurrentOp, 2000);
		CurrentOp->nStep = STEP3;
		break;
	case STEP3:
		if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON)
		{
			CurrentOp->nResult = 0xFFFF;
		}
		else if (CheckOpTimeDelay(CurrentOp))
		{
			if (CurrentOp->nResumeMode == ERROR_RETRY)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_DOWN_FAIL, ERROR_RETRY | ERROR_END); //��ѹ�½�ʧ��
			}
		}
		break;
	default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_PresRise
*****************************************************
Descriptions:  ��ѹ����
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_PresRise(void)
{
	stOpCtrl *CurrentOp = &OpPresRise;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_OFF)
		{
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //��ѹλ�ô������쳣
			}
		}
		else
		{
			CurrentOp->nResult = 0xFFFF;
		}
		break;

	case STEP2:
		dm_ctrl_one(DM_PRES_CONTROL, RELAY7_OFF);
		SetOpTimeDelay(CurrentOp, 2000);
		CurrentOp->nStep = STEP3;
		break;
	case STEP3:
		if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON)
		{
			CurrentOp->nResult = 0xFFFF;
		}
		else if (CheckOpTimeDelay(CurrentOp))
		{
			if (CurrentOp->nResumeMode == ERROR_RETRY)
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_RISE_FAIL, ERROR_RETRY | ERROR_END); //��ѹ����ʧ��
			}
		}
		break;
	default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_OpeningPressure
*****************************************************
Descriptions:  ѹ����
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_OpeningPressure(void)
{
	stOpCtrl *CurrentOp = &OpOpeningPressure;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		dm_ctrl_one(DM_ALL_CONTROL, RELAY8_ON);
		CurrentOp->nStep = STEP2;
		break;

	case STEP2:
		CurrentOp->nResult = 0xFFFF;
		break;
	default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_ClosingPressure
*****************************************************
Descriptions:  ѹ����
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ClosingPressure(void)
{
	stOpCtrl *CurrentOp = &OpClosingPressure;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
	case START:
		CurrentOp->nStep = STEP1;
		break;

	case STEP1:
		dm_ctrl_one(DM_ALL_CONTROL, RELAY8_OFF);
		CurrentOp->nStep = STEP2;
		break;

	case STEP2:
		CurrentOp->nResult = 0xFFFF;
		break;
	default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_MakeCardStart
*****************************************************
Descriptions:  �ƿ�����
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_MakeCardStart(void)
{
	stOpCtrl *CurrentOp = &OpMakeCardStart;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpSendCardToWarm); //����ͨ��ǰ���ӳ���
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp, &OpSendCardToWarm)) //�ɹ�������һ��,���ɹ�,�ӳ��򱨴�.
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpWarmDown); //������ѹ��ѹ�ӳ���
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp, &OpWarmDown)) //�ж��ӳ�������Ƿ�ɹ�
			{
				SetOpTimeDelay(CurrentOp, parameter[WARM_TIME] * 500); //���ݴ������Ĳ�����ʱѹ�¼���ʱ��
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if (CheckOpTimeDelay(CurrentOp)) //�ж���ʱʱ���Ƿ�
			{
				CurrentOp->nStep = STEP6;
				SetOpTimeDelay(CurrentOp, parameter[WARM_TIME] * 500); //���ݴ������Ĳ�����ʱѹ�¼���ʱ��
			}

		break;
		case STEP6:
			if (CheckOpTimeDelay(CurrentOp)) //�ж���ʱʱ���Ƿ�
			{
				StartChildOp(CurrentOp, &OpWarmRise); //����������ѹģ��
				CurrentOp->nStep = STEP7;
			}
		break;
	case STEP7:
		if (IsChildOpOK(CurrentOp, &OpWarmRise))
		{
			CurrentOp->nStep = STEP8;
			SetOpTimeDelay(CurrentOp, 500); //��ʱ500msʱ��,�е�ʱ���ճ��
		}
		break;
		case STEP8:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP9;
			}
		break;
		case STEP9:
			StartChildOp(CurrentOp, &OpSendCardToCool); //�ѿ��ƶ�����ѹģ��
			CurrentOp->nStep = STEP10;
		break;
		case STEP10:
			if (IsChildOpOK(CurrentOp, &OpSendCardToCool)) //�Ƿ�ɹ�
			{
				CurrentOp->nStep = STEP11;
			}
		break;
		case STEP11:
			StartChildOp(CurrentOp, &OpCoolDown); //��ѹģ����ѹ
			CurrentOp->nStep = STEP12;
		break;
		case STEP12:
			if (IsChildOpOK(CurrentOp, &OpCoolDown)) //�Ƿ�ִ�гɹ�
			{
				SetOpTimeDelay(CurrentOp, parameter[COOL_TIME] * 1000); //���ݴ������Ĳ�����ʱ��ѹʱ��
				CurrentOp->nStep = STEP13;
			}
		break;
		case STEP13:
			if (CheckOpTimeDelay(CurrentOp)) //����ʱ��
			{
				StartChildOp(CurrentOp, &OpCoolRise); //��ѹģ������
				CurrentOp->nStep = STEP14;
			}
		break;
		case STEP14:
			if (IsChildOpOK(CurrentOp, &OpCoolRise)) //ִ�гɹ�
			{
				StartChildOp(CurrentOp, &OpChannalBackORG); //ͨ������ԭ��
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_WarmEnforceRise
*****************************************************
Descriptions:  ǿ����ѹ����
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_WarmEnforceRise(void)
{
	stOpCtrl *CurrentOp = &OpWarmEnforceRise;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			dm_ctrl_one(DM_WARM_CONTROL, RELAY5_OFF); //����
			SetOpTimeDelay(CurrentOp, 2000);						//��ʱ
			CurrentOp->nStep = STEP2;
		break;
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_WarmEnforceDown
*****************************************************
Descriptions:  ǿ����ѹ�½�
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_WarmEnforceDown(void)
{
	stOpCtrl *CurrentOp = &OpWarmEnforceDown;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			dm_ctrl_one(DM_WARM_CONTROL, RELAY5_ON); //�½�
			SetOpTimeDelay(CurrentOp, 2000);					 //��ʱ
			CurrentOp->nStep = STEP2;
		break;
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_OpCoolEnforceRise
*****************************************************
Descriptions:  ǿ����ѹ����
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_OpCoolEnforceRise(void)
{
	stOpCtrl *CurrentOp = &OpCoolEnforceRise;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
				dm_ctrl_one(DM_COOL_CONTROL, RELAY6_OFF); //����
				SetOpTimeDelay(CurrentOp, 2000);			  //��ʱ
				CurrentOp->nStep = STEP2;
		break;
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_CoolEnforceDown
*****************************************************
Descriptions:  ǿ����ѹ�½�
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_CoolEnforceDown(void)
{
	stOpCtrl *CurrentOp = &OpCoolEnforceDown;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			dm_ctrl_one(DM_COOL_CONTROL, RELAY6_ON); //�½�
			SetOpTimeDelay(CurrentOp, 2000);		//��ʱ
			CurrentOp->nStep = STEP2;
		break;
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}

/****************************************************
Function Name: Op_CutCardPosition
*****************************************************
Descriptions: ������λ
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_CutCardPosition(void)
{
	stOpCtrl *CurrentOp = &OpCutCardPosition;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //������ѹ��λ
			{
				sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], 8000); //�ߵ���ѹ��λ
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //����ѹ��λ
			{
				sm_stop(SM_CHANNEL);//ֹͣͨ�����
				SetOpTimeDelay(CurrentOp, 100); //��ʱ
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:																																														//�ߵ��ü���λ
			sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED], parameter[CUT_CARD_POSITION]); //�ߵ���ѹ��λ
			CurrentOp->nStep = STEP5;
		break;
		case STEP5:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}

/****************************************************
Function Name: Op_CutCard
*****************************************************
Descriptions: ����
*****************************************************
Calls:
*****************************************************
Input  parameter: None	
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_CutCard(void)
{
	stOpCtrl *CurrentOp = &OpCutCard;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpCutCardPosition); //���е�����λ��
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp, &OpCutCardPosition)) //�����Ƿ�ɹ�
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpPresDown); //��ѹ
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp, &OpPresDown)) //�����Ƿ�ɹ�
			{
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			StartChildOp(CurrentOp, &OpPresRise); //����
			CurrentOp->nStep = STEP6;
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp, &OpPresRise)) //�����Ƿ�ɹ�
			{
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			StartChildOp(CurrentOp, &OpCutCardOriginBack); //���ؼ���֮ǰ��λ��
			CurrentOp->nStep = STEP8;
		break;
		case STEP8:
			if (IsChildOpOK(CurrentOp, &OpCutCardOriginBack)) //�����Ƿ����
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_RightClampClose
*****************************************************
Descriptions: Right clamp close
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_RightClampClose(void)
{
	stOpCtrl *CurrentOp = &OpRightClampClose;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if ((input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON)
				&& (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_OFF))//Clamp is close status
			{
				CurrentOp->nStep = STEP_SUCCESS; //Return success
			}
			else if ((input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON) 
				&& (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_OFF))//Clamp is open status
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_CLAMP_CLOSE); //Run Clamp direct current motor
				SetOpTimeDelay(CurrentOp,500); //Delay 0.5s
				CurrentOp->nStep = STEP2;
			}
			else //Other status,������������������⵽�򶼼�ⲻ��
			{
				SetOpErrorInfo(CurrentOp,ERROR_RIGHT_CLAMP_CLOSE_FAIL_SERSOR_FAULT,ERROR_END); //10014 Right Sensor fault clamp close faile
			}	
		break;
		case STEP2:
			if ((input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON)
				&& (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_OFF))//Already close clamp
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE); //Stop direct current motor
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else if (CheckOpTimeDelay(CurrentOp)) //��ʱʱ�䵽,������δ�ر�
			{		
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE); //Stop direct current motor
				SetOpErrorInfo(CurrentOp,ERROR_RIGHT_CLAMP_CLOSE_FAIL_MOTOR_FAULT,ERROR_END); //10015 Right Motor fault clamp close faile
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_RightClampLoose
*****************************************************
Descriptions:  Right clamp loose
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_RightClampLoose(void)
{
	stOpCtrl *CurrentOp = &OpRightClampLoose;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}
	switch (CurrentOp->nStep)
	{				
		case START:
				CurrentOp->nStep = STEP1;
		break;
		case STEP1:
				if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON 
				 && input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_OFF ) //Already is open status
				{
						CurrentOp->nStep = STEP_SUCCESS; //return success	
				}
				else if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_OFF 
				 && input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON ) //Already is open status
				{
						dm_ctrl_one(DM_CLAMP_RIGHT,DM_CLAMP_OPEN); //Open right Clamp
						SetOpTimeDelay(CurrentOp,500);//Delay 0.5S
						CurrentOp->nStep = STEP2;
				}
				else
				{
					SetOpErrorInfo(CurrentOp,ERROR_RIGHT_CLAMP_OPEN_FAIL_SERSOR_FAULT,ERROR_END); // 10016 Right Sensor fault clamp open faile
				}
				
		break;
		case STEP2:
				if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON 
				 && input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_OFF ) //Run to open status
				{
						dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE); //Stop direct current motor 
						CurrentOp->nStep = STEP_SUCCESS;
				}
				else if(CheckOpTimeDelay(CurrentOp)) 			//��ʱʱ�䵽��,������δ��
				{
						dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE); //Stop direct current motor	
						SetOpErrorInfo(CurrentOp,ERROR_RIGHT_CLAMP_OPEN_FAIL_MOTOR_FAULT,ERROR_END); // 10017 Right Motor fault clamp open faile					
				}		
		break;
		case STEP_SUCCESS:
				CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_LeftClampClose
*****************************************************
Descriptions: Left clamp close
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_LeftClampClose(void)
{
	stOpCtrl *CurrentOp = &OpLeftClampClose;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if ((input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON)
				&& (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_OFF))//Clamp is close status
			{
				CurrentOp->nStep = STEP_SUCCESS; //Return success
			}
			else if ((input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON) 
				&& (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_OFF))//Clamp is close status
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_CLAMP_CLOSE); //Run Clamp direct current motor
				SetOpTimeDelay(CurrentOp,500); //Delay 0.5s
				CurrentOp->nStep = STEP2;
			}
			else //Other status,������������������⵽�򶼼�ⲻ��
			{
				SetOpErrorInfo(CurrentOp,ERROR_LEFT_CLAMP_CLOSE_FAIL_SERSOR_FAULT,ERROR_END); //10018 Left Sensor fault clamp close faile
			}	
		break;
		case STEP2:
			if ((input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON)
				&& (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_OFF))//Already close clamp
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //Stop direct current motor
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else if (CheckOpTimeDelay(CurrentOp)) //��ʱʱ�䵽,������δ�ر�
			{		
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //Stop direct current motor
				SetOpErrorInfo(CurrentOp,ERROR_LEFT_CLAMP_CLOSE_FAIL_MOTOR_FAULT,ERROR_END); //10019 Left Motor fault clamp close faile
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_LeftClampLoose
*****************************************************
Descriptions:  Left clamp loose
*****************************************************
Calls: input_get_one, dm_ctrl_one,SetOpTimeDelay
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_LeftClampLoose(void)
{
	stOpCtrl *CurrentOp = &OpLeftClampLoose;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}
	switch (CurrentOp->nStep)
	{				
		case START:
				CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON 
				&& input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_OFF ) //Already is open status
			{
					CurrentOp->nStep = STEP_SUCCESS; //return success	
			}
			else if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_OFF 
				&& input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON ) //Already is open status
			{
					dm_ctrl_one(DM_CLAMP_LEFT,DM_CLAMP_OPEN); //Open right Clamp
					SetOpTimeDelay(CurrentOp,500);//Delay 0.5S
					CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp,ERROR_LEFT_CLAMP_OPEN_FAIL_SERSOR_FAULT,ERROR_END); // 10020 Left Sensor fault clamp open faile
			}
				
		break;
		case STEP2:
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON 
				&& input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_OFF ) //Run to open status
			{
					dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //Stop direct current motor 
					CurrentOp->nStep = STEP_SUCCESS;
			}
			else if(CheckOpTimeDelay(CurrentOp)) 			//��ʱʱ�䵽��,������δ��
			{
					dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //Stop direct current motor	
					SetOpErrorInfo(CurrentOp,ERROR_LEFT_CLAMP_OPEN_FAIL_MOTOR_FAULT,ERROR_END); // 10021 Left Motor fault clamp open faile					
			}		
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
}
/****************************************************
Function Name: Op_ClampClose
*****************************************************
Descriptions: Close left and right Clamp
*****************************************************
Calls:OpRightClampLoose,OpRightClampClose
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ClampClose(void)
{
		stOpCtrl *CurrentOp = &OpClampClose;
		if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
		{
			return;
		}
		switch (CurrentOp->nStep)
		{
			case START:
				CurrentOp->nStep = STEP1;
			break;
			case STEP1:
				StartChildOp(CurrentOp,&OpRightClampClose);  //�����ұ߼��ӹر��ӳ���
				CurrentOp->nStep = STEP2;
			break;
			case STEP2:
				if (IsChildOpOK(CurrentOp,&OpRightClampClose))  //�����Ƿ�ɹ�
				{
						CurrentOp->nStep = STEP3;
				}
			break;
			case STEP3:
				StartChildOp(CurrentOp,&OpLeftClampClose); //������߼����ӳ���
				CurrentOp->nStep = STEP4;
			break;
			case STEP4:
				if (IsChildOpOK(CurrentOp,&OpLeftClampClose)) //�����Ƿ�ɹ�
				{
					CurrentOp->nStep = STEP_SUCCESS;
				}
			break;
			case STEP_SUCCESS:
				CurrentOp->nResult = 0xffff;
			break;
			default:
			break;
		}
		DealResult(CurrentOp); //�������ʹ��
}
/****************************************************
Function Name: Op_ClampLoose
*****************************************************
Descriptions: Loose left and right Clamp
*****************************************************
Calls: OpRightClampLoose,OpLeftClampLoose
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ClampLoose(void)
{
	stOpCtrl *CurrentOp = &OpClampLoose;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp,&OpRightClampLoose);  //�����ұ߼��ӹر��ӳ���
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpRightClampLoose))  //�����Ƿ�ɹ�
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp,&OpLeftClampLoose); //������߼����ӳ���
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpLeftClampLoose)) //�����Ƿ�ɹ�
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;

	}
	DealResult(CurrentOp); //�������ʹ��
}
/****************************************************
Function Name: Op_ClampReset
*****************************************************
Descriptions:  Reset Clamp
*****************************************************
Calls: OpClampClose,OpClampLoose,StartChildOp,IsChildOpOK
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ClampReset(void)
{
	stOpCtrl *CurrentOp = &OpClampReset;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp,&OpClampClose);
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpClampClose))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp,&OpClampLoose);
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpClampLoose))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	
}
/****************************************************
Function Name: Op_ShiftTrolley
*****************************************************
Descriptions:  Shift trolley
*****************************************************
Calls:sm_run, IsSmRunFinish         
*****************************************************
Input  parameter: iStepNum
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ShiftTrolley(int iStepNum)
{
	stOpCtrl *CurrentOp = &OpShiftTrolley;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))                                  
	{
		return;	
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;			
		break;
		case STEP1:
			if (iStepNum >= 0) //����Ϊ��  
			{
				sm_run(SM_TROLLEY,DIR_TROLLEY_FORWARD,g_sTrolleyPara.uiSpeed,iStepNum); //��ǰ��N�i
			}
			else  //����Ϊ��
			{
				sm_run(SM_TROLLEY,DIR_TROLLEY_BACKWARD,g_sTrolleyPara.uiSpeed,iStepNum); //�����N�i
			}
			CurrentOp->nStep = STEP2;	
		break;
		case STEP2:
			if (IsSmRunFinish(SM_TROLLEY)) //����Ƿ�����
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else
			{
				SetOpErrorInfo(CurrentOp,ERROR_TROLLEY_STEP_MOTOR_FAULT,ERROR_END);//10022 ����С������   
			}	
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff; //���سɹ���
		break; 
		default:
		break;
	}
	DealResult(CurrentOp); //�嶯��ʹ��
}

void Op_TrolleyDown(void)
{
	stOpCtrl *CurrentOp = &OpTrolleyDown;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))                             
	{
		return;	
	}
    switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;	
		break;
		case STEP1:
			if ((input_get_one(SN_TROLLEY_DOWN) == SENSOR_TYPE_BEAM_ON) 
			  ||(input_get_one(SN_TROLLEY_UP) == SENSOR_TYPE_BEAM_OFF))//�Ѿ����½�λ
			{
				CurrentOp->nStep = STEP_SUCCESS;		
			}
			else if ((input_get_one(SN_TROLLEY_DOWN) == SENSOR_TYPE_BEAM_OFF) 
			  ||(input_get_one(SN_TROLLEY_UP) == SENSOR_TYPE_BEAM_ON))//�Ѿ�������λ
			  {
						sm_123


			  }	
		break;


 		default:
		break;
	}                                                                     										


}


void Op_ReverseFront(void)
{
	stOpCtrl *CurrentOp = &OpReverseFront;
	if((CurrentOp->bEn == false) || (CurrentOp->bFlagPause))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if ()
			{
				
			}
			
		break;

		default:
		break;
	}
}




void Op_TearFilm(void)
{


}




/****************************************************
Function Name: StepCtrlMachine
*****************************************************
Descriptions:  �����豸���ж�����ִ��
*****************************************************
Calls:
*****************************************************
Input  parameter: None
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void StepCtrlMachine(void)
{
	Op_ResetModule();						 //1 ��λ
	Op_ResetChannal(); 					   	 //2 ��λ�Ϳ�ͨ��
	Op_SendCardToWarm();				   	 //3 ��������ѹ��λ
	Op_SendCardToCool();				   	 //4 ��������ѹ��λ
	Op_ChannalBackORG();    			     //5 ͨ���ص�ԭ��
	Op_WarmRise();						   	 //6 �����½�
	Op_WarmDown();						   	 //7 ��������
	Op_CoolRise();						   	 //8 ��ȴ�½�
	Op_CoolDown();						   	 //9 ��ȴ����
	Op_PresRise();						   	 //10 ��ѹ�½�
	Op_PresDown();						   	 //11 ��ѹ����
	Op_OpeningPressure();				   	 //12 ѹ����
	Op_ClosingPressure();				   	 //13 ѹ����						
	Op_MakeCardStart();					   	 //14 �ƿ�����
	Op_WarmEnforceRise();				     //15 ��ѹǿ������
	Op_WarmEnforceDown();				     //16 ��ѹǿ���½�
	Op_OpCoolEnforceRise();				     //17 ��ѹǿ������
	Op_CoolEnforceDown();				     //18 ��ѹǿ���½�
	Op_CutCardPosition();				     //19 ���ü�����λ��
	Op_CutCard();						     //20 ����
	Op_RightClampClose();			     	 //21 �Ҳ���ӹر� 			
  	Op_RightClampLoose();				     //22 �Ҳ���Ӵ�          
	Op_LeftClampClose();					 //23 �����ӹر�
	Op_LeftClampLoose();					 //24 �����Ӵ�
	Op_ClampClose();						 //25 ���߼��ӹر�
	Op_ClampLoose();						 //26 ������Ӵ�

}






