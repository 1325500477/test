#include "../Parameter/Parameter.h"
#include "../Command/command.h"
#include "../IOCfg.h"
#include "../../BSP/bsp.h"
#include "../../BSP/Command/bsp_command.h"
#include "process.h"
#include <string.h>
#include <stdio.h>

/******** 相关动作结构体变量定义 ******************************/
stOpCtrl OpResetModule;	            					//复位
stOpCtrl OpResetClampCardCar;   						//复位送卡通道
stOpCtrl OpWarmDown; 			 						//加热下降
stOpCtrl OpWarmRise; 			 		  				//加热升起
stOpCtrl OpCoolDown; 				  					//冷却下降
stOpCtrl OpCoolRise; 			 	  					//冷却升起
stOpCtrl OpPresDown; 				  					//冲压下降
stOpCtrl OpPresRise; 			 	  					//冲压升起
stOpCtrl OpOpeningPressure;	 							//压力开
stOpCtrl OpClosingPressure; 							//压力关
stOpCtrl OpRightClampClose; 							//右边夹子闭合
stOpCtrl OpRightClampOpen; 								//右边夹子松开
stOpCtrl OpLeftClampClose; 								//左边夹子闭合
stOpCtrl OpLeftClampOpen; 								//左边夹子松开
stOpCtrl OpClampClose; 									//夹子闭合
stOpCtrl OpClampCardCarMove; 							//抓卡小车移动位置	
stOpCtrl OpPretargetingReset;            				//预定位复位
stOpCtrl OpPretargetingUp;                				//预定位上	
//hww
stOpCtrl  OpResetClamp;									//爪子复位
stOpCtrl  OpClampOpen;									//爪子张开
stOpCtrl  OpClampClose;									//爪子闭合
stOpCtrl OpWasteStorageReset;  							//废料场电机复位
stOpCtrl OpWasteStorageDown;	 						//废料场电机下降
stOpCtrl OpPretargetingVacuumCupOpen;					//预定位真空吸打开
stOpCtrl OpPretargetingVacuumCupClose;					//预定位真空吸关闭
stOpCtrl OpWasteStorageVacuumCupOpen;					//废料场真空吸打开
stOpCtrl OpWasteStorageVacuumCupClose;					//废料场真空吸关闭
stOpCtrl OpOpenGasSwitch;			 				    //气路开启
stOpCtrl OpCloseGasSwitch;								//气路关闭
stOpCtrl OpMakeCard;									//制卡
stOpCtrl OpGotoLoadingPlatform;			   				//去预定位平台
stOpCtrl OpCuttingPlatformWork;		 					//裁剪平台工作
stOpCtrl OpScramProcess;								//紧急停止剪卡和制卡流程
//吸卡小车
stOpCtrl OpSuckCardCarMove;								//吸卡小车移动位置
stOpCtrl OpResetSuckCardCar;							//吸卡小车复位
stOpCtrl OpSuckReset;									//吸盘复位
//吸盘
stOpCtrl OpSuckMove;									//吸盘下降
//白卡箱
stOpCtrl OpBlankCardBoxReset;							//白卡箱复位
stOpCtrl OpBlankCardBoxUp;								//白卡箱升起




unsigned char nStatusClipOpen 	  = STATUS_UNKNOW;		//机械手开合状态
unsigned char nStatusPretarget 	  = STATUS_UNKNOW;		//预定位位置状态
unsigned char nStatusWastestorage = STATUS_UNKNOW;  	//废料位置状态
unsigned char nStatusSuck		  = STATUS_UNKNOW;  	//吸盘的位置状态
unsigned char nStatusBlankCardBox = STATUS_UNKNOW;      //空白卡箱位置状态

unsigned char bFlagReset = FALSE;        				//复位标志位
/******** 其它定义*********************************/

stOpCtrl *gMachineOperation[50]; // 设置一个支持50个流程的指针数组
unsigned char gnOperationNo = 1; // 执行的动作序号（从1开始对各个动作结构体变量逐个初始化）
extern unsigned char cParamBuf[];//接收参数
unsigned char gStartBottonPressed  	 = false;  		//开始键按键状态标记（0-未按下按键；1-按下按键）
unsigned char gPauseBottonPressed  	 = false;   	//暂停键按键状态标记（0-未按下按键；1-按下按键）
//抓卡小车
unsigned char gTrolleyCarMove_Flag 	 = false;  		//通道小车运行标志
unsigned char gTrolleyCarPosition  	 = ENUM_UNKNOW; //通道小车的位置
unsigned char gTrolleyCarMoveCount 	 = 0;           //小车移动次数,达到一定次数复位消除累积误差
//吸卡小车
unsigned char gSuckCardCarMove_Flag  = false;       //吸卡小车运行标志
unsigned char gSuckCardCarPosition   = ENUM_UNKNOW; //吸卡小车位置
unsigned char gSuckCardCarMoveCount  = 0;           //吸卡小车移动次数,达到一定次数复位消除累积误差
//吸盘
unsigned char gSuckMove_Flag  		 = false;       //吸盘运行标志
unsigned char gSuckPosition   		 = ENUM_UNKNOW; //吸盘位置
unsigned char gSuckMoveCount  		 = 0;           //吸盘移动次数,达到一定次数复位消除累积误差

unsigned int iAbsPosTrolleyCarMov[7] = { 			//小车移动各个工位的绝对步数初始值
   0, 	//空余
   140, //热压位 
   1830,//冷压位
   3500,//撕膜位
   4800,//预定位
   6200,//剪卡位
   0 	//空余
 };		
