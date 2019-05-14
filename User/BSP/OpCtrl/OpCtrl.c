#include <string.h>
#include "stm32f10x.h"
#include "bsp.h"

/***************************************************************************************************************
  * �������ƣ� SetOpTimeDelay
  * ���������� ������ʱʱ��
  * ����˵����
****************************************************************************************************************/
void SetOpTimeDelay(stOpCtrl*op,unsigned int delaytime)
{
	op->nDelayStartTime=systemTime;
	op->nDelay=delaytime;
}

/***************************************************************************************************************
  * �������ƣ� CheckOpTimeDelay
  * ���������� �����ʱʱ���Ƿ�
  * ����˵����
****************************************************************************************************************/
unsigned char CheckOpTimeDelay(stOpCtrl *op)
{
	unsigned long long tmpTime;
	tmpTime = systemTime;  //��ǰʱ��

	if(op->nDelayStartTime >tmpTime)	//��ʼʱ����ڵ�ǰʱ��,��ʱ�����
		tmpTime |= 0x100000000;

	tmpTime = tmpTime - op->nDelayStartTime;   //��ǰʱ���ȥ��ʼʱ��

	if((unsigned int)tmpTime >= op->nDelay)   //��ǰʱ���ȥ��ʼʱ��,���ʱ�������ʱʱ��,��ʱʱ�䵽
		return true;
	else
		return false;
}

/***************************************************************************************************************
  * �������ƣ� StartOp
  * ���������� ����һ���Ӷ���
  * ����˵������1������ʽ����
****************************************************************************************************************/
unsigned char StartOp(stOpCtrl * Operation)
{
	return StartOpByMode(Operation, OP_RUN_MODE_NORMAL);
}

/***************************************************************************************************************
  * �������ƣ� StartOpByMode
  * ���������� ����һ���Ӷ���������ָ������ģʽ
  * ����˵����  nMode  0-ֱ�����У�1-��������
****************************************************************************************************************/
unsigned char StartOpByMode(stOpCtrl *Operation, unsigned char nMode)
{
// 2016-04-10��ȥ������ʹ�ܵĴ���
	//if(Operation->bEn)			// �����У���������ָ��
	//{
	//		return false;
	//}
	Operation->bEn = true;
	Operation->nStep = START;

    Operation->bRunMode = nMode;
	Operation->bFlagPause = false;
    Operation->bFlagStub = false;
	Operation->nResult = 0;
	Operation->nResumeMode = 0;
	Operation->nAlarmMode = 0;
	Operation->nPreStep = START;
	Operation->nLastStep = START;
    Operation->bFlagPopUp = false;

	Operation->OpParent = Operation;
//	Operation->OpChild = Operation;
	Operation->OpChildError = Operation;

	Operation->nStartTime = systemTime;	//��ʱ���жϻ�ȡ׼ȷʱ��

	return true;
}

/***************************************************************************************************************
  * �������ƣ� StartChildOp
  * ���������� ����һ���Ӷ���
  * ����˵������1�����ʱ�䣺2016-04-20��
			  ��2�����������������صĴ���
****************************************************************************************************************/
unsigned char StartChildOp(stOpCtrl * Operation, stOpCtrl * OperationChild)
{
    OperationChild->bEn = true;
    OperationChild->nStep = START;

    OperationChild->bRunMode = OP_RUN_MODE_NORMAL;
    OperationChild->bFlagPause = false;
    OperationChild->bFlagStub = false;
    OperationChild->nResult = 0;
    OperationChild->nResumeMode = 0;
    OperationChild->nAlarmMode = 0;
    OperationChild->nPreStep = START;
    OperationChild->nLastStep = START;
    OperationChild->bFlagPopUp = false;

    //	Operation->OpChild = OperationChild;
    Operation->OpChildError = OperationChild;
    OperationChild->OpParent = Operation;

    OperationChild->nStartTime = systemTime;	//��ʱ���жϻ�ȡ׼ȷʱ��

    return true;
}


/***************************************************************************************************************
  * �������ƣ� ResumeOp
  * ���������� �ָ�����
  * ����˵������1�����ʱ�䣺2016-04-20��
					 ��1�������ڽ��д����������ָ̻�
****************************************************************************************************************/
unsigned char ResumeOp(stOpCtrl * Operation,unsigned char nMode)
{
	stOpCtrl *OpTemp;

	Operation->bEn = true;
	Operation->nResumeMode = nMode;
	Operation->nAlarmMode = 0;

	OpTemp = Operation;

	if (OpTemp->nResult != 0xFFFF)
		OpTemp->nResult = 0;

	OpTemp->bFlagPause  = 0;

	while (OpTemp->OpParent != OpTemp)	// ˵���и�����
	{
		OpTemp = OpTemp->OpParent ;		// �ҵ�������
		if (OpTemp->nResult != 0xFFFF)
			OpTemp->nResult = 0;
		OpTemp->bFlagPause = 0;			// ���������ָ�
	}
	return true;
}


