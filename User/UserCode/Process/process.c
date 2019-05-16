
#include "../Parameter/Parameter.h"
#include "../Command/command.h"
#include "../IOCfg.h"
#include "../../BSP/bsp.h"
#include "../../BSP/Command/bsp_command.h"
#include "process.h"
#include <string.h>
#include <stdio.h>


/***************定义进卡口为后*****************/
/******** 相关动作结构体变量定义 ******************************/
stOpCtrl OpResetModule;	            //复位
stOpCtrl OpResetChannal;   			//复位送卡通道
stOpCtrl OpSendCardToWarm; 			//发卡到热压工位
stOpCtrl OpSendCardToCool; 			//发卡到冷压工位
stOpCtrl OpChannalBackORG; 			//通道回到原点
stOpCtrl OpWarmDown; 			 	//加热下降
stOpCtrl OpWarmRise; 			 	//加热升起
stOpCtrl OpCoolDown; 			 	//冷却下降
stOpCtrl OpCoolRise; 			 	//冷却升起
stOpCtrl OpPresDown; 			 	//冲压下降
stOpCtrl OpPresRise; 			 	//冲压升起
stOpCtrl OpOpeningPressure;	 		//压力开
stOpCtrl OpClosingPressure; 		//压力关
stOpCtrl OpMakeCardStart;			//制卡测试
stOpCtrl OpWarmEnforceRise;			//热压强制上升
stOpCtrl OpWarmEnforceDown;			//热压强制下降
stOpCtrl OpCoolEnforceRise;			//冷压强制上升
stOpCtrl OpCoolEnforceDown;			//冷压强制下降
stOpCtrl OpCutCardPosition;			//到裁剪卡的位置
stOpCtrl OpCutCard;					//裁剪卡

stOpCtrl OpRightClampClose; 		//右边夹子闭合
stOpCtrl OpRightClampLoose; 		//右边夹子松开
stOpCtrl OpLeftClampClose; 			//左边夹子闭合
stOpCtrl OpLeftClampLoose; 			//左边夹子松开
stOpCtrl OpClampClose; 				//夹子闭合
stOpCtrl OpClampLoose; 				//夹子松开
stOpCtrl OpClampReset;              //夹子复位
stOpCtrl OpShiftTrolley;			//小车移动
stOpCtrl OpReverseFront;			//翻转到正面
/******** 其它定义*********************************/

stOpCtrl *gMachineOperation[50]; // 设置一个支持50个流程的指针数组
unsigned char gnOperationNo = 1; // 执行的动作序号（从1开始对各个动作结构体变量逐个初始化）
extern unsigned char cParamBuf[];

//unsigned char gFlagStopSimulation = false; // 模拟跑机测试执行状态标记
unsigned char gStartBottonPressed = false; // 开始键按键状态标记（0-未按下按键；1-按下按键）
unsigned char gPauseBottonPressed = false; // 暂停键按键状态标记（0-未按下按键；1-按下按键）

struct sTrolleyPara
{
	int iaAbsPosLiftCar[5];  //小车移动各个工位的绝对步数初始值	
	unsigned int uiSpeed;     
}g_sTrolleyPara =
{
	{0,0,0,0,0},
	100,
};