unsigned int iAbsSuckCardCarMov[8] = { //吸卡小车移动各个工位的绝对步数初始值
	0,	//空余
	100,//辅料位
	110,//预定位
	120,//临时放卡位
	130,//新辅料位
	140,//打印位
	150,//白卡位
	0 	//空余
}; 
unsigned int iAbsSuckMove[8] = { //吸盘电机在各个工位下降的绝对步数初始值
	0,	//空余
	100,//辅料位
	110,//预定位
	120,//临时放卡位
	130,//新辅料位
	140,//打印位
	150,//白卡位
	0 	//空余
}; 
void ProcessUpdateParameter(void)
{	
	unsigned char i;
	for (i = 0; i < 6; i++)
	{
		iAbsPosTrolleyCarMov[i+1] = parameter[i];
//1 到热压工位//2 到冷压工位//3 到废料工位//4 到预定位//5 裁剪卡位置
		iAbsSuckCardCarMov[i+1]   = parameter[i+11];
//12 吸卡小车辅料位//13 吸卡小车预定位//14 吸卡小车暂放卡位//15 吸卡小车新辅料位
//16 吸卡小车打印位//17 吸卡小车空白卡位		
		iAbsSuckMove[i+1]  		  = parameter[i+17];
//18 吸盘辅料位 //19 吸盘预定位//20 吸盘暂放卡位//21 吸盘新辅料位//22 吸盘打印位//23 吸盘空白卡位
	}
	
	// iAbsPosTrolleyCarMov[ENUM_INSIDE_WARE] 	       = parameter[PAR_WARM_STEP];		   //1 到热压工位
	// iAbsPosTrolleyCarMov[ENUM_INSIDE_COOL]         = parameter[PAR_COOL_STEP];		   //2 到冷压工位
	// iAbsPosTrolleyCarMov[ENUM_INSIDE_WASTE]    = parameter[PAR_WASTE];		   		   //3 到撕膜工位
	// iAbsPosTrolleyCarMov[ENUM_INSIDE_PRETARGETING] = parameter[PAR_PRETARGETING];	   //4 到预定位
	// iAbsPosTrolleyCarMov[ENUM_INSIDE_CUT_CARD] 	   = parameter[PAR_CUT_CARD_POSITION]; //5 到剪卡位
}

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

	gMachineOperation[gnOperationNo++] = &OpResetModule;				//1 复位
	gMachineOperation[gnOperationNo++] = &OpResetClampCardCar; 			//2 复位夹卡小车
	gMachineOperation[gnOperationNo++] = &OpClampCardCarMove;   		//3 夹卡小车移动位置
	gMachineOperation[gnOperationNo++] = &OpResetClamp;					//4 复位爪子
	gMachineOperation[gnOperationNo++] = &OpClampOpen; 					//5 爪子张开
	gMachineOperation[gnOperationNo++] = &OpClampClose; 				//6 爪子闭合
	gMachineOperation[gnOperationNo++] = &OpRightClampClose; 			//7 右侧爪子闭合
	gMachineOperation[gnOperationNo++] = &OpRightClampOpen; 			//8 右侧爪子张开
	gMachineOperation[gnOperationNo++] = &OpLeftClampClose; 			//9 左侧爪子闭合
	gMachineOperation[gnOperationNo++] = &OpLeftClampOpen; 				//10 左侧爪子张开
	gMachineOperation[gnOperationNo++] = &OpOpeningPressure;			//11 总源开
	gMachineOperation[gnOperationNo++] = &OpClosingPressure;			//12 总源关
	gMachineOperation[gnOperationNo++] = &OpWarmRise;				 	//13 热压上升
	gMachineOperation[gnOperationNo++] = &OpWarmDown;				 	//14 热压下降
	gMachineOperation[gnOperationNo++] = &OpCoolDown;					//15 冷却下降
	gMachineOperation[gnOperationNo++] = &OpCoolRise;					//16 冷却升起
	gMachineOperation[gnOperationNo++] = &OpPresDown;					//17 冲压下降
	gMachineOperation[gnOperationNo++] = &OpPresRise;					//18 冲压升起
	gMachineOperation[gnOperationNo++] = &OpPretargetingReset;          //19 预定位复位
	gMachineOperation[gnOperationNo++] = &OpPretargetingUp;             //20 预定位上升
	gMachineOperation[gnOperationNo++] = &OpWasteStorageReset;			//21 废料场电机复位
	gMachineOperation[gnOperationNo++] = &OpWasteStorageDown;	  		//22 废料场电机下降
	gMachineOperation[gnOperationNo++] = &OpPretargetingVacuumCupOpen;	//23 预定位真空打开
 	gMachineOperation[gnOperationNo++] = &OpPretargetingVacuumCupClose;	//24 预定位真空关闭
	gMachineOperation[gnOperationNo++] = &OpWasteStorageVacuumCupOpen;	//25 废料真空打开
	gMachineOperation[gnOperationNo++] = &OpWasteStorageVacuumCupClose;	//26 废料真空关闭
	gMachineOperation[gnOperationNo++] = &OpOpenGasSwitch;				//27 总气打开	
	gMachineOperation[gnOperationNo++] = &OpCloseGasSwitch;				//28 总气关闭
	//组合动作
	gMachineOperation[gnOperationNo++] = &OpMakeCard;					//29 制卡	
	gMachineOperation[gnOperationNo++] = &OpGotoLoadingPlatform;		//30 到预定位+预定位上升+打开夹子
	gMachineOperation[gnOperationNo++] = &OpCuttingPlatformWork;		//31 剪卡+废料				 
	gMachineOperation[gnOperationNo++] = &OpScramProcess;				//32 紧急停止剪卡和制卡流程
	//吸卡小车
	gMachineOperation[gnOperationNo++] = &OpSuckCardCarMove;            //33 吸卡小车移动位置
	gMachineOperation[gnOperationNo++] = &OpResetSuckCardCar;			//34 吸卡小车复位
	gMachineOperation[gnOperationNo++] = &OpSuckReset;					//35 吸盘复位
	gMachineOperation[gnOperationNo++] = &OpSuckMove;					//36 吸盘下降
	//白卡箱
	gMachineOperation[gnOperationNo++] = &OpBlankCardBoxReset;			//37 白卡箱复位
	gMachineOperation[gnOperationNo++] = &OpBlankCardBoxUp;				//38 白卡箱上升	



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