/***************************************************************************************************************
  * �������ƣ� InitOpStruct
  * ���������� ��ʼ��һ�������ṹ��
  * ����˵������1����һ�������ṹ�����س�Ա���г�ʼ������
****************************************************************************************************************/
void InitOpStruct(stOpCtrl *Operation,unsigned char nCmdID, unsigned int nCmdResult)
{
    Operation->bEn = false;
    Operation->nStep = START;

    Operation->bRunMode = OP_RUN_MODE_NORMAL;
    Operation->bFlagPause = false;
    Operation->bFlagStub = false;
    //Operation->nResult = 0;
    Operation->nResumeMode = 0;
    Operation->nAlarmMode = 0;
    Operation->nPreStep = START;
    Operation->nLastStep = START;
    Operation->bFlagPopUp = false;

    Operation->nCmd = nCmdID;				//���������
    Operation->nResult = nCmdResult;		//���ý��

    Operation->OpParent = Operation;		//���ø�����ָ��
    //	Operation->OpChild = Operation;			//�����Ӷ���ָ��
    Operation->OpChildError = Operation;	//���ô�����ָ��	
}

//����0x00˵���������ڽ�����
//0xFFFF��ʾ�����ɹ����
//����ֵ��ʾ�����������,���һֱ����,��λ���������������
unsigned short int CheckResult(stOpCtrl *Operation)
{
	if(Operation->bEn)
	{
		return 0x00;      // ����������
	}
	else
	{
		return Operation->nResult;  //0xFFFF ��ʾ�ɹ����
	}
}


//������ɺ��嶯��ʹ��,�����㶯��ʹ��ʱ��
void DealResult(stOpCtrl *Operation)
{
	if (Operation->nStep != Operation->nLastStep)
	{
		Operation->nPreStep=Operation->nLastStep;
		Operation->nLastStep=Operation->nStep;
	}

	Operation->nResumeMode = 0;

	if  (Operation->nResult == 0xFFFF)
	{
		Operation->nAlarmMode = 0;
		Operation->bEn = false;

		Operation->nEndTime=systemTime;

		if ((Operation->nEndTime)<(Operation->nStartTime))		//��ʱ���
		{
			unsigned long tmpTime;

			tmpTime=Operation->nEndTime;
			tmpTime |= 0x10000;
			Operation->nUseTime=tmpTime-Operation->nStartTime;//���㱾�ζ�������ʱ��
		}
		else
		{
			Operation->nUseTime=Operation->nEndTime-Operation->nStartTime;//���㱾�ζ�������ʱ��
		}
	}
	else if(Operation->nResult > 0)
	{
		Operation->bFlagPause = 1;
	}
}


/***************************************************************************************************************
  * �������ƣ� IsChildOpOK
  * ���������� �ж��Ӷ���ִ�н��
  * ����˵������1����ʧ�ܣ����Ӷ�����ص���Ϣ�������������������࣬������ָ�룩���ص���������
****************************************************************************************************************/
bool IsChildOpOK(stOpCtrl *Operation, stOpCtrl *OperationChild)
{
	if (OperationChild->nResult == 0xFFFF)
	{
		Operation->OpChildError = Operation;
//		Operation->OpChild = Operation;
		return true;
	}
	else if (OperationChild->nResult != 0x00)	// ����Ӷ�������
	{
		Operation->bFlagPopUp = true;
		Operation->OpChildError = OperationChild;
		Operation->nResult = OperationChild->nResult;
		Operation->nAlarmMode = OperationChild->nAlarmMode;
		return false;
	}
	else
		return false;
}


/***************************************************************************************************************
  * �������ƣ� SetOpErrorInfo
  * ���������� ������ش�����Ϣ
  * ����˵������1��nAlarmMode�����������ERROR_RETRY�����ԣ���ERROR_INGORE�����ԣ��Լ�ERROR_END
									���������������������	����
****************************************************************************************************************/
void SetOpErrorInfo(stOpCtrl *Operation, unsigned int nErrorCode, unsigned char nAlarmMode)
{
	Operation->bFlagPopUp = true;
	Operation->nResult = nErrorCode;
	Operation->nAlarmMode = nAlarmMode;
}

/***************************************************************************************************************
  * �������ƣ� OpSetOneStub
  * ���������� ���������н��д�һ��׮����StepByStepģʽ�»�����׮����ͣ
  * ����˵����
****************************************************************************************************************/
void OpPlugOneStub(stOpCtrl *Operation)
{
    //char cBuf[32];
    //memset(cBuf,0,sizeof(cBuf));

    Operation->bFlagStub = true;

    //sprintf(cBuf, "Op[%d] running at step[%d] ",Operation->nCmd, Operation->nStep);
    //debug_info(cBuf);
    //debug_info("and set a stub!\n");
}

/***************************************************************************************************************
  * �������ƣ� OpUnplugOneStub
  * ���������� ���������н��г���һ��׮��������������
  * ����˵����
****************************************************************************************************************/
void OpUnplugOneStub(stOpCtrl *Operation)
{
    Operation->bFlagStub = false;
}

/***************************************************************************************************************
  * �������ƣ� SetOpRunMode
  * ���������� ����Op���̹���ģʽ
  * ����˵����  nMode 0--�������У� 1--StepByStep����ģʽ���ɡ���׮����ͣ�����ڹ۲�
****************************************************************************************************************/
void SetOpRunMode(stOpCtrl *Operation, unsigned char nMode)
{
    Operation->bRunMode = nMode;
}
