//Tab size: 4;	Indent size: 4
//#include "stdbool.h"

#ifndef __OP_CTRL_H__
#define __OP_CTRL_H__

//#define bool _Bool

//#define FOREVER		0xFFFF


enum EnumResumeMode				// 设备运行出错类型定义
{
	ERROR_RETRY = 0x01,				// 重试动作
	ERROR_IGNORE = 0x02,			// 忽略当前错误
	ERROR_END = 0x04				// 无法处理，退出软件
};

enum enum_run_mode
{
    OP_RUN_MODE_NORMAL = 0,
    OP_RUN_MODE_STUB = 1,
};

typedef struct _stOpCtrl
{
	unsigned char bEn;				// 功能动作允许
	unsigned char nStep;			// 分支动作步骤

	unsigned int nDelay;			// 延时
	unsigned int nStartTime;		// 动作开始时间
	unsigned int nEndTime;			// 动作结束时间
	unsigned int nUseTime;			// 动作使用时间（可以用nEndTime-nStartTime来替代，节省内存）
	unsigned long nDelayStartTime;	// 延时开始时间
    unsigned int nParameter;        // 动态库下传流程参数（目前只支持两个字节）

	unsigned char bRunMode;			// 程序运行模式（0--连续运行， 1--StepByStep运行模式，可“打桩”暂停，便于观察）
	unsigned char bFlagPause;		// 程序暂停（几种情况可能：1.程序出错了，2.被“打桩”暂停）
    unsigned char bFlagStub;        // 程序已被打桩
	unsigned int nResult;			// 同mErrorCode（0x0000--工作中，0xFFFF--工作成功，0xFFFE--初始化，其他--错误）
	unsigned char nResumeMode;		// 错误处理模式（1--重试，2--忽略；3--无法处理，结束）
	unsigned char nAlarmMode;		// 由ERROR_RETRY、ERROR_INGORE以及ERROR_END三种自由组合（nButtonNum）
	unsigned char nPreStep;			// 跳转到当前出错case的Step
	unsigned char nLastStep;		// 寄存上一次PreStep值
	unsigned char nCmd;				// 当前流程编号
    unsigned char bFlagPopUp;		// 弹窗提示（只在出错误的时候弹出窗体）

	void * OpParent;				// 指向父流程（若无父流程调用，则指向本身）
//	void * OpChild;					// 指向子流程（若无子流程调用，则指向本身）
	void * OpChildError;			// 子流程出错（多个子动作并行时会用到）
} stOpCtrl;

void InitOpStruct(stOpCtrl *operation, unsigned char nCmdID, unsigned int nResult);			// 初始化流程
void SetOpTimeDelay(stOpCtrl*op,unsigned int delaytime);
unsigned char CheckOpTimeDelay(stOpCtrl *op);                                   // 检测是否超时，如果超时，返回真
unsigned char StartOp(stOpCtrl *operation);									    // 开始启动流程
unsigned char StartChildOp(stOpCtrl * Operation, stOpCtrl * OperationChild);    // 开始子流程
unsigned char StartOpByMode(stOpCtrl *operation, unsigned char nMode);	        // 开始启动流程，采用指定运行模式（0-直接运行；1-单步运行）
void OpPlugOneStub(stOpCtrl *Operation);							            // 动作流程中进行打一个桩，在StepByStep模式下会遇到桩会暂停
void OpUnplugOneStub(stOpCtrl *Operation);                                      // 动作流程中进行撤销一个桩，流程能往下走
void SetOpRunMode(stOpCtrl *Operation, unsigned char nMode);			        // 设置动作执行的模式（0-直接运行；1-单步运行）

unsigned char ResumeOp(stOpCtrl *operation, unsigned char nResumeMode);         // 流程报错、人工处理（重试/忽略/致命结束）完毕后，恢复流程

bool IsChildOpOK(stOpCtrl *operation, stOpCtrl *operationChild);
void SetOpErrorInfo(stOpCtrl *operation, unsigned int nErrorCode, unsigned char nErrorType);
void DealResult(stOpCtrl *Operation);
extern unsigned short int CheckResult(stOpCtrl *Operation);

#endif