/****************************************************
Function Name: Op_ResetClampCardCar
*****************************************************
Descriptions:  复位小车
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
void Op_ResetClampCardCar(void)
{
	stOpCtrl *CurrentOp = &OpResetClampCardCar;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			gTrolleyCarPosition = ENUM_UNKNOW;
			CurrentOp->nStep = STEP20;
		break;
		case STEP20:
			StartChildOp(CurrentOp, &OpPretargetingReset); //预定位复位
			CurrentOp->nStep = STEP21;
	   	break;
		case STEP21:
		   	if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset);//废料场复位
				CurrentOp->nStep = STEP22;
			}
	   	break;
		case STEP22:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				StartChildOp(CurrentOp, &OpPresRise);  	//裁剪工位复位
				CurrentOp->nStep = STEP23;
			}
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);  	//冷压工位复位
				CurrentOp->nStep = STEP24;
			}
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//热压工位复位
				CurrentOp->nStep = STEP25;
			}
		break;
		case STEP25:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) 	//不在原点	
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 80, 50000); //向后运行一段距离
				CurrentOp->nStep = STEP5;
			}
			else if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //在原点
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_FORWARD, 80, 1000); //向前运行一段距离
				CurrentOp->nStep = STEP2;
			}			
		break;
		case STEP2:
			if(IsSmRunFinish(SM_CHANNEL) == true)  
			{
				if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //离开原点
				{
					SetOpTimeDelay(CurrentOp,300);   //反转延时
					CurrentOp->nStep = STEP3;	
				}
				else if(IsSmRunFinish(SM_CHANNEL))        	
				{
					CurrentOp->nResult = ERROR_CHANNAL_LEAVE_ORG_FAIL;   //离开原点传感器失败
				}	
			}
		break;																																																	
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{
				CurrentOp->nStep = STEP1;
			}	
		break;
		case STEP5:
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //到原点
			{
				sm_stop(SM_CHANNEL); 			//停止电机
				CurrentOp->nStep = STEP6;
			}	
			else if (IsSmRunFinish(SM_CHANNEL) == true) //电机动作完成
			{
				CurrentOp->nResult = ERROR_CHANNAL_RETURN_ORG_FAIL;		  //10002 通道返回传感器失败			
			}		
		break;
		case STEP6:
			sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 10, 5); 		//继续往回走一段距离
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if (IsSmRunFinish(SM_CHANNEL) == true)
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			sm_set_abs_zero(SM_CHANNEL); //清零步进电机的步数
			gTrolleyCarPosition = ENUM_INSIDE_ORIGIN; //在原点
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
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_ON)	 //在下面传感器处
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_WARM_CONTROL, RELAY_ON); //下降
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_WARMING_DOWN_FAIL; //10003加热下降失败
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
			if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)	 //在上面传感器处
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_WARM_CONTROL, RELAY_OFF); //上升
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_WARMING_RISE_FAIL;	 //10004加热升起失败
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
			if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_ON)	 //在下面传感器处
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_COOL_CONTROL, RELAY_ON); //下降
				SetOpTimeDelay(CurrentOp,3000); 
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_COOLING_DOWN_FAIL;	//10005冷却下降失败
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
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON)  //在上面传感器处
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_COOL_CONTROL, RELAY_OFF); //上升
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_COOLING_RISE_FAIL;  //10006冷却升起失败
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
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON)   //在下面传感器处
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_PRES_CONTROL, RELAY_ON);		//下降
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_PRESS_DOWN_FAIL;	 //10007冲压下降失败
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
			if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON)	 //在上面传感器处
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_PRES_CONTROL, RELAY_OFF);	 //上升
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_PRESS_RISE_FAIL;	  		//10008 冲压升起失败
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
Descriptions:  油压开
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
			dm_ctrl_one(DM_ALL_CONTROL, RELAY_ON);	//油压开
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
			dm_ctrl_one(DM_ALL_CONTROL, RELAY_OFF);  //油压关
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
Function Name: Op_RightClampClose
*****************************************************
Descriptions: 右边夹子关闭
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
			if (input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON) //已经在关闭位
			{
				CurrentOp->nResult = 0xffff;
			} 
			else //其它
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_CW);  //右边夹子电机
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON)  //到达闭合位
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE); //停止电机
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);	//停止电机
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_CLOSE_FAIL;	 //10009 右边夹子关闭失败
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_RightClampOpen
*****************************************************
Descriptions:  右边夹子打开
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
void Op_RightClampOpen(void)
{
	stOpCtrl *CurrentOp = &OpRightClampOpen;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}
	switch (CurrentOp->nStep)
	{				
		case START:
			if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON)  //已经在打开位
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_CW); //电机运行 
				SetOpTimeDelay(CurrentOp,3000);    //延时
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON)  //运行到打开位
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);	//电机停止
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_OPEN_FAIL;	//10010 右边夹子打开失败
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_LeftClampClose
*****************************************************
Descriptions: 左边夹子关闭
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
			if (input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON)	//已经在关闭位
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_CW); //电机运行
				SetOpTimeDelay(CurrentOp,3000); 
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON) //运行到关闭位
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //停止电机
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE);
				CurrentOp->nResult = ERROR_LEFT_CLAMP_CLOSE_FAIL;	//10011 左边夹子关闭失败
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_LeftClampOpen
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
void Op_LeftClampOpen(void)
{
	stOpCtrl *CurrentOp = &OpLeftClampOpen;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}
	switch (CurrentOp->nStep)
	{				
		case START:
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON) //已经在打开位
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_CW);   //运行电机
				SetOpTimeDelay(CurrentOp,3000); 	//延时3S
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON) //运行到打开位
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //停止电机
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //停止电机
				CurrentOp->nResult = ERROR_LEFT_CLAMP_OPEN_FAIL; //10012 左边传感器故障导致夹子打开失败
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //结束程序清理使能
}
/****************************************************
Function Name: Op_ClampClose
*****************************************************
Descriptions: 夹子关闭
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
				StartChildOp(CurrentOp, &OpRightClampClose); //关闭右边夹子
				StartChildOp(CurrentOp, &OpLeftClampClose);  //关闭左边夹子
				CurrentOp->nStep = STEP1;	
			break;

			case STEP1:	
				if((IsChildOpOK(CurrentOp, &OpRightClampClose) == true) && (IsChildOpOK(CurrentOp, &OpLeftClampClose) == true))
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
Function Name: Op_ClampOpen
*****************************************************
Descriptions: 夹子打开
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
void Op_ClampOpen(void)
{
	stOpCtrl *CurrentOp = &OpClampOpen;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:	
			StartChildOp(CurrentOp, &OpRightClampOpen); //右边夹子打开
			StartChildOp(CurrentOp, &OpLeftClampOpen);  //左边夹子打开
			CurrentOp->nStep = STEP1;	
    	break;
		case STEP1:	
			if((IsChildOpOK(CurrentOp, &OpRightClampOpen) == true) && (IsChildOpOK(CurrentOp, &OpLeftClampOpen) == true))
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
Function Name: Op_ResetClamp
*****************************************************
Descriptions: 夹子复位
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
void Op_ResetClamp(void)
{
		stOpCtrl *CurrentOp = &OpResetClamp;
		if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
		{
			return;	
		}	
		switch (CurrentOp->nStep)
		{
			case START:	
				StartChildOp(CurrentOp, &OpClampOpen); //夹子打开 
				CurrentOp->nStep = STEP1;
			break;	
			case STEP1:	
				if(IsChildOpOK(CurrentOp, &OpClampOpen) == true) //完成
				{
					CurrentOp->nStep = STEP2;
				}	
			break;
			case STEP2:	
				StartChildOp(CurrentOp, &OpClampClose); //夹子关闭
				CurrentOp->nStep = STEP3;
			break;
			case STEP3:	
				if(IsChildOpOK(CurrentOp, &OpClampClose) == true) //完成
				{
					CurrentOp->nResult = 0xffff;
				}	
			break;
			default:
			break;
		}
		DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_ClampCardCarMove
*****************************************************
Descriptions:  小车移动
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ClampCardCarMove(void)
{
	stOpCtrl *CurrentOp = &OpClampCardCarMove;
	static unsigned char ChannelTrolleyCarPos = 0; //接收通道小车的位置
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (gTrolleyCarMove_Flag == true) //底层调用
			{
				gTrolleyCarMove_Flag = false;
				ChannelTrolleyCarPos = gTrolleyCarPosition; 
			}
			else //上层调用
			{
				ChannelTrolleyCarPos = cParamBuf[10];
			}
			if(ChannelTrolleyCarPos>5)
			{
				CurrentOp->nResult = ERROR_TROLLEY_CAR_POSITION_PARA_MAX;	  	//10013 抓卡小车上层传输的数据超出接收最大值
			}		
			CurrentOp->nStep = STEP20;					
		break;
/////////////////////////////////////////////////////////////////////////////
/*增加小车移动时候的保护,小车移动时如果不把预定位(撕膜位)下降和废料位上升,
冷压热压剪卡升起,夹子闭合,小车移动时会撞到导致电机失歩及皮带对不准工位  */
		case STEP20:
			StartChildOp(CurrentOp, &OpPretargetingReset);  //预定位复位
			CurrentOp->nStep = STEP21;
		break;

		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //废料场复位
				CurrentOp->nStep = STEP22;
			}
		break;
		case STEP22:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				StartChildOp(CurrentOp, &OpPresRise);  		//裁剪工位复位
				CurrentOp->nStep = STEP23;
			}
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);  		//冷压工位复位
				CurrentOp->nStep = STEP24;
			}
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//热压工位复位
				CurrentOp->nStep = STEP25;
			}
		break;
		case STEP25:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				StartChildOp(CurrentOp, &OpClampClose);  		//爪子闭合
				CurrentOp->nStep = STEP26;
			}
		break;
		case STEP26:
			if(IsChildOpOK(CurrentOp, &OpClampClose) == true)
			{
				CurrentOp->nStep = STEP1;
			}
		break;