unsigned char nLocation;	//存储夹卡片的夹子位置
/****************************************************
Function Name: InitializeMachine
*****************************************************
Descriptions:  
 对设备进行初始化操作
 在启动设备其他动作之前要先进行初始化操作

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

	gMachineOperation[gnOperationNo++] = &OpResetModule;					 //1 复位
	gMachineOperation[gnOperationNo++] = &OpResetChannal; 			 		 //2 复位送卡通道
	gMachineOperation[gnOperationNo++] = &OpSendCardToWarm;			 		 //3 发卡到热压工位
	gMachineOperation[gnOperationNo++] = &OpSendCardToCool;					 //4 发卡到冷压工位
	gMachineOperation[gnOperationNo++] = &OpChannalBackORG;			 		 //5 通道回到原点
	gMachineOperation[gnOperationNo++] = &OpWarmDown;						 //6 加热下降
	gMachineOperation[gnOperationNo++] = &OpWarmRise;						 //7 加热升起
	gMachineOperation[gnOperationNo++] = &OpCoolDown;						 //8 冷却下降
	gMachineOperation[gnOperationNo++] = &OpCoolRise;						 //9 冷却升起
	gMachineOperation[gnOperationNo++] = &OpPresDown;						 //10 冲压下降
	gMachineOperation[gnOperationNo++] = &OpPresRise;						 //11 冲压升起
	gMachineOperation[gnOperationNo++] = &OpOpeningPressure;			 	 //12 总源开
	gMachineOperation[gnOperationNo++] = &OpClosingPressure;			 	 //13 总源关
	gMachineOperation[gnOperationNo++] = &OpMakeCardStart;					 //14 制卡测试
	gMachineOperation[gnOperationNo++] = &OpWarmEnforceRise;				 //15 热压无条件上升
	gMachineOperation[gnOperationNo++] = &OpWarmEnforceDown;				 //16 热压无条件下降
	gMachineOperation[gnOperationNo++] = &OpCoolEnforceRise;				 //17 冷压无条件上升
	gMachineOperation[gnOperationNo++] = &OpCoolEnforceDown;				 //18 冷压无条件下降
	gMachineOperation[gnOperationNo++] = &OpCutCardPosition;		 		 //19 到裁剪卡的位置
	gMachineOperation[gnOperationNo++] = &OpCutCard;						 //20 裁剪卡
	gMachineOperation[gnOperationNo++] = &OpRightClampClose; 		         //21 右边夹子闭合
	gMachineOperation[gnOperationNo++] = &OpRightClampLoose; 				 //22 右边夹子松开
	gMachineOperation[gnOperationNo++] = &OpLeftClampClose; 				 //23 左边夹子闭合
	gMachineOperation[gnOperationNo++] = &OpLeftClampLoose; 				 //24 左边夹子松开
	gMachineOperation[gnOperationNo++] = &OpClampClose; 					 //25 夹子闭合
	gMachineOperation[gnOperationNo++] = &OpClampLoose; 					 //26 夹子松开
	gMachineOperation[gnOperationNo++] = &OpClampReset; 					 //27 夹子复位
	gMachineOperation[gnOperationNo++] = &OpShiftTrolley;					 //28 小车移动 
	gMachineOperation[gnOperationNo++] = &OpReverseFront;					 //29 翻转到正面


	for (i = 1; i < gnOperationNo; i++)
	{
		InitOpStruct(gMachineOperation[i], i, 0xFFFE);
	}
	/**************步进电机复位速度定义*******************/
	StepMotorCtr[SM_CHANNEL].nResetSpeed = 5; // 通道电机复位速度
}
/****************************************************
Function Name: ResetAllSiganl
*****************************************************
Descriptions:  
 复位所有标志
 复位所有结构体,必然会复位复位本身这个结构体,
 所以得重新启动才行,其它动作不用

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
		// 初始化所有动作结构体；不对复位当前模块动作函数处理
		InitOpStruct(gMachineOperation[i], i, 0xFFFF);
	}
	for (i = 1; i <= 12; i++)
	{
		// 关闭所有直流输出
		dm_ctrl_one(i, 0);
	}
	for (i = 1; i <= 8; i++)
	{
		// 关闭所有步进输出
		sm_stop(i);
		sm_set_enable(i, SM_ENABLE);
		StepMotorCtr[i].bRunNoStop = FALSE;
	}
}

/****************************************************************************************
  * 函数名称： Op_ResetModule
  * 函数描述： 复位所有设备
  * 其它说明：
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
Descriptions:  复位通道电机
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
			if ((nLocation == ENUM_INSIDE_WARE) || (nLocation == ENUM_INSIDE_COOL)) //在热压或冷压工位
			{
				StartChildOp(CurrentOp, &OpChannalBackORG); //返回原点
				CurrentOp->nStep = STEP7;
			}
			else
			{
				if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //不在热压工位
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
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //到达热压工位
			{
				sm_stop(SM_CHANNEL);  //要先停止然后再运行否者运行不正常
				sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED], parameter[WARM_STEP]); //向前再走一点(补偿)
				CurrentOp->nStep = STEP3;
			}
			else
			{
				if (IsSmRunFinish(SM_CHANNEL)) //电机动作完成
				{
					sm_stop(SM_CHANNEL);					  //停止电机
					SetOpErrorInfo(CurrentOp, 20001, ERROR_END); //通道到达热压传感器失败
				}
			}
		break;
		case STEP3:
			if (IsSmRunFinish(SM_CHANNEL)) //电机动作完成
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
			sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], parameter[BACK_STEP]); //继续往回走一段距离
			CurrentOp->nStep = STEP6;
			break;
		case STEP6:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				nLocation = ENUM_OUTSIDE; //加卡夹子在外面
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
Descriptions:  发卡到热压工位
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
				SetOpErrorInfo(CurrentOp, 20002, ERROR_RETRY | ERROR_END); //通道返回热压失败
			}
		break;
		case STEP2:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //到达热压工位
			{
				sm_stop(SM_CHANNEL);  //要先停止然后再运行否者运行不正常
				sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED], parameter[WARM_STEP]); //向前再走一点(补偿)
				CurrentOp->nStep = STEP3;
			}
			else
			{
				if (IsSmRunFinish(SM_CHANNEL)) //电机动作完成
				{
					sm_stop(SM_CHANNEL);											//停止电机
					SetOpErrorInfo(CurrentOp, 20003, ERROR_END); //通道到达热压传感器失败
				}
			}
		break;
		case STEP3:
			if (IsSmRunFinish(SM_CHANNEL)) //电机动作完成
			{
				nLocation = ENUM_INSIDE_WARE; //加卡夹子在里面
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
Descriptions:  发卡到冷压工位
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
			if (nLocation == ENUM_OUTSIDE) //在外面原点
			{
				StartChildOp(CurrentOp, &OpSendCardToWarm); //先运行到热压工位
				CurrentOp->nStep = STEP2;
			}
			else if (nLocation == ENUM_INSIDE_WARE) //已经在热压工位
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, 20004, ERROR_RETRY | ERROR_END); //不在外面原点,运行的话报错.
			}
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp, &OpSendCardToWarm)) //在热压工位的基础上再向冷压工位运行
			{
				sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED],(2000 + parameter[COOL_STEP]));
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				nLocation = ENUM_INSIDE_COOL; //加卡夹子在里面
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
Descriptions:  返回原点
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
			if ((nLocation == ENUM_INSIDE_WARE) || (nLocation == ENUM_INSIDE_COOL)) //如果在热压或者冷压工位才可以向后运行
			{
				sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], 8000); //往回走
				CurrentOp->nStep = STEP4;
			}
			else
			{
				SetOpErrorInfo(CurrentOp, 20005, ERROR_RETRY | ERROR_END); //不在运行报错
			}
		break;
		case STEP4:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //向后运行到热压工位
			{
				sm_stop(SM_CHANNEL);  //要先停止然后再运行否者运行不正常
				sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], parameter[BACK_STEP]); //继续往回走一段距离
				CurrentOp->nStep = STEP5;
			}
			else
			{
				if (IsSmRunFinish(SM_CHANNEL)) //动作完成为运行到热压工位报错
				{
					sm_set_enable(SM_CHANNEL, SM_DISABLE);
					SetOpErrorInfo(CurrentOp, 20006, ERROR_RETRY | ERROR_END); //通道返回热压失败
				}
			}
		break;
		case STEP5:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				nLocation = ENUM_OUTSIDE; //加卡夹子在外面
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
Descriptions:  加热下降
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
					SetOpErrorInfo(CurrentOp, ERROR_WARMING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //加热部分传感器异常
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
					SetOpErrorInfo(CurrentOp, ERROR_WARMING_DOWN_FAIL, ERROR_RETRY | ERROR_END); //加热下降失败
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
Descriptions:  加热升起
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
				SetOpErrorInfo(CurrentOp, ERROR_WARMING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //加热部分传感器异常
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
				SetOpErrorInfo(CurrentOp, ERROR_WARMING_DOWN_FAIL, ERROR_RETRY | ERROR_END); //加热部分传感器异常
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
Descriptions:  冷却下降
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
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //冷却位置传感器异常
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
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_DOWN_FAIL, ERROR_RETRY | ERROR_END); //冷却下降失败
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
Descriptions:  加热升起
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
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //冷却位置传感器异常
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
				SetOpErrorInfo(CurrentOp, ERROR_COOLING_RISE_FAIL, ERROR_RETRY | ERROR_END); //冷却升起失败
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
Descriptions:  冲压下降
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
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //冲压位置传感器异常
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
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_DOWN_FAIL, ERROR_RETRY | ERROR_END); //冲压下降失败
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
Descriptions:  冲压升起
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
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_UPDOWN_ABNOMAL, ERROR_RETRY | ERROR_END); //冲压位置传感器异常
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
				SetOpErrorInfo(CurrentOp, ERROR_PRESS_RISE_FAIL, ERROR_RETRY | ERROR_END); //冲压升起失败
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
Descriptions:  压力开
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
Descriptions:  压力关
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
Descriptions:  制卡测试
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
			StartChildOp(CurrentOp, &OpSendCardToWarm); //调用通道前进子程序
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp, &OpSendCardToWarm)) //成功进行下一步,不成功,子程序报错.
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpWarmDown); //调用热压下压子程序
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp, &OpWarmDown)) //判断子程序调用是否成功
			{
				SetOpTimeDelay(CurrentOp, parameter[WARM_TIME] * 500); //根据传下来的参数延时压下加热时间
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if (CheckOpTimeDelay(CurrentOp)) //判断延时时间是否到
			{
				CurrentOp->nStep = STEP6;
				SetOpTimeDelay(CurrentOp, parameter[WARM_TIME] * 500); //根据传下来的参数延时压下加热时间
			}

		break;
		case STEP6:
			if (CheckOpTimeDelay(CurrentOp)) //判断延时时间是否到
			{
				StartChildOp(CurrentOp, &OpWarmRise); //调用升起热压模块
				CurrentOp->nStep = STEP7;
			}
		break;
	case STEP7:
		if (IsChildOpOK(CurrentOp, &OpWarmRise))
		{
			CurrentOp->nStep = STEP8;
			SetOpTimeDelay(CurrentOp, 500); //延时500ms时间,有的时候会粘卡
		}
		break;
		case STEP8:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP9;
			}
		break;
		case STEP9:
			StartChildOp(CurrentOp, &OpSendCardToCool); //把卡移动到冷压模块
			CurrentOp->nStep = STEP10;
		break;
		case STEP10:
			if (IsChildOpOK(CurrentOp, &OpSendCardToCool)) //是否成功
			{
				CurrentOp->nStep = STEP11;
			}
		break;
		case STEP11:
			StartChildOp(CurrentOp, &OpCoolDown); //冷压模块下压
			CurrentOp->nStep = STEP12;
		break;
		case STEP12:
			if (IsChildOpOK(CurrentOp, &OpCoolDown)) //是否执行成功
			{
				SetOpTimeDelay(CurrentOp, parameter[COOL_TIME] * 1000); //根据传下来的参数延时冷压时间
				CurrentOp->nStep = STEP13;
			}
		break;
		case STEP13:
			if (CheckOpTimeDelay(CurrentOp)) //到达时间
			{
				StartChildOp(CurrentOp, &OpCoolRise); //冷压模块升起
				CurrentOp->nStep = STEP14;
			}
		break;
		case STEP14:
			if (IsChildOpOK(CurrentOp, &OpCoolRise)) //执行成功
			{
				StartChildOp(CurrentOp, &OpChannalBackORG); //通道返回原点
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_WarmEnforceRise
*****************************************************
Descriptions:  强制热压上升
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
			dm_ctrl_one(DM_WARM_CONTROL, RELAY5_OFF); //上升
			SetOpTimeDelay(CurrentOp, 2000);						//延时
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
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_WarmEnforceDown
*****************************************************
Descriptions:  强制热压下降
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
			dm_ctrl_one(DM_WARM_CONTROL, RELAY5_ON); //下降
			SetOpTimeDelay(CurrentOp, 2000);					 //延时
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
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_OpCoolEnforceRise
*****************************************************
Descriptions:  强制冷压上升
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
				dm_ctrl_one(DM_COOL_CONTROL, RELAY6_OFF); //上升
				SetOpTimeDelay(CurrentOp, 2000);			  //延时
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
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_CoolEnforceDown
*****************************************************
Descriptions:  强制冷压下降
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
			dm_ctrl_one(DM_COOL_CONTROL, RELAY6_ON); //下降
			SetOpTimeDelay(CurrentOp, 2000);		//延时
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
	DealResult(CurrentOp); //结束程序清理使能
}

/****************************************************
Function Name: Op_CutCardPosition
*****************************************************
Descriptions: 到剪卡位
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
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //不在热压工位
			{
				sm_run(SM_CHANNEL, SM_CHANNEL_BACKWARD, parameter[SM_CHANNAL_SPEED], 8000); //走到热压工位
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if (input_get_one(SN_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //在热压工位
			{
				sm_stop(SM_CHANNEL);//停止通道电机
				SetOpTimeDelay(CurrentOp, 100); //延时
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:																																														//走到裁剪工位
			sm_run(SM_CHANNEL, SM_CHANNEL_FORWARDD, parameter[SM_CHANNAL_SPEED], parameter[CUT_CARD_POSITION]); //走到热压工位
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
	DealResult(CurrentOp); //结束程序清理使能
}

/****************************************************
Function Name: Op_CutCard
*****************************************************
Descriptions: 剪卡
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
			StartChildOp(CurrentOp, &OpCutCardPosition); //运行到发卡位置
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp, &OpCutCardPosition)) //动作是否成功
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpPresDown); //下压
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp, &OpPresDown)) //动作是否成功
			{
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			StartChildOp(CurrentOp, &OpPresRise); //升起
			CurrentOp->nStep = STEP6;
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp, &OpPresRise)) //动作是否成功
			{
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			StartChildOp(CurrentOp, &OpCutCardOriginBack); //返回剪卡之前的位置
			CurrentOp->nStep = STEP8;
		break;
		case STEP8:
			if (IsChildOpOK(CurrentOp, &OpCutCardOriginBack)) //动作是否完成
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
	DealResult(CurrentOp); //结束程序清理使能
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
			else //Other status,比如两个传感器都检测到或都检测不到
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
			else if (CheckOpTimeDelay(CurrentOp)) //延时时间到,夹子仍未关闭
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
	DealResult(CurrentOp); //结束程序清理使能
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
				else if(CheckOpTimeDelay(CurrentOp)) 			//延时时间到了,夹子仍未打开
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
	DealResult(CurrentOp); //结束程序清理使能
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
			else //Other status,比如两个传感器都检测到或都检测不到
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
			else if (CheckOpTimeDelay(CurrentOp)) //延时时间到,夹子仍未关闭
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
	DealResult(CurrentOp); //结束程序清理使能
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
			else if(CheckOpTimeDelay(CurrentOp)) 			//延时时间到了,夹子仍未打开
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
	DealResult(CurrentOp); //结束程序清理使能
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
				StartChildOp(CurrentOp,&OpRightClampClose);  //调用右边夹子关闭子程序
				CurrentOp->nStep = STEP2;
			break;
			case STEP2:
				if (IsChildOpOK(CurrentOp,&OpRightClampClose))  //调用是否成功
				{
						CurrentOp->nStep = STEP3;
				}
			break;
			case STEP3:
				StartChildOp(CurrentOp,&OpLeftClampClose); //调用左边夹子子程序
				CurrentOp->nStep = STEP4;
			break;
			case STEP4:
				if (IsChildOpOK(CurrentOp,&OpLeftClampClose)) //调用是否成功
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
		DealResult(CurrentOp); //清除动作使能
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
			StartChildOp(CurrentOp,&OpRightClampLoose);  //调用右边夹子关闭子程序
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpRightClampLoose))  //调用是否成功
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp,&OpLeftClampLoose); //调用左边夹子子程序
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpLeftClampLoose)) //调用是否成功
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
	DealResult(CurrentOp); //清除动作使能
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
			if (iStepNum >= 0) //步数为正  
			{
				sm_run(SM_TROLLEY,DIR_TROLLEY_FORWARD,g_sTrolleyPara.uiSpeed,iStepNum); //向前走N歩
			}
			else  //步数为负
			{
				sm_run(SM_TROLLEY,DIR_TROLLEY_BACKWARD,g_sTrolleyPara.uiSpeed,iStepNum); //向后走N歩
			}
			CurrentOp->nStep = STEP2;	
		break;
		case STEP2:
			if (IsSmRunFinish(SM_TROLLEY)) //电机是否走完
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else
			{
				SetOpErrorInfo(CurrentOp,ERROR_TROLLEY_STEP_MOTOR_FAULT,ERROR_END);//10022 电梯小车故障   
			}	
		break;
		case STEP_SUCCESS:
			CurrentOp->nResult = 0xffff; //返回成功码
		break; 
		default:
		break;
	}
	DealResult(CurrentOp); //清动作使能
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
			  ||(input_get_one(SN_TROLLEY_UP) == SENSOR_TYPE_BEAM_OFF))//已经在下降位
			{
				CurrentOp->nStep = STEP_SUCCESS;		
			}
			else if ((input_get_one(SN_TROLLEY_DOWN) == SENSOR_TYPE_BEAM_OFF) 
			  ||(input_get_one(SN_TROLLEY_UP) == SENSOR_TYPE_BEAM_ON))//已经在升起位
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
Descriptions:  控制设备所有动作的执行
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
	Op_ResetModule();						 //1 复位
	Op_ResetChannal(); 					   	 //2 复位送卡通道
	Op_SendCardToWarm();				   	 //3 发卡到热压工位
	Op_SendCardToCool();				   	 //4 发卡到冷压工位
	Op_ChannalBackORG();    			     //5 通道回到原点
	Op_WarmRise();						   	 //6 加热下降
	Op_WarmDown();						   	 //7 加热升起
	Op_CoolRise();						   	 //8 冷却下降
	Op_CoolDown();						   	 //9 冷却升起
	Op_PresRise();						   	 //10 冲压下降
	Op_PresDown();						   	 //11 冲压升起
	Op_OpeningPressure();				   	 //12 压力开
	Op_ClosingPressure();				   	 //13 压力关						
	Op_MakeCardStart();					   	 //14 制卡测试
	Op_WarmEnforceRise();				     //15 热压强制上升
	Op_WarmEnforceDown();				     //16 热压强制下降
	Op_OpCoolEnforceRise();				     //17 冷压强制上升
	Op_CoolEnforceDown();				     //18 冷压强制下降
	Op_CutCardPosition();				     //19 到裁剪卡的位置
	Op_CutCard();						     //20 剪卡
	Op_RightClampClose();			     	 //21 右侧夹子关闭 			
  	Op_RightClampLoose();				     //22 右侧夹子打开          
	Op_LeftClampClose();					 //23 左侧夹子关闭
	Op_LeftClampLoose();					 //24 左侧夹子打开
	Op_ClampClose();						 //25 两边夹子关闭
	Op_ClampLoose();						 //26 两侧夹子打开

}






