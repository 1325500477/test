#include "../Parameter/Parameter.h"
#include "../Command/command.h"
#include "../IOCfg.h"
#include "../../BSP/bsp.h"
#include "../../BSP/Command/bsp_command.h"
#include "process.h"
#include <string.h>
#include <stdio.h>

/******** 相关动作结构体变量定义 ******************************/
stOpCtrl OpResetModule;	            			//1 复位
stOpCtrl OpResetClampCardCar;   				//2 复位夹卡小车
stOpCtrl OpClampCardCarMove; 					//3 夹卡小车移动位置	
stOpCtrl OpResetClamp;							//4 爪子复位
stOpCtrl OpClampOpen;							//5 爪子张开
stOpCtrl OpClampClose;							//6 爪子闭合
stOpCtrl OpRightClampClose; 					//7 右边夹子闭合
stOpCtrl OpRightClampOpen; 						//8 右边夹子松开
stOpCtrl OpLeftClampClose; 						//9 左边夹子闭合
stOpCtrl OpLeftClampOpen; 						//10 左边夹子松开
stOpCtrl OpOpeningPressure;	 					//11 压力开
stOpCtrl OpClosingPressure; 					//12 压力关
stOpCtrl OpWarmDown; 			 				//13 加热下降
stOpCtrl OpWarmRise; 			 		  		//14 加热升起
stOpCtrl OpCoolDown; 				  			//15 冷却下降
stOpCtrl OpCoolRise; 			 	  			//16 冷却升起
stOpCtrl OpPresDown; 				  			//17 冲压下降
stOpCtrl OpPresRise; 			 	  			//18 冲压升起	
stOpCtrl OpPretargetingReset;            		//19 预定位复位
stOpCtrl OpPretargetingUp;                		//20 预定位上升
stOpCtrl OpWasteStorageReset;  					//21 废料场电机复位
stOpCtrl OpWasteStorageDown;	 				//22 废料场电机下降
stOpCtrl OpPretargetingVacuumCupOpen;			//23 预定位真空吸打开
stOpCtrl OpPretargetingVacuumCupClose;			//24 预定位真空吸关闭
stOpCtrl OpOpenGasSwitch;			 			//25 气路开启
stOpCtrl OpCloseGasSwitch;						//26 气路关闭
//组合动作
stOpCtrl OpWarmCoolMakeCard;					//27 热冷压制卡
stOpCtrl OpGotoLoadingPlatform;			   		//28 去预定位平台
stOpCtrl OpCuttingPlatformWork;		 			//29 裁剪平台工作
stOpCtrl OpScramProcess;						//30 紧急停止剪卡和制卡流程
//电梯模块
stOpCtrl OpResetElevatorCar;					//31 复位电梯小车
stOpCtrl OpElevatorCarMove;						//32 电梯小车移动
stOpCtrl OpResetConnection;						//33 复位衔接模块
stOpCtrl OpConnectionSendCard;					//34 衔接模块到发卡位
stOpCtrl OpReceiveCardVacuumOn;					//35 接卡位真空开
stOpCtrl OpReceiveCardVacuumOff;				//36 接卡位真空关
stOpCtrl OpHeaterPowerOn;						//37 发热管电源开
stOpCtrl OpHeaterPowerOff;						//38 发热管电源关
stOpCtrl OpRWModuleInCard;						//39 读写模块进卡
stOpCtrl OpRWModuleOutCard;						//40 读写模块出卡
//组合流程
stOpCtrl OpElevatorSendCardToRWModule;			//41 电梯发卡到读写模块
stOpCtrl OpRWModuleSendCardToOCR;				//42 读写模块发卡到OCR
stOpCtrl OpOCRToRWModuleOutCard;				//43 OCR到读写模块出卡


stOpCtrl OpAutoMakeCard;						//44 自动制卡


unsigned char gStatusPretarget 	  = STATUS_UNKNOW;		//预定位位置状态
unsigned char gStatusWastestorage = STATUS_UNKNOW;  	//废料位置状态
unsigned char bFlagReset 		  = FALSE;        		//复位标志位
/******** 其它定义*********************************/
stOpCtrl *gMachineOperation[50]; // 设置一个支持50个流程的指针数组
unsigned char gnOperationNo = 1; // 执行的动作序号（从1开始对各个动作结构体变量逐个初始化）
extern unsigned char cParamBuf[];//接收参数
//Clamp card car
unsigned char gTrolleyCarMove_Flag 	 = false;  		//通道小车运行标志
unsigned char gTrolleyCarPosition  	 = ENUM_UNKNOW; //通道小车的位置
unsigned int gTrolleyCarMoveCount 	 = 0;           //小车移动次数,达到一定次数复位消除累积误差
unsigned int gAbsPosTrolleyCarMov[6] = { 	//抓卡小车移动各个工位的绝对步数初始值
	0, 	 	//空余
	13, 	//1热压位 
	165,	//2冷压位
	311,	//3预定位
	448,	//4剪卡位
	580,	//5废料位
 };	