/////////////////////////////////////////////////////////////////////////////////////
		case STEP1:
			if (gTrolleyCarPosition == ENUM_UNKNOW) //位置未知
			{				
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}             	
		break;
		case STEP2:
			StartChildOp(CurrentOp,&OpResetClampCardCar); //复位小车		
			CurrentOp->nStep = STEP3;	
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpResetClampCardCar) == true) //完成
			{
					CurrentOp->nStep = STEP4;	
			}
		break;
		case STEP4:
			if(gTrolleyCarMoveCount<100) //运行到一定次数后复位清除累积误差
			{
				gTrolleyCarMoveCount++;
				CurrentOp->nStep = STEP5;
			}
			else
			{
				CurrentOp->nStep = STEP2;
				gTrolleyCarMoveCount = 0;
			}
		break;
		case STEP5:   //根据传下来的位置走绝对步数
			sm_run_abs(SM_CHANNEL,parameter[PAR_SM_CHANNAL_SPEED],iAbsPosTrolleyCarMov[ChannelTrolleyCarPos]);	
			CurrentOp->nStep= STEP6;
		break;
		case STEP6:
			if(IsSmRunFinish(SM_CHANNEL) == true)	//小车完成运动
			{
				sm_stop(SM_CHANNEL); //停止电机
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;

		case STEP_SUCCESS:
			gTrolleyCarPosition = ChannelTrolleyCarPos; //同步上层传下来的位置
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);			
}
/****************************************************
Function Name: Op_PretargetingReset
*****************************************************
Descriptions:  预定位复位
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_PretargetingReset(void)
{
	stOpCtrl *CurrentOp = &OpPretargetingReset;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(nStatusPretarget == STATUS_ORG) //标志在原点
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_ON)	 //传感器检测在原点
			{
					nStatusPretarget = STATUS_UNKNOW; //清标志位 
					CurrentOp->nStep = STEP20;
			} 
			else //不在原点  
			{
				nStatusPretarget = STATUS_UNKNOW;
				sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_BACKWARD,10,6000);	 //回原点
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_ON) //检测到回到原点
			{
				sm_stop(SM_PRETARGETING); //停止电机
				sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_BACKWARD,10,350);		 //多走一点
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_PRETARGETING) == true)
			{
				CurrentOp->nResult = ERROR_PRETARGETING_RETURN_ORIGIN_FAIL;	  //10014 预定位返回原点失败
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_PRETARGETING) == true)
			{
				nStatusPretarget = STATUS_ORG; //在原点
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_FORWARD,10,400);	 			//离开原点
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_PRETARGETING) == true)
			{
				if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_OFF)	//离开原点
				{
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL; //10015 预定位离开原点失败
				}
			}
		break;
		default:
		break;
	}	

	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_PretargetingUp
*****************************************************
Descriptions:  预定位升起
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_PretargetingUp(void)
{
	stOpCtrl *CurrentOp = &OpPretargetingUp;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(nStatusPretarget == STATUS_POSITION) //在升起位
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(nStatusPretarget == STATUS_ORG)	 //在原点处
			{
				CurrentOp->nStep = STEP2; 									
			} 
			else //其它状态
			{
				StartChildOp(CurrentOp, &OpPretargetingReset); //预定位复位
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				CurrentOp->nStep = STEP2; 
			}
		break;
		case STEP2:
			sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_FORWARD,10,parameter[PAR_PRETARGET_UP_POSITION]);	 //离开原点	
			CurrentOp->nStep = STEP3; 
		break;
		case STEP3:
			if(IsSmRunFinish(SM_PRETARGETING) == true)
			{
				nStatusPretarget = STATUS_POSITION;
				CurrentOp->nResult = 0xffff;
			}
		break;
		default:
		break;
	}	
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_WasteStorageReset
*****************************************************
Descriptions:  废料场电机复位
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/

void Op_WasteStorageReset(void)
{
	stOpCtrl *CurrentOp = &OpWasteStorageReset;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(	nStatusWastestorage == STATUS_ORG) //标志位在原点
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON)	 //传感器检测到在原点
			{
				nStatusWastestorage = STATUS_UNKNOW;
				CurrentOp->nStep = STEP20;
			} 
			else
			{
				nStatusWastestorage = STATUS_UNKNOW;
				sm_run(SM_WASTESTORAGE,SM_CCW,10,2000);	 //回原点
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_WASTESTORAGE); //停止电机
				sm_run(SM_WASTESTORAGE,SM_CCW,10,100);	 //多走一点
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				CurrentOp->nResult = ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL;	 //10016 废卡位返回原点失败
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				nStatusWastestorage = STATUS_ORG;  //在原点
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_WASTESTORAGE,SM_CW,10,200);	 			//离开原点
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_OFF)	//离开原点
			{
					CurrentOp->nStep = START;
			}else
			{
					CurrentOp->nResult = ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL; //10017 废卡位离开原点失败
			}
			}
		break;
		default:
		break;
	}	
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_WasteStorageDown
*****************************************************
Descriptions:  废料场电机下降
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_WasteStorageDown(void)
{
 	stOpCtrl *CurrentOp = &OpWasteStorageDown;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(nStatusWastestorage == STATUS_POSITION) //在下降位
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(nStatusWastestorage == STATUS_ORG) //在原点处
			{
				CurrentOp->nStep = STEP2; 									
			} 
			else
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //复位
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				CurrentOp->nStep = STEP2; 
			}
		break;
		case STEP2:
			sm_run(SM_WASTESTORAGE,SM_CW,10, parameter[PAR_WASTESTORAGE_DN_POSTION]); //向下走一定步数
			CurrentOp->nStep = STEP3; 
		break;
		case STEP3:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				nStatusWastestorage = STATUS_POSITION; //在下降位
				CurrentOp->nResult = 0xffff;
			}
		break;
		default:
		break;
	}	
	DealResult(CurrentOp);
}	

/****************************************************
Function Name: Op_PretargetingVacuumCupOpen
*****************************************************
Descriptions:  预定位真空吸打开
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_PretargetingVacuumCupOpen(void)
{
	stOpCtrl *CurrentOp = &OpPretargetingVacuumCupOpen;

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
			dm_ctrl_one(DM_PRETARGETING_VACUUMCUP, RELAY_ON); //真空吸继电器打开
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
Function Name: Op_PretargetingVacuumCupClose
*****************************************************
Descriptions:  预定位真空吸关闭
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_PretargetingVacuumCupClose(void)
{
 	stOpCtrl *CurrentOp = &OpPretargetingVacuumCupClose;
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
			dm_ctrl_one(DM_PRETARGETING_VACUUMCUP, RELAY_OFF); //预定位真空吸继电器关闭
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
Function Name: Op_WasteStorageVacuumCupOpen
*****************************************************
Descriptions: 废料场真空吸打开
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/	
void Op_WasteStorageVacuumCupOpen(void)
{
 	stOpCtrl *CurrentOp = &OpWasteStorageVacuumCupOpen;

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
			dm_ctrl_one(DM_WASTESTORAGE_VACUUMCUP, RELAY_ON);  //废料场真空吸打开
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
Function Name: Op_WasteStorageVacuumCupClose
*****************************************************
Descriptions: 废料场真空吸关闭
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_WasteStorageVacuumCupClose(void)
{
	stOpCtrl *CurrentOp = &OpWasteStorageVacuumCupClose;
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
			dm_ctrl_one(DM_WASTESTORAGE_VACUUMCUP, RELAY_OFF); //肥料场真空吸关闭
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
Function Name: Op_OpenGasSwitch
*****************************************************
Descriptions: 气路打开
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_OpenGasSwitch(void)
{
	stOpCtrl *CurrentOp = &OpOpenGasSwitch;
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
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);	  //气路继电器打开
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
Function Name: Op_OpenGasSwitch
*****************************************************
Descriptions: 气路关闭
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_CloseGasSwitch(void)
{
	stOpCtrl *CurrentOp = &OpCloseGasSwitch;
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
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_OFF);	  //气路继电器关闭
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
Function Name: Op_ResetModule
*****************************************************
Descriptions: 复位模块
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_ResetModule(void)
{
	stOpCtrl *CurrentOp = &OpResetModule;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			bFlagReset = FALSE;
			nStatusPretarget = STATUS_UNKNOW;		//预定位状态未知
			nStatusWastestorage = STATUS_UNKNOW;	//废料场状态未知
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpOpeningPressure);  //开油压
			StartChildOp(CurrentOp, &OpOpenGasSwitch);	  //开气压
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if((IsChildOpOK(CurrentOp, &OpOpeningPressure) == true) && (IsChildOpOK(CurrentOp, &OpOpenGasSwitch) == true))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpPretargetingReset);  		//预定位复位
			CurrentOp->nStep = STEP4;
	   	break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset);  	//废料场复位
				CurrentOp->nStep = STEP5;
			}
	   	break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				StartChildOp(CurrentOp, &OpPresRise);  					//裁剪工位复位
				CurrentOp->nStep = STEP6;
			}
	   	break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);  		//冷压工位复位
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//热压工位复位
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				StartChildOp(CurrentOp, &OpResetClamp);  		//爪子复位
				CurrentOp->nStep = STEP9;
			}
		break;
		case STEP9:
			if(IsChildOpOK(CurrentOp, &OpResetClamp) == true)
			{
				StartChildOp(CurrentOp, &OpResetClampCardCar);  		//小车复位
				CurrentOp->nStep = STEP10;
			}
		break;	
		case STEP10:
			if(IsChildOpOK(CurrentOp, &OpResetClampCardCar) == true)
			{
				bFlagReset = TRUE;  //复位标志
				CurrentOp->nResult = 0xFFFF;
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_MakeCard
*****************************************************
Descriptions: 制卡模块
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_MakeCard(void)
{
	stOpCtrl *CurrentOp = &OpMakeCard;

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
			StartChildOp(CurrentOp, &OpClampClose);		//启动爪子闭合
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpClampClose) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);	//预定位复位
				CurrentOp->nStep = STEP3;		
			}
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);	//冷压复位
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);	//热压复位
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				CurrentOp->nStep = STEP13;
			}
		break;
		case STEP13:
			gTrolleyCarMove_Flag = true;
			gTrolleyCarPosition = ENUM_INSIDE_WARE;
			StartChildOp(CurrentOp, &OpClampCardCarMove); //去热压工位
			CurrentOp->nStep = STEP14;
		break;
		case STEP14:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpWarmDown);	//去热压下降
				CurrentOp->nStep = STEP15;		
			}
	   	break;
		case STEP15:
			if(IsChildOpOK(CurrentOp, &OpWarmDown) == true)
			{
				SetOpTimeDelay(CurrentOp, parameter[PAR_WARM_TIME]*1000);	//热压保压延时
				CurrentOp->nStep = STEP16;		
			}
	   	break;
		case STEP16:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);	//去热压上升
				CurrentOp->nStep = STEP17;		
			}
		break;
		case STEP17:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				gTrolleyCarMove_Flag = true;
				gTrolleyCarPosition = ENUM_INSIDE_COOL;
				StartChildOp(CurrentOp, &OpClampCardCarMove);	//去冷压工位
				CurrentOp->nStep = STEP18;		
			}
		break;
		case STEP18:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpCoolDown);//去冷压下降
				CurrentOp->nStep = STEP19;		
			}
	   	break;
		case STEP19:
			if(IsChildOpOK(CurrentOp, &OpCoolDown) == true)
			{
				SetOpTimeDelay(CurrentOp, parameter[PAR_COOL_TIME]*1000);//冷压保压延时
				CurrentOp->nStep = STEP20;		
			}
	   	break;
		case STEP20:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpCoolRise); //去冷压上升
				CurrentOp->nStep = STEP21;		
			}
		break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				gTrolleyCarMove_Flag = true;
				gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING;
				StartChildOp(CurrentOp, &OpClampCardCarMove); //回预定位工位
				CurrentOp->nStep = STEP22;		
			}
		break;
		case STEP22:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);	//启动预定位上升
				CurrentOp->nStep = STEP23;		 
			}
	   	break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp) == true)
			{
				StartChildOp(CurrentOp, &OpClampOpen);	//启动爪子张开
				CurrentOp->nStep = STEP24;	
			}
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpClampOpen) == true)
			{
				CurrentOp->nResult = 0xFFFF;
			}
		break;	
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_GotoLoadingPlatform
*****************************************************
Descriptions: 到预定位+预定位上升+打开夹子
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_GotoLoadingPlatform(void)
{
	stOpCtrl *CurrentOp = &OpGotoLoadingPlatform;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			if(bFlagReset) //已经复位过
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				StartChildOp(CurrentOp, &OpResetModule); //复位压铸模块
				CurrentOp->nStep = STEP1;
			}	
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp, &OpResetModule) == true)
			{
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if(nStatusPretarget == STATUS_ORG)	//确保预定位在原点，防止夹爪被撞
			{
				CurrentOp->nStep = STEP4;	
			}
			else
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);	//预定位复位
				CurrentOp->nStep = STEP3;	
			}
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			gTrolleyCarMove_Flag = true;  					//小车运行标志
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING; //运行到位置
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//回预定位工位
			CurrentOp->nStep = STEP5;		
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);//启动预定位上升
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp) == true)
			{
				StartChildOp(CurrentOp, &OpClampOpen); //打开夹子
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpClampOpen) == true)
			{
				CurrentOp->nResult = 0xFFFF;
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_CuttingPlatformWork
*****************************************************
Descriptions: 剪卡+废料
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_CuttingPlatformWork(void)
{
	stOpCtrl *CurrentOp = &OpCuttingPlatformWork;

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
			StartChildOp(CurrentOp, &OpClampClose);	//启动爪子闭合
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpClampClose) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingReset); //预定位复位
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{					
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(	nStatusWastestorage == STATUS_ORG)
			{
				CurrentOp->nStep = STEP6;
			}
			else
			{
			StartChildOp(CurrentOp, &OpWasteStorageReset);//启动废料平台复位			
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:			   			
			StartChildOp(CurrentOp, &OpPresRise);	//启动冲压复位
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				CurrentOp->nStep = STEP8;		
			}
		break;
		case STEP8:
			gTrolleyCarMove_Flag = true;  //爪卡小车运行标志
			gTrolleyCarPosition = ENUM_INSIDE_CUT_CARD; //运行位置
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//到裁剪工位
			CurrentOp->nStep = STEP9;	
		break;
		case STEP9:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP10;		 
			}
	   	break;
		case STEP10:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpPresDown);	//裁剪下降
				CurrentOp->nStep = STEP11;		
			}
		break;
		case STEP11:
			if(IsChildOpOK(CurrentOp, &OpPresDown) == true)
			{					
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP12;	
			}
		break;
		case STEP12:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpPresRise);	//裁剪上升
				CurrentOp->nStep = STEP13;		
			}
		break;
		case STEP13:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				CurrentOp->nStep = STEP14;		
			}
		break;
		case STEP14:
			gTrolleyCarMove_Flag = true; 
			gTrolleyCarPosition = ENUM_INSIDE_WASTE;
			StartChildOp(CurrentOp, &OpClampCardCarMove);//到废料场工位
			CurrentOp->nStep = STEP15;	
		break;
		case STEP15:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				SetOpTimeDelay(CurrentOp, 1000);			
				CurrentOp->nStep = STEP16;		 
			}
	   	break;
///////////////////////////////先打开夹子然后往下压把废料压掉下去/////////////////////////////
		case STEP16:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpClampOpen);//打开夹子
				CurrentOp->nStep = STEP17;
			}
		break;
		case STEP17:
			if (IsChildOpOK(CurrentOp,&OpClampOpen) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageDown);	//启动废料平台下降			
				CurrentOp->nStep = STEP18;			
			}
			
		break;
		case STEP18:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageDown) == true)
			{
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP19;
			}
		break;
		case STEP19:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp,&OpWasteStorageReset); //废料平台升起
				CurrentOp->nStep = STEP20;		
			}
		break;
		case STEP20:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				StartChildOp(CurrentOp,&OpClampClose); //关闭夹子
				CurrentOp->nStep = STEP21;
			}
		break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpClampClose) == true)
			{
				CurrentOp->nStep =STEP22;
			}
		break;
		case STEP22:
			gTrolleyCarMove_Flag = true; 					 //运行标志
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING;  //运行位置
			StartChildOp(CurrentOp, &OpClampCardCarMove);	 //回预定位工位
			CurrentOp->nStep = STEP23;		
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				CurrentOp->nResult = 0xFFFF;	 
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ScramProcess
*****************************************************
Descriptions:  紧急停止制剪卡流程
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
void Op_ScramProcess(void)
{
	stOpCtrl *CurrentOp = &OpScramProcess;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			OpCuttingPlatformWork.bEn = false; 	 //失能剪卡流程
			OpMakeCard.bEn = false;			   	 //失能热压流程
			sm_stop(SM_CHANNEL);			     //停止电机
			StartChildOp(CurrentOp,&OpWarmRise); //升起热压
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp,&OpWarmRise) == true)
			{
				StartChildOp(CurrentOp,&OpCoolRise); //冷压升起
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp,&OpCoolRise) == true)
			{
				StartChildOp(CurrentOp,&OpPresRise); //剪卡升起
				CurrentOp->nStep = STEP3;
			}			
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp,&OpPresRise) == true)
			{
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			CurrentOp->nResult = 0xffff;
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_SuckCardCarMove
*****************************************************
Descriptions:  吸卡小车移动
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_SuckCardCarMove(void)
{
	stOpCtrl *CurrentOp = &OpSuckCardCarMove;
	static unsigned char SuckCardCarPos = 0; //接收通道小车的位置
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (gSuckCardCarMove_Flag == true) //底层调用
			{
				gSuckCardCarMove_Flag= false;
				SuckCardCarPos = gSuckCardCarPosition; //取出底层调用的位置
			}
			else //上层调用
			{
				SuckCardCarPos = cParamBuf[10]; //数据位
			}
			if(SuckCardCarPos>6)
			{
				CurrentOp->nResult = ERROR_SUCK_CARD_CAR_POSITION_PARA_MAX;   //10018 吸卡小车上层传输的数据超出接收最大值
			}		
			CurrentOp->nStep = STEP20;					
		break;	
 		case STEP20: //保护,吸卡小车移动前将吸盘复位,避免撞到
			StartChildOp(CurrentOp, &OpSuckReset);  //吸盘复位 
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpSuckReset) == true)
			{
				CurrentOp->nStep = STEP1;
			}
		break; 
		case STEP1:
			if (gSuckCardCarPosition == ENUM_SUCK_UNKNOW) //位置未知
			{				
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}             	
		break;
		case STEP2:
			StartChildOp(CurrentOp,&OpResetSuckCardCar); //复位吸卡小车		
			CurrentOp->nStep = STEP3;	
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpResetSuckCardCar) == true) //完成
			{
					CurrentOp->nStep = STEP4;	
			}
		break;
		case STEP4:
			if(gSuckCardCarMoveCount<100) //运行到一定次数后复位清除累积误差
			{
				gSuckCardCarMoveCount++;
				CurrentOp->nStep = STEP5;
			}
			else
			{
				CurrentOp->nStep = STEP2;
				gSuckCardCarMoveCount = 0;
			}
		break;
		case STEP5:   //根据传下来的位置走绝对步数
			sm_run_abs(SM_SUCK_CARD_CAR,parameter[PAR_SM_CHANNAL_SPEED],iAbsSuckCardCarMov[SuckCardCarPos]);	
			CurrentOp->nStep= STEP6;
		break;
		case STEP6:
			if(IsSmRunFinish(SM_SUCK_CARD_CAR) == true)	//小车完成运动
			{
				sm_stop(SM_SUCK_CARD_CAR); //停止电机
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;

		case STEP_SUCCESS:
			gTrolleyCarPosition = SuckCardCarPos; //同步上层传下来的位置
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);			
}
/****************************************************
Function Name: Op_ResetSuckCardCar
*****************************************************
Descriptions:  复位吸卡小车
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
void Op_ResetSuckCardCar(void)
{
	stOpCtrl *CurrentOp = &OpResetSuckCardCar;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
		case START:
			gSuckCardCarPosition = ENUM_SUCK_UNKNOW;
			CurrentOp->nStep = STEP20;
		break;
		case STEP20:
			StartChildOp(CurrentOp, &OpSuckReset); //吸盘复位
			CurrentOp->nStep = STEP21;
	   	break;
	
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpSuckReset) == true)
			{
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_OFF) 	//不在原点	
			{
				sm_run(SM_SUCK_CARD_CAR, DIR_SM_SUCK_CARD_CAR_BACKWARD, 80, 50000); //向后运行一段距离
				CurrentOp->nStep = STEP5;
			}
			else if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_ON) //在原点
			{
				sm_run(SM_SUCK_CARD_CAR, DIR_SM_SUCK_CARD_CAR_FORWARD, 80, 1000); //向前运行一段距离
				CurrentOp->nStep = STEP2;
			}			
		break;
		case STEP2:
			if(IsSmRunFinish(SM_SUCK_CARD_CAR) == true)  
			{
				if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_OFF) //离开原点
				{
					SetOpTimeDelay(CurrentOp,300);   //反转延时
					CurrentOp->nStep = STEP3;	
				}
				else if(IsSmRunFinish(SM_SUCK_CARD_CAR))        	
				{
					CurrentOp->nResult = ERROR_SUCK_CARD_CAR_LEAVE_ORIGIN_FAIL;  //10019 吸卡小车离开原点失败
				}	
			}
		break;																																																	
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{
				CurrentOp->nStep = STEP1;
			}	
		break;
		case STEP5:
			if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_ON) //到原点
			{
				sm_stop(SM_SUCK_CARD_CAR); 			//停止电机
				CurrentOp->nStep = STEP6;
			}	
			else if (IsSmRunFinish(SM_CHANNEL) == true) //电机动作完成
			{
				CurrentOp->nResult = ERROR_SUCK_CARD_CAR_RETURN_ORIGIN_FAIL; //10020 吸卡小车离开原点失败			
			}		
		break;
		case STEP6:
			sm_run(SM_SUCK_CARD_CAR, DIR_SM_SUCK_CARD_CAR_BACKWARD, 10, 5);  //继续往回走一段距离
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if (IsSmRunFinish(SM_CHANNEL) == true)
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			sm_set_abs_zero(SM_SUCK_CARD_CAR); //清零步进电机的步数
			gTrolleyCarPosition = ENUM_SUCK_ORIGIN; //吸卡小车在原点
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_SuckReset
*****************************************************
Descriptions:  吸盘复位
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_SuckReset(void)
{
	stOpCtrl *CurrentOp = &OpSuckReset;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(	nStatusSuck == ENUM_SUCK_ORIGIN) //标志位在原点
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_SUCK_ORG) == SENSOR_TYPE_BEAM_ON)	 //传感器检测到在原点
			{
				nStatusSuck = ENUM_SUCK_UNKNOW;
				CurrentOp->nStep = STEP20;
			} 
			else
			{
				nStatusSuck = ENUM_SUCK_UNKNOW;
				sm_run(SM_SUCK,DIR_SM_SUCK_UP,50,2000);	 //回原点
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_SUCK_ORG) == SENSOR_TYPE_BEAM_ON) //传感器检测到在原点
			{
				sm_stop(SM_SUCK); //停止电机
				sm_run(SM_SUCK,DIR_SM_SUCK_UP,10,100);	 //多走一点
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_SUCK) == true)
			{
				CurrentOp->nResult = ERROR_SUCK_RETURN_ORIGIN_FAIL;	 //10021 吸盘返回原点失败
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_SUCK) == true)
			{
				nStatusSuck = ENUM_SUCK_ORIGIN;  //在原点
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_SUCK,DIR_SM_SUCK_DOWN,50,200);  //离开原点
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_SUCK) == true)
			{
				if (input_get_one(SN_SUCK_ORG) == SENSOR_TYPE_BEAM_OFF)	//离开原点
				{
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_SUCK_LEAVE_ORIGIN_FAIL;	//10022 吸盘离开原点失败
				}
			}
		break;
		default:
		break;
	}	
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_SuckMove
*****************************************************
Descriptions:  吸盘下降
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_SuckMove(void)
{
	stOpCtrl *CurrentOp = &OpSuckMove;
	static unsigned char SuckPos = 0; //接收吸盘的位置
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (gSuckMove_Flag == true) //底层调用
			{
				gSuckMove_Flag= false;
				SuckPos = gSuckPosition; //取出底层调用的位置
			}
			else //上层调用
			{
				SuckPos = cParamBuf[10]; //数据位
			}
			if(SuckPos>6)
			{
				CurrentOp->nResult = ERROR_SUCK_POSITION_PARA_MAX;//10023 吸卡盘上层传输的数据超出接收最大值
			}		
			CurrentOp->nStep = STEP1;					
		break;	
		case STEP1:
			if (gSuckPosition == ENUM_SUCK_UNKNOW) //位置未知
			{				
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}             	
		break;
		case STEP2:
			StartChildOp(CurrentOp,&OpSuckReset); //复位吸盘		
			CurrentOp->nStep = STEP3;	
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpSuckReset) == true) //完成
			{
				CurrentOp->nStep = STEP4;	
			}
		break;
		case STEP4:
			if(gSuckMoveCount<100) //运行到一定次数后复位清除累积误差
			{
				gSuckMoveCount++;
				CurrentOp->nStep = STEP5;
			}
			else
			{
				CurrentOp->nStep = STEP2;
				gSuckMoveCount = 0;
			}
		break;
		case STEP5:   //根据传下来的位置走绝对步数
			sm_run_abs(SM_SUCK,parameter[PAR_SM_CHANNAL_SPEED],iAbsSuckMove[SuckPos]);	
			CurrentOp->nStep= STEP6;
		break;
		case STEP6:
			if(IsSmRunFinish(SM_SUCK) == true)	//小车完成运动
			{
				sm_stop(SM_SUCK); //停止电机
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;

		case STEP_SUCCESS:
			gTrolleyCarPosition = SuckPos; //同步上层传下来的位置
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);			
}
/****************************************************
Function Name: Op_SuckMove
*****************************************************
Descriptions: 白卡箱复位
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_BlankCardBoxReset(void)
{
	stOpCtrl *CurrentOp = &OpBlankCardBoxReset;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(	nStatusBlankCardBox == STATUS_ORG) //标志位在原点
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_BLANK_CARD_BOX_ORG) == SENSOR_TYPE_BEAM_ON)  //传感器检测到在原点
			{
				nStatusBlankCardBox = STATUS_UNKNOW;
				CurrentOp->nStep = STEP20;
			} 
			else
			{
				nStatusBlankCardBox = STATUS_UNKNOW;
				sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_DOWN,10,2000);	 //回原点
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_BLANK_CARD_BOX_ORG) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_BLANK_CARD_BOX); //停止电机
				sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_DOWN,10,5);	 //多走一点
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_BLANK_CARD_BOX) == true)
			{
				CurrentOp->nResult = ERROR_BLANK_CARD_BOX_RESET_FAIL;  //10024 白卡箱复位失败
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				nStatusBlankCardBox = STATUS_ORG;  //在原点
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_UP,10,100);	 //离开原点
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_BLANK_CARD_BOX) == true)
			{
				if (input_get_one(SN_BLANK_CARD_BOX_ORG) == SENSOR_TYPE_BEAM_OFF)	//离开原点
				{
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_BLANK_CARD_BOX_LEAVE_ORIGIN_FAIL; //10025 白卡箱离开原点失败
				}
			}
		break;
		default:
		break;
	}	
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_BlankCardBoxUp
*****************************************************
Descriptions:  白卡箱升起
*****************************************************
Calls:         
*****************************************************
Input  parameter: 
*****************************************************
Output parameter: None
*****************************************************
Return value: None
*****************************************************
Author:Jim Wong
*****************************************************/
void Op_BlankCardBoxUp(void)
{
	stOpCtrl *CurrentOp = &OpBlankCardBoxUp;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(input_get_one(SN_BLANK_CARD_BOX_CHECK) == SENSOR_TYPE_REFLECT_ON) //有卡
			{
				CurrentOp->nResult = 0xffff;
			}
			else //其它状态
			{
				sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_UP,50,parameter[PAR_BLANK_CARD_BOX_UP]); //升起一定步数
				CurrentOp->nStep = STEP1;
			}
				
		break;
		case STEP1:
			if(input_get_one(SN_BLANK_CARD_BOX_UP) == SENSOR_TYPE_BEAM_ON) //到达顶部
			{
				sm_stop(SM_BLANK_CARD_BOX); //停止电机
				CurrentOp->nResult = ERROR_BLANK_CARD_BOX_EMPTY;  //10026 白卡箱空
			}
			else if(input_get_one(SN_BLANK_CARD_BOX_CHECK) == SENSOR_TYPE_REFLECT_ON) //有卡
			{

				sm_stop(SM_BLANK_CARD_BOX); //停止电机
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
	Op_ResetModule();						//1 复位
	Op_ResetClampCardCar(); 				//2 复位夹卡小车通道
	Op_ClampCardCarMove(); 					//3 夹卡小车移动
	Op_ResetClamp();						//4 爪子复位
	Op_ClampOpen();							//5 爪子张开
	Op_ClampClose();						//6 爪子闭合
	Op_RightClampClose();			     	//7 右侧夹子关闭 			
  	Op_RightClampOpen();				   	//8 右侧夹子打开          
	Op_LeftClampClose();					//9 左侧夹子关闭
	Op_LeftClampOpen();					   	//10 左侧夹子打开
	Op_OpeningPressure();				   	//11 压力开
	Op_ClosingPressure();				   	//12 压力关
	Op_WarmRise();						    //13 加热下降
	Op_WarmDown();						   	//14 加热升起
	Op_CoolRise();						   	//15 冷却下降
	Op_CoolDown();						   	//16 冷却升起
	Op_PresRise();						   	//17 冲压下降
	Op_PresDown();						   	//18 冲压升起
	Op_PretargetingReset();					//19 预定位复位
	Op_PretargetingUp();					//20 预定位上升
	Op_WasteStorageReset();		 			//21 废料场复位
	Op_WasteStorageDown();	   				//22 废料场下降
	Op_PretargetingVacuumCupOpen();			//23 预定位真空吸打
	Op_PretargetingVacuumCupClose();		//24 预定位真空吸关闭
	Op_WasteStorageVacuumCupOpen();			//25 废料场真空吸打开
	Op_WasteStorageVacuumCupClose();		//26 废料场真空吸关闭
	Op_OpenGasSwitch();			  			//27 打开总气开关
	Op_CloseGasSwitch();				  	//28 关闭总气开关
	Op_MakeCard();							//29 制卡
	Op_GotoLoadingPlatform();				//30 去装料平台	
	Op_CuttingPlatformWork();				//31 裁剪平台工作
	Op_ScramProcess();						//32 紧急停止制剪卡流程
	Op_SuckCardCarMove();					//33 吸卡小车移动位置
	Op_ResetSuckCardCar();					//34 吸卡小车复位
	Op_SuckReset();							//35 吸盘复位
	Op_SuckMove();							//36 吸盘下降


}






