#include <string.h>
#include "stm32f10x.h"
#include "bsp.h"

/***************************************************************************************************************
  * 函数名称： SetOpTimeDelay
  * 函数描述： 设置延时时间
  * 其它说明：
****************************************************************************************************************/
void SetOpTimeDelay(stOpCtrl*op,unsigned int delaytime)
{
	op->nDelayStartTime=systemTime;
	op->nDelay=delaytime;
}

/***************************************************************************************************************
  * 函数名称： CheckOpTimeDelay
  * 函数描述： 检测延时时间是否到
  * 其它说明：
****************************************************************************************************************/
unsigned char CheckOpTimeDelay(stOpCtrl *op)
{
	unsigned long long tmpTime;
	tmpTime = systemTime;  //当前时间

	if(op->nDelayStartTime >tmpTime)	//开始时间大于当前时间,计时器溢出
		tmpTime |= 0x100000000;

	tmpTime = tmpTime - op->nDelayStartTime;   //当前时间减去开始时间

	if((unsigned int)tmpTime >= op->nDelay)   //当前时间减去开始时间,如果时间大于延时时间,延时时间到
		return true;
	else
		return false;
}

/***************************************************************************************************************
  * 函数名称： StartOp
  * 函数描述： 启动一个子动作
  * 其它说明：（1）旧形式调用
****************************************************************************************************************/
unsigned char StartOp(stOpCtrl * Operation)
{
	return StartOpByMode(Operation, OP_RUN_MODE_NORMAL);
}

/***************************************************************************************************************
  * 函数名称： StartOpByMode
  * 函数描述： 启动一个子动作，采用指定运行模式
  * 其它说明：  nMode  0-直接运行；1-单步运行
****************************************************************************************************************/
unsigned char StartOpByMode(stOpCtrl *Operation, unsigned char nMode)
{
// 2016-04-10：去掉动作使能的处理
	//if(Operation->bEn)			// 在运行，不接受新指令
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

	Operation->nStartTime = systemTime;	//关时钟中断获取准确时间

	return true;
}

/***************************************************************************************************************
  * 函数名称： StartChildOp
  * 函数描述： 启动一个子动作
  * 其它说明：（1）设计时间：2016-04-20；
			  （2）增加了与错误处理相关的处理
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

    OperationChild->nStartTime = systemTime;	//关时钟中断获取准确时间

    return true;
}


/***************************************************************************************************************
  * 函数名称： ResumeOp
  * 函数描述： 恢复流程
  * 其它说明：（1）设计时间：2016-04-20；
					 （1）用于在进行错误处理后的流程恢复
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

	while (OpTemp->OpParent != OpTemp)	// 说明有父动作
	{
		OpTemp = OpTemp->OpParent ;		// 找到父动作
		if (OpTemp->nResult != 0xFFFF)
			OpTemp->nResult = 0;
		OpTemp->bFlagPause = 0;			// 将父动作恢复
	}
	return true;
}


/***************************************************************************************************************
  * 函数名称： InitOpStruct
  * 函数描述： 初始化一个动作结构体
  * 其它说明：（1）对一个动作结构体的相关成员进行初始化处理
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

    Operation->nCmd = nCmdID;				//设置命令号
    Operation->nResult = nCmdResult;		//设置结果

    Operation->OpParent = Operation;		//设置父动作指针
    //	Operation->OpChild = Operation;			//设置子动作指针
    Operation->OpChildError = Operation;	//设置错误动作指针	
}

//返回0x00说明动作正在进行中
//0xFFFF表示动作成功完成
//其它值表示动作错误代码,结果一直保存,复位与重启会清除错误
unsigned short int CheckResult(stOpCtrl *Operation)
{
	if(Operation->bEn)
	{
		return 0x00;      // 动作进行中
	}
	else
	{
		return Operation->nResult;  //0xFFFF 表示成功完成
	}
}


//动作完成后，清动作使能,并计算动作使用时间
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

		if ((Operation->nEndTime)<(Operation->nStartTime))		//计时溢出
		{
			unsigned long tmpTime;

			tmpTime=Operation->nEndTime;
			tmpTime |= 0x10000;
			Operation->nUseTime=tmpTime-Operation->nStartTime;//计算本次动作所用时间
		}
		else
		{
			Operation->nUseTime=Operation->nEndTime-Operation->nStartTime;//计算本次动作所用时间
		}
	}
	else if(Operation->nResult > 0)
	{
		Operation->bFlagPause = 1;
	}
}


/***************************************************************************************************************
  * 函数名称： IsChildOpOK
  * 函数描述： 判断子动作执行结果
  * 其它说明：（1）若失败，将子动作相关的信息（错误结果，错误处理种类，错误动作指针）返回到父动作中
****************************************************************************************************************/
bool IsChildOpOK(stOpCtrl *Operation, stOpCtrl *OperationChild)
{
	if (OperationChild->nResult == 0xFFFF)
	{
		Operation->OpChildError = Operation;
//		Operation->OpChild = Operation;
		return true;
	}
	else if (OperationChild->nResult != 0x00)	// 如果子动作出错
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
  * 函数名称： SetOpErrorInfo
  * 函数描述： 设置相关错误信息
  * 其它说明：（1）nAlarmMode的组合类型由ERROR_RETRY（重试）、ERROR_INGORE（忽略）以及ERROR_END
									（结束）这三种自由组合	而成
****************************************************************************************************************/
void SetOpErrorInfo(stOpCtrl *Operation, unsigned int nErrorCode, unsigned char nAlarmMode)
{
	Operation->bFlagPopUp = true;
	Operation->nResult = nErrorCode;
	Operation->nAlarmMode = nAlarmMode;
}

/***************************************************************************************************************
  * 函数名称： OpSetOneStub
  * 函数描述： 动作流程中进行打一个桩，在StepByStep模式下会遇到桩会暂停
  * 其它说明：
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
  * 函数名称： OpUnplugOneStub
  * 函数描述： 动作流程中进行撤销一个桩，流程能往下走
  * 其它说明：
****************************************************************************************************************/
void OpUnplugOneStub(stOpCtrl *Operation)
{
    Operation->bFlagStub = false;
}

/***************************************************************************************************************
  * 函数名称： SetOpRunMode
  * 函数描述： 设置Op流程工作模式
  * 其它说明：  nMode 0--连续运行， 1--StepByStep运行模式，可“打桩”暂停，便于观察
****************************************************************************************************************/
void SetOpRunMode(stOpCtrl *Operation, unsigned char nMode)
{
    Operation->bRunMode = nMode;
}