//电梯小车
unsigned char gRWCheckCar_Flag	    = false; //电梯小车运行标志位
unsigned char gElevatorCarPosition  = ENUM_ELEVATOR_UNKNOW;  //电梯小车位置
unsigned int  gRWCheckCarCount	    = 0;	//电梯小车运行达到一定次数复位消除累积误差
//衔接模块 
unsigned char gConnectionPosition   = STATUS_UNKNOW; //Connection module position flag
unsigned int gAbsElevatorCarMove[6] = { //电梯小车移动
	0,	  //空余
	250,  //1接卡位
	42,   //2电梯到读写模块位(好卡出卡位)
	195,  //3读写模块到电梯位
	177,  //4废卡位
	13,   //5OCR位 
}; 
/****************************************************
Function Name: ProcessUpdateParameter
*****************************************************
Descriptions:  
刷新步进电机绝对步数参数提供给步进电机移动函数使用
Op_SuckCardCarMove()
Op_SuckMove()
Op_ClampCardCarMove()
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
void ProcessUpdateParameter(void)
{	
	unsigned char i;
 	for (i = 0; i < 5; i++)
	{
		//1 到热压工位
		//2 到冷压工位
		//3 到废料工位
		//4 到预定位
		//5 裁剪卡位置		
		gAbsPosTrolleyCarMov[i+1] = parameter[i];
		//12接卡位
		//13电梯到读写模块
		//14读写模块到电梯
		//15废卡位
		//16OCR检测位 	 		
		gAbsElevatorCarMove[i+1] = parameter[i+11];
	} 
	/* 	gAbsPosTrolleyCarMov[ENUM_INSIDE_WARE] 	       = parameter[PAR_WARM_STEP];		   //1 到热压工位
		gAbsPosTrolleyCarMov[ENUM_INSIDE_COOL]         = parameter[PAR_COOL_STEP];		   //2 到冷压工位
		gAbsPosTrolleyCarMov[ENUM_INSIDE_WASTE]    	   = parameter[PAR_WASTE];	   		   //3 到撕膜工位
		gAbsPosTrolleyCarMov[ENUM_INSIDE_PRETARGETING] = parameter[PAR_PRETARGETING];	   //4 到预定位
		gAbsPosTrolleyCarMov[ENUM_INSIDE_CUT_CARD] 	   = parameter[PAR_CUT_CARD_POSITION]; //5 到剪卡位
	*/
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
	gMachineOperation[gnOperationNo++] = &OpOpenGasSwitch;				//25 总气打开	
	gMachineOperation[gnOperationNo++] = &OpCloseGasSwitch;				//26 总气关闭
	//组合动作
	gMachineOperation[gnOperationNo++] = &OpWarmCoolMakeCard;			//27 热冷压制卡	
	gMachineOperation[gnOperationNo++] = &OpGotoLoadingPlatform;		//28 到预定位+预定位上升+打开夹子
	gMachineOperation[gnOperationNo++] = &OpCuttingPlatformWork;		//29 剪卡+废料				 
	gMachineOperation[gnOperationNo++] = &OpScramProcess;				//30 紧急停止剪卡和制卡流程
	//电梯模块
	gMachineOperation[gnOperationNo++] = &OpResetElevatorCar;			//31 复位电梯小车
	gMachineOperation[gnOperationNo++] = &OpElevatorCarMove;			//32 电梯小车移动
	gMachineOperation[gnOperationNo++] = &OpResetConnection;			//33 复位衔接模块
	gMachineOperation[gnOperationNo++] = &OpConnectionSendCard;			//34 衔接模块到发卡位
	gMachineOperation[gnOperationNo++] = &OpReceiveCardVacuumOn;		//35 接卡位负压开
	gMachineOperation[gnOperationNo++] = &OpReceiveCardVacuumOff;		//36 接卡位负压关
	gMachineOperation[gnOperationNo++] = &OpHeaterPowerOn;				//37 发热管电源开
	gMachineOperation[gnOperationNo++] = &OpHeaterPowerOff;				//38 发热管电源关
	gMachineOperation[gnOperationNo++] = &OpRWModuleInCard;				//39 读写模块进卡
	gMachineOperation[gnOperationNo++] = &OpRWModuleOutCard;			//40 读写模块出卡
	gMachineOperation[gnOperationNo++] = &OpElevatorSendCardToRWModule;	//41 电梯发卡到读写模块
	gMachineOperation[gnOperationNo++] = &OpRWModuleSendCardToOCR;		//42 读写模块发卡到OCR
	gMachineOperation[gnOperationNo++] = &OpOCRToRWModuleOutCard;		//43 OCR到读写模块出卡

	gMachineOperation[gnOperationNo++] = &OpAutoMakeCard;				//44 自动制卡
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
Function Name: Op_ResetModule
*****************************************************
Descriptions: Reset all module
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
			gStatusPretarget = STATUS_UNKNOW;		//Pretargeting status unknow
			gStatusWastestorage = STATUS_UNKNOW;	//Waste status unknow
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpOpeningPressure);  //Hydraulic open
			StartChildOp(CurrentOp, &OpOpenGasSwitch);	  //Gas open
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if((IsChildOpOK(CurrentOp, &OpOpeningPressure)) && 
				(IsChildOpOK(CurrentOp, &OpOpenGasSwitch)))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpPretargetingReset);  //Pretargeting reset
			CurrentOp->nStep = STEP4;
	   	break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset)) //Pretargeting reset finish
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset);  //Waste reset 
				CurrentOp->nStep = STEP5;
			}
	   	break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				StartChildOp(CurrentOp, &OpPresRise);  //Cut Up
				CurrentOp->nStep = STEP6;
			}
	   	break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpPresRise))
			{
				StartChildOp(CurrentOp, &OpCoolRise);  //Cool Up
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpCoolRise))
			{
				StartChildOp(CurrentOp, &OpWarmRise);  	//Warm Up
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if(IsChildOpOK(CurrentOp, &OpWarmRise))
			{
				StartChildOp(CurrentOp, &OpResetClamp);  //Clamp reset
				CurrentOp->nStep = STEP9;
			}
		break;
		case STEP9:
			if(IsChildOpOK(CurrentOp, &OpResetClamp))
			{
				StartChildOp(CurrentOp, &OpResetClampCardCar);  //Clamp card car reset
				CurrentOp->nStep = STEP10;
			}
		break;	
		case STEP10:
			if(IsChildOpOK(CurrentOp, &OpResetClampCardCar))
			{
				StartChildOp(CurrentOp, &OpResetElevatorCar);  	//Elevator car reset
				CurrentOp->nStep = STEP11;	
			}
		break;
		case STEP11:
			if(IsChildOpOK(CurrentOp, &OpResetElevatorCar))
			{
				StartChildOp(CurrentOp, &OpResetConnection);  	//Connection reset
				CurrentOp->nStep = STEP12;	
			}
		break;
		case STEP12:
			if(IsChildOpOK(CurrentOp,&OpResetConnection))
			 {
				bFlagReset = TRUE;  //Reset flag
				CurrentOp->nResult = 0xffff;				
			 }
		break;
		default:
		break;
	} 
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ResetClampCardCar
*****************************************************
Descriptions:  Reset clamp car
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
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpPretargetingReset);     //Pretargeting reset
			CurrentOp->nStep = STEP2;
	   	break;
		case STEP2:
		   	if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //Waste Reset
				CurrentOp->nStep = STEP3;
			}
	   	break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				StartChildOp(CurrentOp, &OpPresRise);  	//Cut reset
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpPresRise))
			{
				StartChildOp(CurrentOp, &OpCoolRise);  	//Cool Rise
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpCoolRise))
			{
				StartChildOp(CurrentOp, &OpWarmRise);  	//Warm Rise
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpWarmRise))
			{
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) 	//Not origin	
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 150,PULSE_TO_MILLIMETER(1500)); //Backward run a distance
				CurrentOp->nStep = STEP10;
			}
			else  //在原点
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_FORWARD, 150, PULSE_TO_MILLIMETER(40)); //Forward run a distance
				CurrentOp->nStep = STEP8;
			}			
		break;
		case STEP8:
			if(IsSmRunFinish(SM_CHANNEL))  
			{
				if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //Leave origin
				{
					SetOpTimeDelay(CurrentOp,300);   //Reverse run delay
					CurrentOp->nStep = STEP9;	
				}
				else       	
				{
					CurrentOp->nResult = ERROR_CHANNAL_LEAVE_ORG_FAIL;   //11001离开原点传感器失败
				}	
			}
		break;																																																	
		case STEP9:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP7;
			}	
		break;
		case STEP10:
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //At the origin
			{
				sm_stop(SM_CHANNEL); 	//Stop Motor
				CurrentOp->nStep = STEP11;
			}	
			else if (IsSmRunFinish(SM_CHANNEL)) //Motor action finish
			{
				CurrentOp->nResult = ERROR_CHANNAL_RETURN_ORG_FAIL;	//11002 通道返回传感器失败			
			}		
		break;
		case STEP11:
			sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 100,PULSE_TO_MILLIMETER(1)); //Continue backward run a distance
			CurrentOp->nStep = STEP12;
		break;
		case STEP12:
			if (IsSmRunFinish(SM_CHANNEL))
			{
				sm_set_abs_zero(SM_CHANNEL); //Clean step motor of steps
				gTrolleyCarPosition = ENUM_INSIDE_ORIGIN; //At origin
				CurrentOp->nResult = 0xffff;
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_WarmDown
*****************************************************
Descriptions:  Warm down
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
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_ON)	 //At Down position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_WARM_CONTROL, RELAY_ON); //Open down hydraulic relay
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_ON) //At down position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_WARMING_DOWN_FAIL; //11003加热下降失败
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
Descriptions:  Warm rise
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
			if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)	 //At rise position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_WARM_CONTROL, RELAY_OFF); //Close hydraulic relay,warm rise
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON) //At rise position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_WARMING_RISE_FAIL; //11004加热升起失败
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
Descriptions:  Cool down
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
			if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_ON)	 //At down
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_COOL_CONTROL, RELAY_ON); //Open Hydraulic relay,Cool down
				SetOpTimeDelay(CurrentOp,3000); 
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_ON) //At down
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_COOLING_DOWN_FAIL; //11005冷却下降失败
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
Descriptions:  Cool rise
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
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON)  //At rise
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_COOL_CONTROL, RELAY_OFF); //Close  hydraulic relay,cool rise
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON) //At rise position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_COOLING_RISE_FAIL;  //11006冷却升起失败
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
Descriptions:  Cut down
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
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON)   //At down
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_PRES_CONTROL, RELAY_ON);	//Cut position hydraulic relay open,Cut down
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON) //At down
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_PRESS_DOWN_FAIL;	 //11007冲压下降失败
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
Descriptions:  Cut rise
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
			if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON)	 //At rise
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_PRES_CONTROL, RELAY_OFF); //Hydraulic close Cut rise
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON) //At rise position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_PRESS_RISE_FAIL;	 //11008 冲压升起失败
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
Descriptions:  Hydraulic open
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
			dm_ctrl_one(DM_HYDRULIC_POWER, RELAY_ON);	//Hydraulic power on
			dm_ctrl_one(DM_ALL_CONTROL, RELAY_ON);		//Hydraulic open
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ClosingPressure
*****************************************************
Descriptions:  Hydraulic close
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
			dm_ctrl_one(DM_HYDRULIC_POWER, RELAY_OFF);	//Hydraulic power on
			dm_ctrl_one(DM_ALL_CONTROL, RELAY_OFF);  	//Hydraulic close
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_RightClampClose
*****************************************************
Descriptions: Right Clamp close
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
			if (input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON) //At close position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else //其它
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_CW);  //Right clamp motor 
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON)  //to close position
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE); //Stop motor
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);	//Stop motor
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_CLOSE_FAIL;	 //11009 右边夹子关闭失败
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
Descriptions:  Right Clamp Open
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
			if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON)  //At open position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_CW); //Right clamp motor
				SetOpTimeDelay(CurrentOp,3000);    
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON)  //Run to open position
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);	//Stop motor
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_OPEN_FAIL;	//11010 右边夹子打开失败
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
Descriptions: Left Clamp Close
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
			if (input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON)	//At close position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_CW); //Left Clamp run
				SetOpTimeDelay(CurrentOp,3000); 
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON) //Run to Close position
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //Stop motor
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE);
				CurrentOp->nResult = ERROR_LEFT_CLAMP_CLOSE_FAIL;	//11011 左边夹子关闭失败
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
Descriptions:  Left clamp open
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
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON) //At open position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_CW);   //Ldft motor run
				SetOpTimeDelay(CurrentOp,3000); 	
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON) //Run to open position
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //Stop motor
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); 
				CurrentOp->nResult = ERROR_LEFT_CLAMP_OPEN_FAIL; //11012 左边传感器故障导致夹子打开失败
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
Descriptions: Clamp close
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
				StartChildOp(CurrentOp, &OpRightClampClose); //Close right clamp
				StartChildOp(CurrentOp, &OpLeftClampClose);  //Close left clamp
				CurrentOp->nStep = STEP1;	
			break;

			case STEP1:	
				if((IsChildOpOK(CurrentOp, &OpRightClampClose)) &&
					 (IsChildOpOK(CurrentOp, &OpLeftClampClose)))
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
		DealResult(CurrentOp); //Clean action 
}
/****************************************************
Function Name: Op_ClampOpen
*****************************************************
Descriptions: Clamp open
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
			StartChildOp(CurrentOp, &OpRightClampOpen); //Right clamp open
			StartChildOp(CurrentOp, &OpLeftClampOpen);  //Left clamp open
			CurrentOp->nStep = STEP1;	
    	break;
		case STEP1:	
			if((IsChildOpOK(CurrentOp, &OpRightClampOpen)) &&
				 (IsChildOpOK(CurrentOp, &OpLeftClampOpen)))
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
Descriptions: Clamp reset
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
			StartChildOp(CurrentOp, &OpClampOpen); //Clamp open
			CurrentOp->nStep = STEP1;
		break;	
		case STEP1:	
			if(IsChildOpOK(CurrentOp, &OpClampOpen)) 
			{
				CurrentOp->nStep = STEP2;
			}	
		break;
		case STEP2:	
			StartChildOp(CurrentOp, &OpClampClose); //Clamp close
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:	
			if(IsChildOpOK(CurrentOp, &OpClampClose)) 
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
Descriptions:  Clamp car move
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
	static unsigned char ChannelTrolleyCarPos = 0; //Receive car of position
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (gTrolleyCarMove_Flag == true) //Bottom call
			{
				gTrolleyCarMove_Flag = false;
				ChannelTrolleyCarPos = gTrolleyCarPosition; 
			}
			else //Upper call
			{
				ChannelTrolleyCarPos = cParamBuf[10];
			}
			if(ChannelTrolleyCarPos > 5)
			{
				CurrentOp->nResult = ERROR_TROLLEY_CAR_POSITION_PARA_MAX; //11013 抓卡小车上层传输的数据超出接收最大值
			}		
			CurrentOp->nStep = STEP1;					
		break;
	/////////////////////////////////////////////////////////////////////////////
	/*增加小车移动时候的保护,小车移动时如果不把预定位(撕膜位)下降和废料位上升,
	冷压热压剪卡升起,夹子闭合,小车移动时会撞到导致电机失歩及皮带对不准工位  */	
		case STEP1:
			if(gStatusPretarget == STATUS_UNKNOW) //Pretargeting position unknow
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);  //Pretargeting reset
				CurrentOp->nStep = STEP2;	
			}
			else
			{
				CurrentOp->nStep = STEP3;
			}		
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (gStatusWastestorage == STATUS_UNKNOW) //Waster position unknow
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //Waste reset
				CurrentOp->nStep = STEP4;				
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				StartChildOp(CurrentOp, &OpPresRise);  		//Cut reset
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpPresRise))
			{
				StartChildOp(CurrentOp, &OpCoolRise);  		//Cool reset
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpCoolRise))
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//Warm reset
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpWarmRise))
			{
				CurrentOp->nStep = STEP8;
			}
		break;
	/////////////////////////////////////////////////////////////////////////////////////
		case STEP8:
			if (gTrolleyCarPosition == ENUM_UNKNOW) //Position unknow
			{				
				CurrentOp->nStep = STEP9;
			}
			else
			{
				CurrentOp->nStep = STEP11;
			}             	
		break;
		case STEP9:
			StartChildOp(CurrentOp,&OpResetClampCardCar); //Reset car		
			CurrentOp->nStep = STEP10;	
		break;
		case STEP10:
			if (IsChildOpOK(CurrentOp,&OpResetClampCardCar)) 
			{
				CurrentOp->nStep = STEP11;	
			}
		break;
		case STEP11:
			if(gTrolleyCarMoveCount<100) //运行到一定次数后复位清除累积误差
			{
				gTrolleyCarMoveCount++;
				CurrentOp->nStep = STEP12;
			}
			else
			{
				CurrentOp->nStep = STEP9;
				gTrolleyCarMoveCount = 0;
			}
		break;
		case STEP12:   //According Upper transfer of position go absolute step
			sm_run_abs(SM_CHANNEL,parameter[PAR_SM_CHANNAL_SPEED],PULSE_TO_MILLIMETER(gAbsPosTrolleyCarMov[ChannelTrolleyCarPos]));	
			CurrentOp->nStep= STEP13;
		break;
		case STEP13:
			if(IsSmRunFinish(SM_CHANNEL))	
			{
				gTrolleyCarPosition = ChannelTrolleyCarPos; //SynchronizeUpper transfer of position
				CurrentOp->nResult = 0xffff;
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);			
}
/****************************************************
Function Name: Op_PretargetingReset
*****************************************************
Descriptions:  Pretargeting Reset
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
			// if(gStatusPretarget == STATUS_ORG)
			// {
			// 	CurrentOp->nResult = 0xffff;
			// }
			if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_ON)	//Sersor check At origin
			{
					CurrentOp->nStep = STEP3;
			} 
			else //Not origin 
			{
				gStatusPretarget = STATUS_UNKNOW; //Clean flag
				sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_BACKWARD,10,6000); //Backward origin
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_ON) //Return to origin
			{
				sm_stop(SM_PRETARGETING); //停止电机
				sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_BACKWARD,10,350); //Again go a distance
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_PRETARGETING))
			{
				CurrentOp->nResult = ERROR_PRETARGETING_RETURN_ORIGIN_FAIL;	  //11014 预定位返回原点失败
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_PRETARGETING))
			{
				gStatusPretarget = STATUS_ORG; // origin
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP3:
			sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_FORWARD,10,400);	 //Leave origin
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if(IsSmRunFinish(SM_PRETARGETING))
			{
				if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_OFF)	//Already leave origin
				{
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL; //11015 预定位离开原点失败
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
Descriptions:  Pretargeting Up
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
			if(gStatusPretarget == STATUS_POSITION) //At Up position
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(gStatusPretarget == STATUS_ORG)	 //At origin
			{
				CurrentOp->nStep = STEP2; 									
			} 
			else 
			{
				StartChildOp(CurrentOp, &OpPretargetingReset); //Pretargeting Reset
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{
				CurrentOp->nStep = STEP2; 
			}
		break;
		case STEP2: //Run to Up position
			sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_FORWARD,10,parameter[PAR_PRETARGET_UP_POSITION]);	 //离开原点	
			CurrentOp->nStep = STEP3; 
		break;
		case STEP3:
			if(IsSmRunFinish(SM_PRETARGETING))
			{
				if(input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_OFF)
				{
					gStatusPretarget = STATUS_POSITION; //At up position
					CurrentOp->nResult = 0xffff;
				}
				else
				{
					gStatusPretarget = STATUS_UNKNOW;
					CurrentOp->nResult = ERROR_PRETARGETING_UP_LEAVE_ORIGIN_FAIL;//11033 预定位升起时离开原点失败
				}
				
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
Descriptions:  Waste Reset
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
			// if (gStatusWastestorage == STATUS_ORG)
			// {
			// 	CurrentOp->nResult = 0xffff;
			// }
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON)	 //Sersor check At origin
			{
				CurrentOp->nStep = STEP3;
			} 
			else
			{
				gStatusWastestorage = STATUS_UNKNOW;
				sm_run(SM_WASTESTORAGE,SM_CCW,10,2000);	 //Backward origin
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON) //Run to origin
			{
				sm_stop(SM_WASTESTORAGE); //Stop Motor
				sm_run(SM_WASTESTORAGE,SM_CCW,10,100);	 //Continue go a distance
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_WASTESTORAGE))
			{
				CurrentOp->nResult = ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL;	 //11016 废卡位返回原点失败
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_WASTESTORAGE))
			{
				gStatusWastestorage = STATUS_ORG;  //At origin
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP3:
			sm_run(SM_WASTESTORAGE,SM_CW,10,200);	 			//Motor leave origin
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if(IsSmRunFinish(SM_WASTESTORAGE))
			{
				if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_OFF)
				{	//Sensor check leave origin
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL; //11017 废卡位离开原点失败
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
Descriptions:  Waste down
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
			if(gStatusWastestorage == STATUS_POSITION) //Flag At down flag
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(gStatusWastestorage == STATUS_ORG) //At origin
			{
				CurrentOp->nStep = STEP2; 									
			} 
			else
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //Waste reset
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				CurrentOp->nStep = STEP2; 
			}
		break;
		case STEP2:
			sm_run(SM_WASTESTORAGE,SM_CW,10, parameter[PAR_WASTESTORAGE_DN_POSTION]); //Motor run
			CurrentOp->nStep = STEP3; 
		break;
		case STEP3:
			if(IsSmRunFinish(SM_WASTESTORAGE))
			{
				if(input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_OFF)
				{
					gStatusWastestorage = STATUS_POSITION; //At down
					CurrentOp->nResult = 0xffff;
				}
				else
				{
					gStatusWastestorage = STATUS_UNKNOW;
					CurrentOp->nResult = ERROR_WASTESTORAGE_DOWN_LEAVE_ORIGIN_FAIL;//11034 废卡位下降离开原点失败 
				}
				
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
Descriptions:  Pretargeting Vacuum Open
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
			dm_ctrl_one(DM_PRETARGETING_VACUUMCUP, RELAY_ON); //Pretargeting Vacuum relay open
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_PretargetingVacuumCupClose
*****************************************************
Descriptions:  Pretargeting Vacuum Close
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
			dm_ctrl_one(DM_PRETARGETING_VACUUMCUP, RELAY_OFF); //Pretargeting Vacuum relay Close
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name: Op_OpenGasSwitch
*****************************************************
Descriptions: Gas open
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
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);	  //Gas relay open
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_OpenGasSwitch
*****************************************************
Descriptions: Gas close
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
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_OFF);	  //Gas relay close
			CurrentOp->nStep = STEP2;
		break;

		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}

/****************************************************
Function Name:Op_WarmCoolMakeCardd
*****************************************************
Descriptions: 热冷压制卡模块
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
void Op_WarmCoolMakeCard(void)
{
	stOpCtrl *CurrentOp = &OpWarmCoolMakeCard;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			if (input_get_one(SN_PRETARGETING_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //Pretargeting check have card
			{
				CurrentOp->nStep = STEP1;					
			}
			else
			{
				CurrentOp->nResult = ERROR_PRETARGETING_NONE_CARD;	//11018 预定位无卡
			}
		break;
	   	case STEP1:
			StartChildOp(CurrentOp, &OpClampClose);					//Clamp Close
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpClampClose))
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);		//Pretargeting Reset
				CurrentOp->nStep = STEP3;		
			}
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{
				StartChildOp(CurrentOp, &OpCoolRise);				//Cool Rise
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpCoolRise))
			{
				StartChildOp(CurrentOp, &OpWarmRise);				//Warm Rise
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpWarmRise))
			{
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			gTrolleyCarMove_Flag = true;
			gTrolleyCarPosition = ENUM_INSIDE_WARE; 	   			//Warm 
			StartChildOp(CurrentOp, &OpClampCardCarMove);  			//Run to warm
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
				if (input_get_one(SN_COOL_CHECK_CARD) == SENSOR_TYPE_BEAM_ON) // Cool position Have card
				{
					CurrentOp->nStep = STEP8;	
				}
				else if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
				{
					sm_stop(SM_CHANNEL);
					CurrentOp->nResult = ERROR_PRETARGETING_FROM_COOL_NONE_CARD; //11019 预定位到冷压位无卡
				}
		break;
		case STEP8:	
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpWarmDown);	//Warm Down
				CurrentOp->nStep = STEP9;		
			}
	   	break;
		case STEP9:
			if(IsChildOpOK(CurrentOp, &OpWarmDown))
			{
				SetOpTimeDelay(CurrentOp, parameter[PAR_WARM_TIME]*1000);	//Warm delay
				CurrentOp->nStep = STEP10;		
			}
	   	break;
		case STEP10:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpWarmRise);	//Warm Rise
				CurrentOp->nStep = STEP11;		
			}
		break;
		case STEP11:
			if(IsChildOpOK(CurrentOp, &OpWarmRise))
			{
				gTrolleyCarMove_Flag = true;
				gTrolleyCarPosition = ENUM_INSIDE_COOL;
				StartChildOp(CurrentOp, &OpClampCardCarMove);	//Run to cool
				CurrentOp->nStep = STEP12;		
			}
		break;
		case STEP12:
			if (input_get_one(SN_WARM_CHECK_CARD) == SENSOR_TYPE_BEAM_ON) //Warm  position detected have card
			{
				CurrentOp->nStep = STEP13;	
			}
			else if(IsChildOpOK(CurrentOp, &OpClampCardCarMove)) //Action before finish not detected card 
			{
				sm_stop(SM_CHANNEL); //Stop car
				CurrentOp->nResult = ERROR_WARM_FROM_COOL_NONE_CARD; //11020 热压位到冷压位无卡
			}	
		break;
		case STEP13:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpCoolDown);//Cool Down
				CurrentOp->nStep = STEP14;		
			}
	   	break;
		case STEP14:
			if(IsChildOpOK(CurrentOp, &OpCoolDown))
			{
				SetOpTimeDelay(CurrentOp, parameter[PAR_COOL_TIME]*1000);//Cool delay
				CurrentOp->nStep = STEP15;		
			}
	   	break;
		case STEP15:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpCoolRise); //Cool Rise
				CurrentOp->nStep = STEP16;		
			}
		break;
		case STEP16:
			if(IsChildOpOK(CurrentOp, &OpCoolRise))
			{
				gTrolleyCarMove_Flag = true;
				gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING; //Pretargeting
				StartChildOp(CurrentOp, &OpClampCardCarMove); //go back Pretargeting
				CurrentOp->nStep = STEP17;		
			}
		break;
		case STEP17:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);	//Pretargeting Up
				CurrentOp->nStep = STEP18;		 
			}
	   	break;
		case STEP18:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp))
			{
				StartChildOp(CurrentOp, &OpClampOpen);	//Clamp Open
				CurrentOp->nStep = STEP19;	
			}
		break;
		case STEP19:
			if (input_get_one(SN_PRETARGETING_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //Pretargeting detected have card
			{
				CurrentOp->nStep = STEP20;					
			}
			else
			{
				CurrentOp->nResult = ERROR_COOL_FROM_PRETARGETING_NONE_CARD;//11021 冷压位到预定位无卡
			}	
		break;
		case STEP20:
			if(IsChildOpOK(CurrentOp, &OpClampOpen))
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
Function Name: Op_GotoLoadingPlatform
*****************************************************
Descriptions: Go to pretargeting+Pretargeting up+Clamp open
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
			if(bFlagReset) //Already reset
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				StartChildOp(CurrentOp, &OpResetModule); //Reset module
				CurrentOp->nStep = STEP1;
			}	
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp, &OpResetModule))
			{
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if(gStatusPretarget == STATUS_ORG)	//确保预定位在原点，防止夹爪被撞
			{
				CurrentOp->nStep = STEP4;	
			}
			else
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);	//Pretargeting reset
				CurrentOp->nStep = STEP3;	
			}
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			gTrolleyCarMove_Flag = true;  					//Car run flag
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING; 
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//Go back pretargeting
			CurrentOp->nStep = STEP5;		
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);	//Pretargeting Up
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp))
			{
				StartChildOp(CurrentOp, &OpClampOpen); 		//Clamp Open
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpClampOpen))
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
Function Name: Op_CuttingPlatformWork
*****************************************************
Descriptions: Cut card+waste
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
			if (input_get_one(SN_PRETARGETING_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //Pretargeting detected have card
			{
				CurrentOp->nStep = STEP1;					
			}
			else
			{
				CurrentOp->nResult = ERROR_PRETARGETING_FROM_WASTE_NONE_CARD;//11022 预定位到废料位无卡
			}
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpClampClose);	//Clamp Close
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpClampClose))
			{
				StartChildOp(CurrentOp, &OpPretargetingReset); //Pretargeting Reset
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{					
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(	gStatusWastestorage == STATUS_ORG) //Waste At origin
			{
				CurrentOp->nStep = STEP6;
			}
			else
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset);//Waste Reset			
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:			   			
			StartChildOp(CurrentOp, &OpPresRise);	//Cut card reset
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpPresRise))
			{
				CurrentOp->nStep = STEP8;		
			}
		break;
		case STEP8:
			gTrolleyCarMove_Flag = true;  
			gTrolleyCarPosition = ENUM_INSIDE_CUT_CARD; //Cut card position
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//Move to Cut card
			CurrentOp->nStep = STEP9;	
		break;
		case STEP9:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP10;		 
			}
	   	break;
		case STEP10:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpPresDown);	//Cut down
				CurrentOp->nStep = STEP11;		
			}
		break;
		case STEP11:
			if(IsChildOpOK(CurrentOp, &OpPresDown))
			{					
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP12;	
			}
		break;
		case STEP12:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpPresRise);	//Cut rise
				CurrentOp->nStep = STEP13;		
			}
		break;
		case STEP13:
			if(IsChildOpOK(CurrentOp, &OpPresRise))
			{
				CurrentOp->nStep = STEP14;		
			}
		break;
		case STEP14:
			gTrolleyCarMove_Flag = true; 
			gTrolleyCarPosition = ENUM_INSIDE_WASTE; //Waste
			StartChildOp(CurrentOp, &OpClampCardCarMove);//move to waste
			CurrentOp->nStep = STEP15;	
		break;
		case STEP15:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				SetOpTimeDelay(CurrentOp, 500);			
				CurrentOp->nStep = STEP16;		 
			}
	   	break;
	///////////////////////////////先打开夹子然后往下压把废料压掉下去/////////////////////////////
		case STEP16:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpClampOpen);//Clamp Open
				CurrentOp->nStep = STEP17;
			}
		break;
		case STEP17:
			if (IsChildOpOK(CurrentOp,&OpClampOpen))
			{
				StartChildOp(CurrentOp, &OpWasteStorageDown);	//Waste platform down		
				CurrentOp->nStep = STEP18;			
			}
			
		break;
		case STEP18:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageDown))
			{
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP19;
			}
		break;
		case STEP19:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp,&OpWasteStorageReset); //Waste rise
				CurrentOp->nStep = STEP20;		
			}
		break;
		case STEP20:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				CurrentOp->nStep = STEP21;
			}
		break;
		case STEP21:
			gTrolleyCarMove_Flag = true; 					 
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING;  //Pretargeting
			StartChildOp(CurrentOp, &OpClampCardCarMove);	 //Go back pretargeting
			CurrentOp->nStep = STEP22;		
		break;
		case STEP22:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);//Pretargeting rise
				CurrentOp->nStep = STEP23;
			}
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp))
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
Function Name: Op_ScramProcess
*****************************************************
Descriptions:  Scram stop make card process
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
			OpCuttingPlatformWork.bEn = false; 	 //Disable Cut card process
			OpWarmCoolMakeCard.bEn = false;		 //Disable make card process
			sm_stop(SM_CHANNEL);			     //Stop motor
			StartChildOp(CurrentOp,&OpWarmRise); //Warm rise
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp,&OpWarmRise))
			{
				StartChildOp(CurrentOp,&OpCoolRise); //Cool Rise
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp,&OpCoolRise))
			{
				StartChildOp(CurrentOp,&OpPresRise); //Cut rise
				CurrentOp->nStep = STEP3;
			}			
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp,&OpPresRise))
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
Function Name: Op_ResetElevatorCar
*****************************************************
Descriptions:  Reset Elevator car
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
void Op_ResetElevatorCar(void)
{
	stOpCtrl *CurrentOp = &OpResetElevatorCar;

	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
		case START:
			gElevatorCarPosition = ENUM_ELEVATOR_UNKNOW;
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if(gConnectionPosition == STATUS_ORG) //Connection at position
			{
				CurrentOp->nStep = STEP3;
			}
			else
			{
				StartChildOp(CurrentOp, &OpResetConnection); //Reset Connection module	
				CurrentOp->nStep = STEP2;			
			}
			
	   	break;
		case STEP2:
			 if(IsChildOpOK(CurrentOp, &OpResetConnection))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (input_get_one(SN_ELEVATOR_CAR_ORG) == SENSOR_TYPE_BEAM_OFF) 	// Not origin	
			{
				sm_run(SM_ELEVATOR_CAR, DIR_SM_ELEVATOR_CAR_DOWN,parameter[PAR_ELEVATOR_CAR_SPEED],PULSE_TO_MILLIMETER(334)); //Go back run a distance
				CurrentOp->nStep = STEP6;
			}
			else //Origin
			{
				sm_run(SM_ELEVATOR_CAR, DIR_SM_ELEVATOR_CAR_UP,parameter[PAR_ELEVATOR_CAR_SPEED], PULSE_TO_MILLIMETER(13)); //Go forward run a distance
				CurrentOp->nStep = STEP4;
			}			
		break;
		case STEP4:
			if(IsSmRunFinish(SM_ELEVATOR_CAR))  
			{
				if (input_get_one(SN_ELEVATOR_CAR_ORG) == SENSOR_TYPE_BEAM_OFF) //Leave origin
				{
					SetOpTimeDelay(CurrentOp,300);   //Reversal delay
					CurrentOp->nStep = STEP5;	
				}
				else        	
				{
					CurrentOp->nResult = ERROR_ELEVATOR_CAR_LEAVE_ORIGIN_FAIL; //11023 电梯小车离开原点失败
				}	
			}
		break;																																																	
		case STEP5:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP3;
			}	
		break;
		case STEP6:
			if (input_get_one(SN_ELEVATOR_CAR_ORG) == SENSOR_TYPE_BEAM_ON) //Go to origin
			{
				sm_stop(SM_ELEVATOR_CAR); 	//Stop elevator car		
				CurrentOp->nStep = STEP7;
			}	
			else if (IsSmRunFinish(SM_ELEVATOR_CAR)) 
			{
				CurrentOp->nResult = ERROR_ELEVATOR_CAR_RETURN_ORIGIN_FAIL; //11024 电梯小车返回原点失败
			}		
		break;
		case STEP7:
			sm_run(SM_ELEVATOR_CAR, DIR_SM_ELEVATOR_CAR_DOWN, parameter[PAR_ELEVATOR_CAR_SPEED],10);  //Continue move a distance
			CurrentOp->nStep = STEP8;
		break;
		case STEP8:
			if (IsSmRunFinish(SM_ELEVATOR_CAR))
			{
				sm_set_abs_zero(SM_ELEVATOR_CAR); //Clean step motor of absolute steps
				gElevatorCarPosition = ENUM_ELEVATOR_CAR_ORG; //Elevator car is origin
				CurrentOp->nResult  = 0xffff;	
			}
		break;

		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ElevatorCarMove
*****************************************************
Descriptions:  Elevator car move
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
void Op_ElevatorCarMove(void)
{
	stOpCtrl *CurrentOp = &OpElevatorCarMove;
	static unsigned char ElevatorCarPos = 0; //Receive elevator car position
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (gRWCheckCar_Flag == true) //Bottom call flag
			{
				gRWCheckCar_Flag = false;
				ElevatorCarPos = gElevatorCarPosition; //Get bottom call position
			}
			else //Upper call
			{
				ElevatorCarPos = cParamBuf[10]; //Get data bit
			}
			if(ElevatorCarPos > 5)
			{
				CurrentOp->nResult = ERROR_ELEVATOR_CAR_POSITION_PARA_MAX;   //11025 电梯小车上层传输的数据超出接收最大值
			}		
			CurrentOp->nStep = STEP1;					
		break;	
 		case STEP1: //Protection elevator car befor move reset Connection avert crash
		 	if(gConnectionPosition == STATUS_ORG) //Connection at position
			{
				CurrentOp->nStep = STEP3;
			}
			else
			{
				StartChildOp(CurrentOp, &OpResetConnection);  //Reset connection
				CurrentOp->nStep = STEP2;				
			}
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpResetConnection)) 
			{
				CurrentOp->nStep = STEP3;
			}
		break; 
		case STEP3:
			if (gElevatorCarPosition == ENUM_ELEVATOR_UNKNOW) //Unknow position
			{				
				CurrentOp->nStep = STEP4;
			}
			else
			{
				CurrentOp->nStep = STEP6;
			}             	
		break;
		case STEP4:
			StartChildOp(CurrentOp,&OpResetElevatorCar); //Reset elevator car		
			CurrentOp->nStep = STEP5;	
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpResetElevatorCar) == true) 
			{
				CurrentOp->nStep = STEP6;	
			}
		break;
		case STEP6:
			if(gRWCheckCarCount < 100) //运行到一定次数后复位清除累积误差
			{
				gRWCheckCarCount++;
				CurrentOp->nStep = STEP7;
			}
			else
			{
				CurrentOp->nStep = STEP4;
				gRWCheckCarCount = 0;
			}
		break;
		case STEP7:   //According upper transfer of position go absolute steps
			sm_run_abs(SM_ELEVATOR_CAR,parameter[PAR_ELEVATOR_CAR_SPEED],PULSE_TO_MILLIMETER(gAbsElevatorCarMove[ElevatorCarPos]));	
			CurrentOp->nStep= STEP8;
		break;
		case STEP8:
			if(IsSmRunFinish(SM_ELEVATOR_CAR))
			{
				sm_stop(SM_ELEVATOR_CAR); 
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			gElevatorCarPosition = ElevatorCarPos; //Synchronize upper transfer of position
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);			
}
/****************************************************
Function Name: Op_ResetConnection
*****************************************************
Descriptions:  Reset Connection module
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
void Op_ResetConnection(void)
{
	stOpCtrl *CurrentOp = &OpResetConnection;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			gConnectionPosition = STATUS_UNKNOW;
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if (input_get_one(SN_CONNECTION_ORG) == SENSOR_TYPE_BEAM_OFF) 	// Not origin	
			{
				if(input_get_one(SN_RWMODULE_ENTER_CARD) == SENSOR_TYPE_BEAM_ON) //Read write module detected have card
				{
					sm_run(SM_RWMODULE, DIR_SM_RWMODULE_OUT_CARD,100,2000); //Read wrtie module and connection together turn
				}
				sm_run(SM_CONNECTION, DIR_SM_CONNECTION_ORG,50,1000); //Backward move a distance
				CurrentOp->nStep = STEP4;
			}
			else //Origin
			{
				sm_run(SM_CONNECTION, DIR_SM_CONNECTION_CARD,50, 220); //Forward move a distance
				CurrentOp->nStep = STEP2;
			}			
		break;
		case STEP2:
			if(IsSmRunFinish(SM_CONNECTION))  
			{
				if (input_get_one(SN_CONNECTION_ORG) == SENSOR_TYPE_BEAM_OFF) //Leave origin
				{
					SetOpTimeDelay(CurrentOp,300);   //Reversal delay
					CurrentOp->nStep = STEP3;	
				}
				else
				{
					CurrentOp->nResult = ERROR_CONNECTION_LEAVE_ORIGIN_FAIL; //11026 衔接模块离开原点失败
				}	
			}
		break;																																																	
		case STEP3:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP1;
			}	
		break;
		case STEP4:
			if (input_get_one(SN_CONNECTION_ORG) == SENSOR_TYPE_BEAM_ON) //Go to origin
			{
				sm_stop(SM_CONNECTION); 	
				CurrentOp->nStep = STEP5;
			}	
			else if (IsSmRunFinish(SM_CONNECTION)) //Motor action finish
			{
				CurrentOp->nResult = ERROR_CONNECTION_RETURN_ORIGIN_FAIL; //10027 衔接模块返回原点失败
			}		
		break;
		case STEP5:
			sm_run(SM_CONNECTION,DIR_SM_CONNECTION_ORG,20,180);  //Continue move a distance
			CurrentOp->nStep = STEP6;
		break;
		case STEP6:
			if (IsSmRunFinish(SM_CONNECTION))
			{
				gConnectionPosition = STATUS_ORG; //Connection at origin
				CurrentOp->nResult  = 0xffff;	
			}
		break;

		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ConnectionSendCard
*****************************************************
Descriptions:  Connection module move to send card position
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
void Op_ConnectionSendCard(void)
{
	stOpCtrl *CurrentOp = &OpConnectionSendCard;
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
			if (gConnectionPosition == STATUS_POSITION)
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(gConnectionPosition == STATUS_UNKNOW) //Position unknow
			{
				StartChildOp(CurrentOp,&OpResetConnection); //Call reset connection subprogram 
				CurrentOp->nStep = STEP2;
			}	
			else
			{
				sm_run(SM_CONNECTION, DIR_SM_CONNECTION_CARD,20, parameter[PAR_CONNECTION_TO_SEND_CARD]); //Move to send card position
				CurrentOp->nStep = STEP3;				
			}
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp,&OpResetConnection) == true)
			{
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP3:
			if(IsSmRunFinish(SM_CONNECTION) == true)  
			{
				if (input_get_one(SN_CONNECTION_ORG) == SENSOR_TYPE_BEAM_OFF) //Leave origin
				{
					CurrentOp->nStep = STEP4;	
				}
				else        	
				{
					CurrentOp->nResult = ERROR_CONNECTION_TO_SENDCARD_LEAVE_ORIGIN_FAIL; //11028 衔接模块到发卡位离开原点失败
				}	
			}
		break;																																																	
		case STEP4:
			gConnectionPosition = STATUS_POSITION; // At send card position
			CurrentOp->nResult  = 0xffff;	
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ReceiveCardVacuumOn
*****************************************************
Descriptions: Receive card position vacuum on
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
void Op_ReceiveCardVacuumOn(void)
{
	stOpCtrl *CurrentOp = &OpReceiveCardVacuumOn;
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
			dm_ctrl_one(DM_RECEIVE_CARD, RELAY_ON);	//Receive card position vacuum open
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ReceiveCardVacuumOff
*****************************************************
Descriptions: Receive card position vacuum off
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
void Op_ReceiveCardVacuumOff(void)
{
	stOpCtrl *CurrentOp = &OpReceiveCardVacuumOff;
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
			dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF);	//Receive card position vacuum close
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_HeaterPowerOn
*****************************************************
Descriptions:  Heater power on
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
void Op_HeaterPowerOn(void)
{
	stOpCtrl *CurrentOp = &OpHeaterPowerOn;
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
			dm_ctrl_one(DM_HEATER_POWER, RELAY_ON);	//Heater power on
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_HeaterPowerOff
*****************************************************
Descriptions:  Heater power on
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
void Op_HeaterPowerOff(void)
{
	stOpCtrl *CurrentOp = &OpHeaterPowerOff;
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
			dm_ctrl_one(DM_HEATER_POWER, RELAY_OFF);	//Heater power off
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_RWModuleInCard
*****************************************************
Descriptions:  Read write module in card
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
void Op_RWModuleInCard(void)
{
	unsigned char count; //Retry timers
	stOpCtrl *CurrentOp = &OpRWModuleInCard;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			count = 0; 
			CurrentOp->nStep = STEP1;
		break;
		case STEP1: //Read write module forward run
			if(input_get_one(SN_RWMODULE_ENTER_CARD) == SENSOR_TYPE_BEAM_ON) //Enter card channel detected card
			{
				sm_run(SM_RWMODULE,DIR_SM_RWMODULE_IN_CARD,parameter[PAR_RWMODULE_SPEED],5000);
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nResult = ERROR_RWMODULE_NOT_CHECK_CARD;//11029 读写模块没有检测到卡片	
			}
		break;
		case STEP2:
			if (input_get_one(SN_CARD_EXIT) == SENSOR_TYPE_BEAM_ON) //Exit card position detected card 
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP5;
			}
			else if(IsSmRunFinish(SM_RWMODULE))
			{
				if (count < 3)
				{
					count++;
					CurrentOp->nStep = STEP3;
				}
				else
				{
					count = 0; //报错将count清零,没报错不用因为每次执行动作START那里会清零
					CurrentOp->nResult = ERROR_RWMODULE_NOT_IN_CARD;//10030 读写模块没有卡片进入		
				}
			}
		break;
		case STEP3: //Go to out card exit
			sm_run(SM_RWMODULE,DIR_SM_RWMODULE_OUT_CARD,parameter[PAR_RWMODULE_SPEED],5000);
			CurrentOp->nStep = STEP4;
		break;
		case STEP4: //Read write module enter card position detected card
			if (input_get_one(SN_RWMODULE_ENTER_CARD) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP1;
			}	
		break;
		case STEP5:
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_RWModuleOutCard
*****************************************************
Descriptions:  Read write module out card
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
void Op_RWModuleOutCard(void)
{
	unsigned char count;
	stOpCtrl *CurrentOp = &OpRWModuleOutCard;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			count = 0;
			CurrentOp->nStep = STEP1;
		break;
		case STEP1: 
			//Read write module out card
			sm_run(SM_RWMODULE,DIR_SM_RWMODULE_OUT_CARD,parameter[PAR_RWMODULE_SPEED],3000);
			CurrentOp->nStep = STEP2;
		break;
		case STEP2: //Read write module enter card position sensor detected have card
			if (input_get_one(SN_RWMODULE_ENTER_CARD) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP3;	
			}
		break;
		case STEP3: //if elevator at read write moule to elevator position
			if (gElevatorCarPosition == ENUM_RWMODULE_TO_ELEVATOR) //Continue go a distance
			{
				sm_run(SM_RWMODULE,DIR_SM_RWMODULE_OUT_CARD,parameter[PAR_RWMODULE_SPEED],3000);
				CurrentOp->nStep = STEP4;
			}
			else //Not at return success
			{
				CurrentOp->nStep = STEP5;
			}
			
		break;			
		case STEP4: //Card slot detected have card
			if (input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON)
			{
				sm_run(SM_RWMODULE,DIR_SM_RWMODULE_OUT_CARD,parameter[PAR_RWMODULE_SPEED],300);
				CurrentOp->nStep = STEP5;
			}	
			else if(IsSmRunFinish(SM_RWMODULE))
			{
				if (count < 3)
				{
					count++;
					CurrentOp->nStep = STEP3;
				}
				else
				{
					count = 0;
					CurrentOp->nResult = ERROR_CARD_SLOT_NOT_IN_CARD;//11031 卡槽没有卡片进入
				}	
			}
		break;
		case STEP5:
			if(IsSmRunFinish(SM_RWMODULE))
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
Function Name: Op_ElevatorSendCardToRWModule
*****************************************************
Descriptions:  Elevator send card to read write module
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
void Op_ElevatorSendCardToRWModule(void)
{
	stOpCtrl *CurrentOp = &OpElevatorSendCardToRWModule;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			CurrentOp->nStep = STEP1;
			dm_ctrl_one(DM_RECEIVE_CARD, RELAY_ON); //Receive card position vacuum open
		break;
		case STEP1:
			gRWCheckCar_Flag	 = true; //Elevator car bottom call
			gElevatorCarPosition = ENUM_ELEVATOR_TO_RWMODULE;  //Elevator car move read write module position
			StartChildOp(CurrentOp,&OpElevatorCarMove); //Elevator car move
			CurrentOp->nStep 	 = STEP2;			
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpElevatorCarMove))
			{
				if(input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_BEAM_ON) //Receive card position vacuum suck card success
				{
					StartChildOp(CurrentOp,&OpConnectionSendCard); //Connection module to send card position
					CurrentOp->nStep = STEP3;
				}
				else
				{
					CurrentOp->nResult = ERROR_RECEIVE_CARD_POSITION_NOT_CARD;//11032 电梯接卡位没有吸到卡
				} 
			}
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpConnectionSendCard))
			{
				dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF); //Receive card position vacuum close
				StartChildOp(CurrentOp,&OpRWModuleInCard); //Read write module in card
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpRWModuleInCard))
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
Function Name: Op_RWModuleSendCardToOCR
*****************************************************
Descriptions:  Read write module send card to OCR
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
void Op_RWModuleSendCardToOCR(void)
{
	stOpCtrl *CurrentOp = &OpRWModuleSendCardToOCR;
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
			if(gElevatorCarPosition != ENUM_RWMODULE_TO_ELEVATOR)
			{
				gRWCheckCar_Flag	 = true; //Elevator car bottom call
				gElevatorCarPosition = ENUM_RWMODULE_TO_ELEVATOR;  //Read write module to elevator position
				StartChildOp(CurrentOp,&OpElevatorCarMove); //Elevator car move
			}
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpElevatorCarMove) == true)
			{
				StartChildOp(CurrentOp,&OpConnectionSendCard); //Connection module to send card position
				CurrentOp->nStep = STEP3; 
			}
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpConnectionSendCard) == true)
			{
				StartChildOp(CurrentOp,&OpRWModuleOutCard); //Read write module Out card
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpRWModuleOutCard) == true)
			{
				CurrentOp->nStep = STEP5;	
			}			
		break;
		case STEP5:
			StartChildOp(CurrentOp,&OpResetConnection); //Connection reset
			CurrentOp->nStep = STEP6;
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpResetConnection) == true)
			{
				gRWCheckCar_Flag	 = true; //Elevator car bottom call
				gElevatorCarPosition = ENUM_OCR_CARD;  //Elevator car move OCR position
				StartChildOp(CurrentOp,&OpElevatorCarMove); //Elevator car move
				CurrentOp->nStep 	 = STEP7;					
			}
		break;
		case STEP7:
			if (IsChildOpOK(CurrentOp,&OpElevatorCarMove) == true)
			{
				StartChildOp(CurrentOp,&OpConnectionSendCard); //Connection move to send card position
				CurrentOp->nStep = STEP8;				
			}
		break;
		case STEP8:
			if (IsChildOpOK(CurrentOp,&OpConnectionSendCard) == true)
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
Function Name: Op_OCRToRWModuleOutCard
*****************************************************
Descriptions:  Read write module send card to OCR
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
void Op_OCRToRWModuleOutCard(void)
{
	stOpCtrl *CurrentOp = &OpOCRToRWModuleOutCard;
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
			StartChildOp(CurrentOp,&OpResetConnection); //Connection reset
			CurrentOp->nStep 	 = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpResetConnection) == true)
			{
				gRWCheckCar_Flag	 = true; //Elevator car bottom call
				gElevatorCarPosition = ENUM_RWMODULE_TO_ELEVATOR;  //Elevator car move read write module position
				StartChildOp(CurrentOp,&OpElevatorCarMove); //Elevator car move
				CurrentOp->nStep = STEP3; 
			}
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpElevatorCarMove) == true)
			{
				StartChildOp(CurrentOp,&OpConnectionSendCard); //Connection reset
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpConnectionSendCard) == true)
			{
				StartChildOp(CurrentOp,&OpRWModuleInCard);
				CurrentOp->nStep =STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp,&OpRWModuleInCard) == true)
			{
				sm_run(SM_RWMODULE,DIR_SM_RWMODULE_IN_CARD,50,parameter[PAR_OUT_CARD]); //Read write module out card
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if (IsSmRunFinish(SM_RWMODULE) == true)
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
Function Name: Op_AutoMakeCard
*****************************************************
Descriptions:  Auto make card
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
void Op_AutoMakeCard(void)
{
	stOpCtrl *CurrentOp = &OpAutoMakeCard;
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
			gRWCheckCar_Flag	 = true; //Elevator car bottom call
			gElevatorCarPosition = ENUM_RECEIVE_CARD;  //Elevator to receive card position
			StartChildOp(CurrentOp,&OpElevatorCarMove); //Elevator car move
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpElevatorCarMove) == true)
			{

				StartChildOp(CurrentOp,&OpCuttingPlatformWork);	//Start cut card process
				CurrentOp->nStep = STEP3;				
			}
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpCuttingPlatformWork) == true)
			{
				StartChildOp(CurrentOp,&OpElevatorSendCardToRWModule); //Elevator send card to read write module
				CurrentOp->nStep = STEP4;
			}	
		break;	
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpElevatorSendCardToRWModule) == true)
			{
				StartChildOp(CurrentOp,&OpRWModuleSendCardToOCR); //Read write module send card to OCR
				CurrentOp->nStep = STEP5;	
			}
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpRWModuleSendCardToOCR) == true)
			{
				StartChildOp(CurrentOp,&OpOCRToRWModuleOutCard);	//OCR to read write module out card position
				CurrentOp->nStep = STEP6;
			}	
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpOCRToRWModuleOutCard) == true)
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
	Op_ResetModule();					//1 复位模块
	Op_ResetClampCardCar(); 			//2 复位夹卡小车通道
	Op_ClampCardCarMove(); 				//3 夹卡小车移动
	Op_ResetClamp();					//4 爪子复位
	Op_ClampOpen();						//5 爪子张开
	Op_ClampClose();					//6 爪子闭合
	Op_RightClampClose();			    //7 右侧夹子关闭 			
  	Op_RightClampOpen();				//8 右侧夹子打开          
	Op_LeftClampClose();				//9 左侧夹子关闭
	Op_LeftClampOpen();					//10 左侧夹子打开
	Op_OpeningPressure();				//11 压力开
	Op_ClosingPressure();				//12 压力关
	Op_WarmRise();						//13 加热下降
	Op_WarmDown();						//14 加热升起
	Op_CoolRise();						//15 冷却下降
	Op_CoolDown();						//16 冷却升起
	Op_PresRise();						//17 冲压下降
	Op_PresDown();						//18 冲压升起
	Op_PretargetingReset();				//19 预定位复位
	Op_PretargetingUp();				//20 预定位上升
	Op_WasteStorageReset();		 		//21 废料场复位
	Op_WasteStorageDown();	   			//22 废料场下降
	Op_PretargetingVacuumCupOpen();		//23 预定位真空吸打
	Op_PretargetingVacuumCupClose();	//24 预定位真空吸关闭
	Op_OpenGasSwitch();			  		//27 打开总气开关
	Op_CloseGasSwitch();				//28 关闭总气开关
	Op_WarmCoolMakeCard();				//29 冷热制卡
	Op_GotoLoadingPlatform();			//30 去装料平台	
	Op_CuttingPlatformWork();			//31 裁剪平台工作
	Op_ScramProcess();					//32 紧急停止制剪卡流程
	Op_ResetElevatorCar();				//31 复位读写检测下车 
	Op_ElevatorCarMove();				//32 电梯小车移动
	Op_ResetConnection();				//33 复位衔接模块
	Op_ConnectionSendCard();			//34 衔接模块移动到发卡位
	Op_ReceiveCardVacuumOn();			//35 接卡位真空开
	Op_ReceiveCardVacuumOff();			//36 接卡位真空关
	Op_HeaterPowerOn();					//37 发热管电源开
	Op_HeaterPowerOff();				//38 发热管电源关
	Op_RWModuleInCard();				//39 读写模块进卡
	Op_RWModuleOutCard();				//40 读写模块出卡
	Op_ElevatorSendCardToRWModule();	//41 电梯发卡到读写模块
	Op_RWModuleSendCardToOCR();			//42 读写模块发卡到OCR
	Op_OCRToRWModuleOutCard();			//43 OCR到读写模块出卡


	Op_AutoMakeCard();					//44 自动制卡
}






