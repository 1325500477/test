//Tab size: 4;	Indent size: 4
//#include "stdbool.h"

#ifndef __OP_CTRL_H__
#define __OP_CTRL_H__

//#define bool _Bool

//#define FOREVER		0xFFFF


enum EnumResumeMode				// �豸���г������Ͷ���
{
	ERROR_RETRY = 0x01,				// ���Զ���
	ERROR_IGNORE = 0x02,			// ���Ե�ǰ����
	ERROR_END = 0x04				// �޷������˳����
};

enum enum_run_mode
{
    OP_RUN_MODE_NORMAL = 0,
    OP_RUN_MODE_STUB = 1,
};

typedef struct _stOpCtrl
{
	unsigned char bEn;				// ���ܶ�������
	unsigned char nStep;			// ��֧��������

	unsigned int nDelay;			// ��ʱ
	unsigned int nStartTime;		// ������ʼʱ��
	unsigned int nEndTime;			// ��������ʱ��
	unsigned int nUseTime;			// ����ʹ��ʱ�䣨������nEndTime-nStartTime���������ʡ�ڴ棩
	unsigned long nDelayStartTime;	// ��ʱ��ʼʱ��
    unsigned int nParameter;        // ��̬���´����̲�����Ŀǰֻ֧�������ֽڣ�

	unsigned char bRunMode;			// ��������ģʽ��0--�������У� 1--StepByStep����ģʽ���ɡ���׮����ͣ�����ڹ۲죩
	unsigned char bFlagPause;		// ������ͣ������������ܣ�1.��������ˣ�2.������׮����ͣ��
    unsigned char bFlagStub;        // �����ѱ���׮
	unsigned int nResult;			// ͬmErrorCode��0x0000--�����У�0xFFFF--�����ɹ���0xFFFE--��ʼ��������--����
	unsigned char nResumeMode;		// ������ģʽ��1--���ԣ�2--���ԣ�3--�޷�����������
	unsigned char nAlarmMode;		// ��ERROR_RETRY��ERROR_INGORE�Լ�ERROR_END����������ϣ�nButtonNum��
	unsigned char nPreStep;			// ��ת����ǰ����case��Step
	unsigned char nLastStep;		// �Ĵ���һ��PreStepֵ
	unsigned char nCmd;				// ��ǰ���̱��
    unsigned char bFlagPopUp;		// ������ʾ��ֻ�ڳ������ʱ�򵯳����壩

	void * OpParent;				// ָ�����̣����޸����̵��ã���ָ����
//	void * OpChild;					// ָ�������̣����������̵��ã���ָ����
	void * OpChildError;			// �����̳�������Ӷ�������ʱ���õ���
} stOpCtrl;

void InitOpStruct(stOpCtrl *operation, unsigned char nCmdID, unsigned int nResult);			// ��ʼ������
void SetOpTimeDelay(stOpCtrl*op,unsigned int delaytime);
unsigned char CheckOpTimeDelay(stOpCtrl *op);                                   // ����Ƿ�ʱ�������ʱ��������
unsigned char StartOp(stOpCtrl *operation);									    // ��ʼ��������
unsigned char StartChildOp(stOpCtrl * Operation, stOpCtrl * OperationChild);    // ��ʼ������
unsigned char StartOpByMode(stOpCtrl *operation, unsigned char nMode);	        // ��ʼ�������̣�����ָ������ģʽ��0-ֱ�����У�1-�������У�
void OpPlugOneStub(stOpCtrl *Operation);							            // ���������н��д�һ��׮����StepByStepģʽ�»�����׮����ͣ
void OpUnplugOneStub(stOpCtrl *Operation);                                      // ���������н��г���һ��׮��������������
void SetOpRunMode(stOpCtrl *Operation, unsigned char nMode);			        // ���ö���ִ�е�ģʽ��0-ֱ�����У�1-�������У�

unsigned char ResumeOp(stOpCtrl *operation, unsigned char nResumeMode);         // ���̱����˹���������/����/������������Ϻ󣬻ָ�����

bool IsChildOpOK(stOpCtrl *operation, stOpCtrl *operationChild);
void SetOpErrorInfo(stOpCtrl *operation, unsigned int nErrorCode, unsigned char nErrorType);
void DealResult(stOpCtrl *Operation);
extern unsigned short int CheckResult(stOpCtrl *Operation);

#endif
