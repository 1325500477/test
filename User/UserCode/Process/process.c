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
stOpCtrl OpLiftUpDnReset;						//30 电梯升降复位
//电梯模块
stOpCtrl OpResetLiftModule;						//31 复位电梯模块
stOpCtrl OpLiftUpDnMove;						//32 电梯小车移动
stOpCtrl OpLiftHorizonReset;					//33 电梯小车水平复位
stOpCtrl OpLiftHorizonMove;						//34 衔接模块移动
stOpCtrl OpReceiveCardVacuumOn;					//35 接卡位真空开
stOpCtrl OpReceiveCardVacuumOff;				//36 接卡位真空关
stOpCtrl OpHeaterPowerOn;						//37 发热管电源开
stOpCtrl OpHeaterPowerOff;						//38 发热管电源关
stOpCtrl OpRWChannelInCardFromLiftSide;			//39 电梯向读写通道进卡
stOpCtrl OpRWChannelOutCardToLiftSide;			//40 读写通道向电梯出卡
//电梯组合流程
stOpCtrl OpLiftSuckSendCardToRWChannel;			//41 电梯发卡到读写模块
stOpCtrl OpRWModuleSendCardToOCR;				//42 读写模块发卡到OCR
stOpCtrl OpOCRToRWModuleOutCard;				//43 OCR到读写模块出卡
stOpCtrl OpCutSendCardToRWModule;				//44 发卡到读写模块
stOpCtrl OpPerfectCardEnter;					//45 好卡进卡
stOpCtrl OpPerfectCardExit;						//46 好卡出卡
//以下新增
stOpCtrl OpLiftSlotSendCardToRWChannel;			//47 电梯槽发卡到读写器通道
stOpCtrl OpRWChannelSendCardToLiftSlot;			//48 读写通道发卡到电梯卡槽
stOpCtrl OpLiftSlotSendCardToRecycleBox;		//49 电梯卡槽送卡到回收箱
stOpCtrl OpRWModuleReset;						//50 读写模块复位

unsigned char gStatusPretarget 	  = STATUS_UNKNOW;	//预定位位置状态
unsigned char gStatusWastestorage = STATUS_UNKNOW;  //废料位置状态
unsigned char bFlagReset 		  = FALSE;        	//复位标志位
unsigned char gGoToPretarget_flag = 0;				//防止每次夹卡小车在原地运行时预定位也复位和升起
unsigned char gWarmDown_flag      = 0;				//热压下降标志
unsigned char gCoolDown_flag	  = 0;				//冷压下降标志
/******** 其它定义*********************************/
stOpCtrl *gMachineOperation[80]; 					//设置一个支持50个流程的指针数组
unsigned char gnOperationNo = 1; 					//执行的动作序号（从1开始对各个动作结构体变量逐个初始化）
extern unsigned char cParamBuf[];					//接收参数
//夹卡小车
unsigned char gTrolleyCarMove_Flag 	 = false;  		//夹卡小车运行标志
unsigned char gTrolleyCarPosition  	 = ENUM_UNKNOW; //夹卡小车的位置
unsigned int gTrolleyCarMoveCount 	 = 0;           //小车移动次数,达到一定次数复位消除累积误差
unsigned int gAbsPosTrolleyCarMov[6] = { 			//抓卡小车移动各个工位的绝对步数初始值
	0, 	 	//空余
	13, 	//1热压位 
	165,	//2冷压位
	311,	//3预定位
	449,	//4剪卡位
	580,	//5废料位
 };	
//电梯小车
unsigned char flag_LiftUpDnMove	    = false; 				 //电梯小车运行标志位
unsigned char gLiftUpDnPosition  = ENUM_LIFT_UP_DN_UNKNOW;   //电梯小车位置
unsigned int gAbsLiftUpDnMove[6] = { 					 	 //电梯小车移动参数
	0,	  //空余
	245,  //1接卡位
	41,   //2电梯吸卡到读写通道	
	195,  //3电梯槽对读写通道
	176,  //4电梯槽对回收箱
	12,   //5电梯槽到OCR
}; 
//电梯小车水平移动 
unsigned char flag_Lift_H_MOVE	= false; 		 		  		 //电梯小车水平运行标志位
unsigned char gLiftHorizonPosition   = ENUM_LIFT_HORIZON_UNKNOW; //电梯小车水平位置
unsigned int  gAbsLiftHorizonMove[4] = { 			 		 	 //电梯小车水平移动参数
	0,	  //空余
	470,  //1接送卡位
	600,  //2进OCR位
	280,  //3出OCR位
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
		//12 接卡位
		//13 电梯到读写模块
		//14 读写模块到电梯
		//15 废卡位
		//16 OCR检测位 	 		
		gAbsLiftUpDnMove[i+1] = parameter[i+11];
		//17 电梯水平接送卡位置 			   		   
		//18 电梯水平进卡到OCR  
		//19 OCR出卡到电梯水平		
		if(i<3)
		{
			gAbsLiftHorizonMove[i+1] = parameter[i+16];
		}
	} 
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
	gMachineOperation[gnOperationNo++] = &OpResetModule;					//1 复位
	gMachineOperation[gnOperationNo++] = &OpResetClampCardCar; 				//2 复位夹卡小车
	gMachineOperation[gnOperationNo++] = &OpClampCardCarMove;   			//3 夹卡小车移动位置
	gMachineOperation[gnOperationNo++] = &OpResetClamp;						//4 复位爪子
	gMachineOperation[gnOperationNo++] = &OpClampOpen; 						//5 爪子张开
	gMachineOperation[gnOperationNo++] = &OpClampClose; 					//6 爪子闭合
	gMachineOperation[gnOperationNo++] = &OpRightClampClose; 				//7 右侧爪子闭合
	gMachineOperation[gnOperationNo++] = &OpRightClampOpen; 				//8 右侧爪子张开
	gMachineOperation[gnOperationNo++] = &OpLeftClampClose; 				//9 左侧爪子闭合
	gMachineOperation[gnOperationNo++] = &OpLeftClampOpen; 					//10 左侧爪子张开
	gMachineOperation[gnOperationNo++] = &OpWarmRise;				 		//13 热压上升
	gMachineOperation[gnOperationNo++] = &OpWarmDown;				 		//14 热压下降
	gMachineOperation[gnOperationNo++] = &OpCoolDown;						//15 冷却下降
	gMachineOperation[gnOperationNo++] = &OpCoolRise;						//16 冷却升起
	gMachineOperation[gnOperationNo++] = &OpPresDown;						//17 冲压下降
	gMachineOperation[gnOperationNo++] = &OpPresRise;						//18 冲压升起
	gMachineOperation[gnOperationNo++] = &OpPretargetingReset;          	//19 预定位复位
	gMachineOperation[gnOperationNo++] = &OpPretargetingUp;             	//20 预定位上升
	gMachineOperation[gnOperationNo++] = &OpWasteStorageReset;				//21 废料场电机复位
	gMachineOperation[gnOperationNo++] = &OpWasteStorageDown;	  			//22 废料场电机下降
	gMachineOperation[gnOperationNo++] = &OpPretargetingVacuumCupOpen;		//23 预定位真空打开
 	gMachineOperation[gnOperationNo++] = &OpPretargetingVacuumCupClose;		//24 预定位真空关闭
	gMachineOperation[gnOperationNo++] = &OpOpenGasSwitch;					//25 总气打开	
	gMachineOperation[gnOperationNo++] = &OpCloseGasSwitch;					//26 总气关闭
	//组合动作	
	gMachineOperation[gnOperationNo++] = &OpWarmCoolMakeCard;				//27 热冷压制卡	
	gMachineOperation[gnOperationNo++] = &OpGotoLoadingPlatform;			//28 到预定位+预定位上升+打开夹子
	gMachineOperation[gnOperationNo++] = &OpCuttingPlatformWork;			//29 剪卡+废料				 
	gMachineOperation[gnOperationNo++] = &OpLiftUpDnReset;					//30 电梯升降复位
	//电梯模块	
	gMachineOperation[gnOperationNo++] = &OpResetLiftModule;				//31 复位电梯小车
	gMachineOperation[gnOperationNo++] = &OpLiftUpDnMove;					//32 电梯小车移动
	gMachineOperation[gnOperationNo++] = &OpLiftHorizonReset;				//33 电梯小车水平复位
	gMachineOperation[gnOperationNo++] = &OpLiftHorizonMove;				//34 衔接模块移动
	gMachineOperation[gnOperationNo++] = &OpReceiveCardVacuumOn;			//35 接卡位负压开
	gMachineOperation[gnOperationNo++] = &OpReceiveCardVacuumOff;			//36 接卡位负压关
	gMachineOperation[gnOperationNo++] = &OpHeaterPowerOn;					//37 发热管电源开
	gMachineOperation[gnOperationNo++] = &OpHeaterPowerOff;					//38 发热管电源关
	gMachineOperation[gnOperationNo++] = &OpRWChannelInCardFromLiftSide;	//39 读写模块进卡
	gMachineOperation[gnOperationNo++] = &OpRWChannelOutCardToLiftSide;		//40 读写模块出卡
	gMachineOperation[gnOperationNo++] = &OpLiftSuckSendCardToRWChannel;	//41 电梯发卡到读写模块
	gMachineOperation[gnOperationNo++] = &OpRWModuleSendCardToOCR;			//42 读写模块发卡到OCR
	gMachineOperation[gnOperationNo++] = &OpOCRToRWModuleOutCard;			//43 OCR到读写模块出卡
	gMachineOperation[gnOperationNo++] = &OpCutSendCardToRWModule;			//44 剪发卡到读写模块
	gMachineOperation[gnOperationNo++] = &OpPerfectCardEnter;				//45 好卡进卡
	gMachineOperation[gnOperationNo++] = &OpPerfectCardExit;				//46 好卡出卡
	gMachineOperation[gnOperationNo++] = &OpLiftSlotSendCardToRWChannel;	//47 电梯槽发卡到读写器通道
	gMachineOperation[gnOperationNo++] = &OpRWChannelSendCardToLiftSlot;	//48 读写通道发卡到电梯卡槽
	gMachineOperation[gnOperationNo++] = &OpLiftSlotSendCardToRecycleBox;	//49 电梯卡槽送卡到回收箱
	gMachineOperation[gnOperationNo++] = &OpRWModuleReset;					//50 读写模块复位

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
			gStatusPretarget = STATUS_UNKNOW;				//Pretargeting status unknow
			gStatusWastestorage = STATUS_UNKNOW;			//Waste status unknow
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);		//Gas open
			dm_ctrl_one(DM_HEATER_POWER, RELAY_ON);		  	//Heater power on	 
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpPretargetingReset);  //Pretargeting reset
			CurrentOp->nStep = STEP4;
	   	break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset)) //Pretargeting reset finish
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //Waste reset 
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
				CurrentOp->nStep = STEP11;	
			}
		break;
		case STEP11:
			gTrolleyCarMove_Flag = true;  					//Car run flag
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING; 
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//Go back pretargeting
			CurrentOp->nStep = STEP12;		
		break;
		case STEP12:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);	//Pretargeting Up
				CurrentOp->nStep = STEP13;
			}
		break;
		case STEP13:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp))
			{
				StartChildOp(CurrentOp, &OpClampOpen); 		//Clamp Open
				CurrentOp->nStep = STEP14;
			}
		break;
		case STEP14:
			if(IsChildOpOK(CurrentOp, &OpClampOpen))
			{
			 	CurrentOp->nStep = STEP15;
			}
		break;
		case STEP15:
			StartChildOp(CurrentOp, &OpRWModuleReset); 		//Read write module reset
			CurrentOp->nStep = STEP16;
		break;
 		case STEP16:
			if(IsChildOpOK(CurrentOp, &OpRWModuleReset))
			{
			 	CurrentOp->nStep = STEP17;
			}
		break;
		case STEP17:
			if((input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON)
			&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON))
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_AND_LIFT_CARD_SLOT_ALL_HAVE_CARD;	//ERROR:下控制板复位时检测到读写通道与电梯卡槽同时有卡
			}
			else 
			{
				dm_ctrl_one(DM_RECEIVE_CARD, RELAY_ON);		//接卡真空吸打开
				SetOpTimeDelay(CurrentOp,2000);
				CurrentOp->nStep = STEP18;
			}
		break;
		case STEP18:
			if (CheckOpTimeDelay(CurrentOp))
			{
				if((input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_VACUUM_ON)     //Lift car suck of card
				&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON)) //Exit have card
				{
					dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF);		//接卡真空吸关闭
					CurrentOp->nResult = ERROR_RWCHANNEL_AND_LIFT_RECEIVE_ALL_HAVE_CARD; //ERROR:下控制板复位时检测到读写通道与电梯接卡吸盘处都有卡
				}
				else 
				{
					CurrentOp->nStep = STEP19;
				}
			}
		break;
		case STEP19:
			if((input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_VACUUM_ON) //Lift car suck of card
			&&(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON))   //Card slot have card
			{
				dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF);		//接卡真空吸关闭
				CurrentOp->nResult = ERROR_CARD_SLOT_AND_LIFT_RECEIVE_ALL_HAVE_CARD; 	//ERROR:下控制板复位时检测到电梯卡槽与电梯接卡吸盘处都有卡
			}
			else 
			{
				CurrentOp->nStep = STEP20;
			}
		break;
		case STEP20:
			if(input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_VACUUM_OFF)		
			{
				dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF);		//接卡真空吸关闭	
			}
			StartChildOp(CurrentOp, &OpResetLiftModule);  	//Lift module reset
			CurrentOp->nStep = STEP21;	
		break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpResetLiftModule))
			{
				CurrentOp->nStep = STEP22;	
			}
		break;
		case STEP22:
			if(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON)		//电梯卡槽有卡
			{
				CurrentOp->nStep = STEP23;
			}
			else
			{
				CurrentOp->nStep = STEP25;
			}
		break;
		case STEP23:
			StartChildOp(CurrentOp, &OpLiftSlotSendCardToRecycleBox);
			CurrentOp->nStep = STEP24; 
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpLiftSlotSendCardToRecycleBox))
			{
				CurrentOp->nStep = STEP25;	
			}
		break;
		case STEP25:
			if(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON)		//Read write channel have card
			{
				CurrentOp->nStep = STEP26;
			}
			else
			{
				CurrentOp->nStep = STEP28;
			}
		break;
		case STEP26:
			StartChildOp(CurrentOp, &OpRWChannelSendCardToLiftSlot);
			CurrentOp->nStep = STEP27; 
		break;
		case STEP27:
			if(IsChildOpOK(CurrentOp, &OpRWChannelSendCardToLiftSlot))
			{
				CurrentOp->nStep = STEP23;	
			}
		break;
		case STEP28:
			if(input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_VACUUM_ON)		//Lift card suck of card
			{
				CurrentOp->nStep = STEP29;
			}
			else
			{
				CurrentOp->nStep = STEP31;
			}
		break;
		case STEP29:
			StartChildOp(CurrentOp, &OpLiftSuckSendCardToRWChannel);
			CurrentOp->nStep = STEP30; 
		break;
		case STEP30:
			if(IsChildOpOK(CurrentOp, &OpLiftSuckSendCardToRWChannel))
			{
				CurrentOp->nStep = STEP26;	
			}
		break;
		case STEP31:
			gGoToPretarget_flag = 0;
			bFlagReset = TRUE;  //Reset flag
			dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF);		//接卡真空吸关闭
			//dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_OFF);	//总气压关闭
			CurrentOp->nResult = 0xffff;
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
			if(gStatusPretarget == STATUS_ORG)
			{
				CurrentOp->nStep = STEP3;	
			}
			else
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);     //Pretargeting reset
				CurrentOp->nStep = STEP2;				
			}
	   	break;
		case STEP2:
		   	if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{
				CurrentOp->nStep = STEP3;
			}
	   	break;
		case STEP3:
			if(gStatusWastestorage == STATUS_ORG)
			{
				CurrentOp->nStep = STEP5;	
			}
			else
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //Waste Reset
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				StartChildOp(CurrentOp, &OpPresRise);  	//Cut reset
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpPresRise))
			{
				StartChildOp(CurrentOp, &OpCoolRise);  	//Cool Rise
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpCoolRise))
			{
				StartChildOp(CurrentOp, &OpWarmRise);  	//Warm Rise
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpWarmRise))
			{
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) 	//Not origin	
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 100,PULSE_TO_MILLIMETER(1500)); //Backward run a distance
				CurrentOp->nStep = STEP11;
			}
			else  //在原点
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_FORWARD, 100, PULSE_TO_MILLIMETER(40)); //Forward run a distance
				CurrentOp->nStep = STEP9;
			}			
		break;
		case STEP9:
			if(IsSmRunFinish(SM_CHANNEL))  
			{
				if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //Leave origin
				{
					SetOpTimeDelay(CurrentOp,300);   //Reverse run delay
					CurrentOp->nStep = STEP10;	
				}
				else       	
				{
					CurrentOp->nResult = ERROR_CHANNAL_LEAVE_ORG_FAIL;   //ERROR:离开原点传感器失败
				}	
			}
		break;																																																	
		case STEP10:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP8;
			}	
		break;
		case STEP11:
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //At the origin
			{
				sm_stop(SM_CHANNEL); 	//Stop Motor
				CurrentOp->nStep = STEP12;
			}	
			else if (IsSmRunFinish(SM_CHANNEL)) //Motor action finish
			{
				CurrentOp->nResult = ERROR_CHANNAL_RETURN_ORG_FAIL;	//ERROR:通道返回传感器失败			
			}		
		break;
		case STEP12:
			sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 100,PULSE_TO_MILLIMETER(1)); //Continue backward run a distance
			CurrentOp->nStep = STEP13;
		break;
		case STEP13:
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
			if (1 == gWarmDown_flag)	 //At the target position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_OFF) //Not at warm rise position
			{
				StartChildOp(CurrentOp,&OpWarmRise);	
				CurrentOp->nStep = STEP1;
			}
			else
			{
				sm_run(SM_WARM_PRESS,DIR_SM_PRESS_DN,PAR_WARMCOOL_PRESS_SPEED,parameter[PAR_WARM_DN_STEP]); //Run to target position 
				CurrentOp->nStep = STEP2;
			}
		break;	
		case STEP1:
			if (IsChildOpOK(CurrentOp,&OpWarmRise))
			{
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if (IsSmRunFinish(SM_WARM_PRESS)) 
			{
				if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)   //At the warm rise position
				{
					CurrentOp->nResult = ERROR_WARMING_DOWN_FAIL; //ERROR:加热下降失败
				}
				else
				{
					CurrentOp->nStep = STEP3;	
				}
			} 
		break;
		case STEP3:
			gWarmDown_flag     = 1;	//Warm down flag
			CurrentOp->nResult = 0xffff;
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
			if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)	 //Origin position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else   
			{
				sm_run(SM_WARM_PRESS,DIR_SM_PRESS_UP,PAR_WARMCOOL_PRESS_SPEED,5000); //Run to origin position 
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON) //To the origin
			{
				sm_stop(SM_WARM_PRESS);
				CurrentOp->nStep = STEP2;
			} 
			else if (IsSmRunFinish(SM_WARM_PRESS))
			{
				CurrentOp->nResult = ERROR_WARMING_RISE_FAIL; //ERROR:加热升起失败
			}
		break;
		case STEP2:
			sm_run(SM_WARM_PRESS,DIR_SM_PRESS_UP,PAR_WARMCOOL_PRESS_SPEED,10); //compensation steps
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:
			if (IsSmRunFinish(SM_WARM_PRESS))
			{
				gWarmDown_flag = 0;		//Warm down flag 
				CurrentOp->nResult = 0xffff;
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
			if (1 == gCoolDown_flag)	 //At the target position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_OFF) //Not at cool rise position
			{
				StartChildOp(CurrentOp,&OpCoolRise);	
				CurrentOp->nStep = STEP1;
			}
			else
			{
				sm_run(SM_COOL_PRESS,DIR_SM_PRESS_DN,PAR_WARMCOOL_PRESS_SPEED,parameter[PAR_COOL_DN_STEP]); //Run to target position 
				CurrentOp->nStep = STEP2;
			}
		break;	
		case STEP1:
			if (IsChildOpOK(CurrentOp,&OpCoolRise))
			{
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if (IsSmRunFinish(SM_COOL_PRESS)) 
			{
				if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON) //At the cool rise position
				{
					CurrentOp->nResult = ERROR_COOLING_DOWN_FAIL; //ERROR:冷却下降失败
				}
				else
				{
					CurrentOp->nStep = STEP3;	
				}
			} 
		break;
		case STEP3:
			gCoolDown_flag     = 1;				//Cool down flag
			CurrentOp->nResult = 0xffff;
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
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON)	 //Origin position
			{
				CurrentOp->nResult = 0xffff;
			} 
			else   
			{
				sm_run(SM_COOL_PRESS,DIR_SM_PRESS_UP,PAR_WARMCOOL_PRESS_SPEED,5000); //Run to origin position 
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON) //To the origin
			{
				sm_stop(SM_COOL_PRESS);
				CurrentOp->nStep = STEP2;
			} 
			else if (IsSmRunFinish(SM_COOL_PRESS))
			{
				CurrentOp->nResult = ERROR_COOLING_RISE_FAIL;  //ERROR:冷却升起失败
			}
		break;
		case STEP2:
			sm_run(SM_COOL_PRESS,DIR_SM_PRESS_UP,PAR_WARMCOOL_PRESS_SPEED,10); //compensation steps
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:
			if (IsSmRunFinish(SM_COOL_PRESS))
			{
				gCoolDown_flag = 0;		//Cool down flag 
				CurrentOp->nResult = 0xffff;
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
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON)   //At the down
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_PUNCH,DIR_DM_PUNCH_DN); //Motor run
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON) //Run to down
			{
				dm_ctrl_one(DM_PUNCH,DM_BRAKE); //Stop motor
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_PRESS_DOWN_FAIL;	 //ERROR:冲压下降失败
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
				dm_ctrl_one(DM_PUNCH,DIR_DM_PUNCH_UP); //Motor run
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON) //At rise position
			{
				dm_ctrl_one(DM_PUNCH,DM_BRAKE); //Stop motor
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nResult = ERROR_PRESS_RISE_FAIL;	 //ERROR:冲压升起失败
			}
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
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_CLOSE_FAIL;	 //ERROR:右边夹子关闭失败
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
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_OPEN_FAIL;	//ERROR:右边夹子打开失败
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
				CurrentOp->nResult = ERROR_LEFT_CLAMP_CLOSE_FAIL;	//ERROR:左边夹子关闭失败
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
				CurrentOp->nResult = ERROR_LEFT_CLAMP_OPEN_FAIL; //ERROR:左边传感器故障导致夹子打开失败
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
Descriptions:  Clamp card car move
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
				CurrentOp->nResult = ERROR_TROLLEY_CAR_POSITION_PARA_MAX; //ERROR:抓卡小车上层传输的数据超出接收最大值
			}	

			if (ChannelTrolleyCarPos != ENUM_INSIDE_PRETARGETING) //
			{
				gGoToPretarget_flag = 0;
			}

			CurrentOp->nStep = STEP1;					
		break;
	/////////////////////////////////////////////////////////////////////////////
	/*增加小车移动时候的保护,小车移动时如果不把预定位(撕膜位)下降和废料位上升,
	冷压热压剪卡升起,夹子闭合,小车移动时会撞到导致电机失歩及皮带对不准工位  */	
		case STEP1:
			if(gStatusPretarget == STATUS_ORG) //Pretargeting position orgin
			{
				CurrentOp->nStep = STEP3;				
			}
			else
			{
				if(1 == gGoToPretarget_flag)
				{
					CurrentOp->nStep = STEP3;	
				}
				else
				{
					StartChildOp(CurrentOp, &OpPretargetingReset);  //Pretargeting reset
					CurrentOp->nStep = STEP2;					
				}
			}		
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (gStatusWastestorage == STATUS_ORG) //Waster position unknow
			{
				CurrentOp->nStep = STEP5;			
			}
			else
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //Waste reset
				CurrentOp->nStep = STEP4;					
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			StartChildOp(CurrentOp, &OpPresRise);  		//Cut reset
			CurrentOp->nStep = STEP6;
		break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpPresRise))
			{
				StartChildOp(CurrentOp, &OpCoolRise);  		//Cool reset
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpCoolRise))
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//Warm reset
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if(IsChildOpOK(CurrentOp, &OpWarmRise))
			{
				CurrentOp->nStep = STEP9;
			}
		break;
	/////////////////////////////////////////////////////////////////////////////////////
		case STEP9:
			if (gTrolleyCarPosition == ENUM_UNKNOW) //Position unknow
			{				
				CurrentOp->nStep = STEP10;
			}
			else
			{
				CurrentOp->nStep = STEP12;
			}             	
		break;
		case STEP10:
			StartChildOp(CurrentOp,&OpResetClampCardCar); //Reset car		
			CurrentOp->nStep = STEP11;	
		break;
		case STEP11:
			if (IsChildOpOK(CurrentOp,&OpResetClampCardCar)) 
			{
				CurrentOp->nStep = STEP12;	
			}
		break;
		case STEP12:
			if(gTrolleyCarMoveCount<100) //运行到一定次数后复位清除累积误差
			{
				gTrolleyCarMoveCount++;
				CurrentOp->nStep = STEP13;
			}
			else
			{
				CurrentOp->nStep = STEP10;
				gTrolleyCarMoveCount = 0;
			}
		break;
		case STEP13:   //According Upper transfer of position go absolute step
			sm_run_abs(SM_CHANNEL,parameter[PAR_SM_CHANNAL_SPEED],PULSE_TO_MILLIMETER(gAbsPosTrolleyCarMov[ChannelTrolleyCarPos]));	
			CurrentOp->nStep= STEP14;
		break;
		case STEP14:
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
				CurrentOp->nResult = ERROR_PRETARGETING_RETURN_ORIGIN_FAIL;	  //ERROR:预定位返回原点失败
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_PRETARGETING))
			{	
				dm_ctrl_one(DM_PRETARGETING_VACUUMCUP, RELAY_OFF);			//Pretargeting vacuum close
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
					CurrentOp->nResult = ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL; //ERROR:预定位离开原点失败
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
					CurrentOp->nResult = ERROR_PRETARGETING_UP_LEAVE_ORIGIN_FAIL;//ERROR:预定位升起时离开原点失败
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
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON)	 //Sersor check At origin
			{
				CurrentOp->nStep = STEP3;
			} 
			else
			{
				gStatusWastestorage = STATUS_UNKNOW;    					 //Reset flag
				dm_ctl_one(DM_PUNCH,DIR_DM_WASTER_UP);   
				SetOpTimeDelay(CurrentOp,500);
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON) //Run to origin
			{
				SetOpTimeDelay(CurrentOp,10);
				CurrentOp->nStep = STEP2;
			} 
			else if(CheckOpTimeDelay(CurrentOp)) //Time out 
			{
				CurrentOp->nResult = ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL;	 //ERROR:废卡位返回原点失败
			}
		break;
		case STEP2:
			if(CheckOpTimeDelay(CurrentOp)) //Compensation steps
			{
				gStatusWastestorage = STATUS_ORG;  //At origin
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP3:
			dm_ctl_one(DM_PUNCH,DIR_DM_WASTER_DN);   
			SetOpTimeDelay(CurrentOp,80);
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if(CheckOpTimeDelay(CurrentOp))
			{
				if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_OFF)
				{	//Sensor check leave origin
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL; //ERROR:废卡位离开原点失败
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
			dm_ctl_one(DM_PUNCH,DIR_DM_WASTER_DN);   
			SetOpTimeDelay(CurrentOp,200);
			CurrentOp->nStep = STEP3; 
		break;
		case STEP3:
			if(CheckOpTimeDelay(CurrentOp))
			{
				if(input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_OFF)
				{
					gStatusWastestorage = STATUS_POSITION; //At down position
					CurrentOp->nResult = 0xffff;
				}
				else
				{
					gStatusWastestorage = STATUS_UNKNOW;
					CurrentOp->nResult = ERROR_WASTESTORAGE_DOWN_LEAVE_ORIGIN_FAIL;//ERROR:废卡位下降离开原点失败 
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
Function Name:Op_WarmCoolMakeCard
*****************************************************
Descriptions: Warm Cool Make card
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
				CurrentOp->nResult = ERROR_PRETARGETING_NONE_CARD;	//ERROR:预定位无卡
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
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_OFF);			//Gas off	
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
				CurrentOp->nResult = ERROR_PRETARGETING_FROM_COOL_NONE_CARD; //ERROR:预定位到冷压位无卡
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
				CurrentOp->nResult = ERROR_WARM_FROM_COOL_NONE_CARD; //ERROR:热压位到冷压位无卡
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
				StartChildOp(CurrentOp, &OpClampCardCarMove);   //go back Pretargeting
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
				CurrentOp->nResult = ERROR_COOL_FROM_PRETARGETING_NONE_CARD;//ERROR:冷压位到预定位无卡
			}	
		break;
		case STEP20:
			if(IsChildOpOK(CurrentOp, &OpClampOpen))
			{
				dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);			//Gas on
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
			gTrolleyCarMove_Flag = true;  					//Car run flag
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING; 
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//Go back pretargeting
			CurrentOp->nStep = STEP3;		
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);	//Pretargeting Up
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp))
			{
				StartChildOp(CurrentOp, &OpClampOpen); 		//Clamp Open
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpClampOpen))
			{
				dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);	  //Gas relay open
				gGoToPretarget_flag = 1;
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
				dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);	//Gas on
				CurrentOp->nStep = STEP1;					
			}
			else
			{
				CurrentOp->nResult = ERROR_PRETARGETING_FROM_WASTE_NONE_CARD;//ERROR:预定位到废料位无卡
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
			gTrolleyCarPosition = ENUM_INSIDE_CUT_CARD; 	//Cut card position
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//Move to Cut card
			CurrentOp->nStep = STEP9;	
		break;
		case STEP9:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				SetOpTimeDelay(CurrentOp, 300);
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
				dm_ctrl_one(DM_RECEIVE_CARD, RELAY_ON);  //Receive card position vacuum open
				SetOpTimeDelay(CurrentOp, 300);
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
				if (input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_VACUUM_OFF) //Lift receive card position no card
				{
					dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF); 		   		  //Receive card position vacuum close
				}
				CurrentOp->nStep = STEP14;		
			}
		break;
		case STEP14:
			gTrolleyCarMove_Flag = true; 
			gTrolleyCarPosition = ENUM_INSIDE_WASTE; 	 //Waste
			StartChildOp(CurrentOp, &OpClampCardCarMove);//move to waste
			CurrentOp->nStep = STEP15;	
		break;
		case STEP15:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				SetOpTimeDelay(CurrentOp, 100);			
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
				SetOpTimeDelay(CurrentOp, 300);
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
				StartChildOp(CurrentOp, &OpWasteStorageDown);	//Waste platform down	
				CurrentOp->nStep = STEP21;
			}
		break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageDown))
			{
				SetOpTimeDelay(CurrentOp, 300);
				CurrentOp->nStep = STEP22;
			}
		break;
		case STEP22:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp,&OpWasteStorageReset); //Waste rise
				CurrentOp->nStep = STEP23;		
			}
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset))
			{
				CurrentOp->nStep = STEP24;
			}
		break;
		case STEP24:
			gTrolleyCarMove_Flag = true; 					 
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING;  //Pretargeting
			StartChildOp(CurrentOp, &OpClampCardCarMove);	 //Go back pretargeting
			CurrentOp->nStep = STEP25;		
		break;
		case STEP25:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove))
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);//Pretargeting rise
				CurrentOp->nStep = STEP26;
			}
		break;
		case STEP26:
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
Function Name: Op_LiftUpDnReset
*****************************************************
Descriptions: Lift car vertical reset
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
void Op_LiftUpDnReset(void)
{
	stOpCtrl *CurrentOp = &OpLiftUpDnReset;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}

	switch (CurrentOp->nStep)
	{
		case START:
			gLiftUpDnPosition = ENUM_LIFT_UP_DN_UNKNOW;
			CurrentOp->nStep = STEP1;
		break;

		case STEP1:
			if (input_get_one(SN_LIFT_UP_DN_ORG) == SENSOR_TYPE_BEAM_OFF) 	// Not origin	
			{
				sm_run(SM_LIFT_UP_DN, DIR_SM_LIFT_DN,100,PULSE_TO_MILLIMETER(334)); //Go back run a distance
				CurrentOp->nStep = STEP6;
			}
			else //Origin
			{
				sm_run(SM_LIFT_UP_DN, DIR_SM_LIFT_UP,100, PULSE_TO_MILLIMETER(13)); //Go forward run a distance
				CurrentOp->nStep = STEP4;
			}			
		break;
		case STEP4:
			if(IsSmRunFinish(SM_LIFT_UP_DN))  
			{
				if (input_get_one(SN_LIFT_UP_DN_ORG) == SENSOR_TYPE_BEAM_OFF) //Leave origin
				{
					SetOpTimeDelay(CurrentOp,300);   //Reversal delay
					CurrentOp->nStep = STEP5;	
				}
				else        	
				{
					CurrentOp->nResult = ERROR_LIFT_CAR_LEAVE_ORIGIN_FAIL; //ERROR:电梯升降离开原点失败
				}	
			}
		break;																																																	
		case STEP5:
			if (CheckOpTimeDelay(CurrentOp))
			{
				CurrentOp->nStep = STEP1;
			}	
		break;
		case STEP6:
			if (input_get_one(SN_LIFT_UP_DN_ORG) == SENSOR_TYPE_BEAM_ON) //Go to origin
			{
				sm_stop(SM_LIFT_UP_DN); 	//Stop elevator car		
				CurrentOp->nStep = STEP7;
			}	
			else if (IsSmRunFinish(SM_LIFT_UP_DN)) 
			{
				CurrentOp->nResult = ERROR_LIFT_CAR_RETURN_ORIGIN_FAIL; //ERROR:电梯小车返回原点失败
			}		
		break;
		case STEP7:
			sm_run(SM_LIFT_UP_DN, DIR_SM_LIFT_DN,100,10);  //Continue move a distance
			CurrentOp->nStep = STEP8;
		break;
		case STEP8:
			if (IsSmRunFinish(SM_LIFT_UP_DN))
			{
				sm_set_abs_zero(SM_LIFT_UP_DN); //Clean step motor of absolute steps
				gLiftUpDnPosition = ENUM_LIFT_UP_DN_ORG; //Elevator car is origin
				CurrentOp->nResult  = 0xffff;	
			}
		break;

		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ResetLiftModule
*****************************************************
Descriptions:  Reset lift module
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
void Op_ResetLiftModule(void)
{
	stOpCtrl *CurrentOp = &OpResetLiftModule;

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
			StartChildOp(CurrentOp, &OpLiftHorizonReset); //Lift horizontal reset
			CurrentOp->nStep = STEP2;				
	   	break;

		case STEP2:
		 if(IsChildOpOK(CurrentOp, &OpLiftHorizonReset))
		{
			CurrentOp->nStep = STEP3;
		}
		break;

		case STEP3:
			StartChildOp(CurrentOp, &OpLiftUpDnReset); //Lift car vertical reset	
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
		if(IsChildOpOK(CurrentOp, &OpLiftUpDnReset))
		{
			CurrentOp->nResult  = 0xffff;
		}
		break;

		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_LiftUpDnMove
*****************************************************
Descriptions:  lift car vertical move
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
void Op_LiftUpDnMove(void)
{
	stOpCtrl *CurrentOp = &OpLiftUpDnMove;
	static unsigned char LiftUpDnPos = 0; 			//Receive lift car position
	static unsigned char LiftUpDnMoveCnt	= 0;	//电梯小车运行达到一定次数复位消除累积误差
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (flag_LiftUpDnMove == true) //Bottom call flag
			{
				flag_LiftUpDnMove = false;
				LiftUpDnPos = gLiftUpDnPosition; //Get bottom call position
			}
			else //Upper call
			{
				LiftUpDnPos = cParamBuf[10]; //Get data bit
			}
			if(LiftUpDnPos > 5)
			{
				CurrentOp->nResult = ERROR_LIFT_CAR_POSITION_PARA_MAX;   //ERROR:电梯升降移动接收参数错误
			}		
			CurrentOp->nStep = STEP1;					
		break;
			
 		case STEP1: //Protection lift car befor move horizontal reset avert crash
		 	if((gLiftHorizonPosition == ENUM_LIFT_HORIZON_ORG) ||		  //Lift horizontal at origin
			   (gLiftHorizonPosition == ENUM_LIFT_HORIZON_OUT_TO_OCR) ||  //Lift send card to OCR position
			   (gLiftHorizonPosition == ENUM_LIFT_HORIZON_GET_FROM_OCR))  //OCR send card to lift position
			{
				CurrentOp->nStep = STEP3;
			}
			else
			{
				StartChildOp(CurrentOp, &OpLiftHorizonReset);  //Lift car horizontal reset
				CurrentOp->nStep = STEP2;				
			}
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpLiftHorizonReset)) 
			{
				CurrentOp->nStep = STEP3;
			}
		break; 
		case STEP3:
			if (gLiftUpDnPosition == ENUM_LIFT_UP_DN_UNKNOW) //Unknow position
			{				
				CurrentOp->nStep = STEP4;
			}
			else
			{
				CurrentOp->nStep = STEP6;
			}             	
		break;
		case STEP4:
			StartChildOp(CurrentOp,&OpLiftUpDnReset); 		//Reset lift car		
			CurrentOp->nStep = STEP5;	
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnReset)) 
			{
				CurrentOp->nStep = STEP6;	
			}
		break;
		case STEP6:
			if(LiftUpDnMoveCnt < 100) //运行到一定次数后复位清除累积误差
			{
				LiftUpDnMoveCnt++;
				CurrentOp->nStep = STEP7;
			}
			else
			{
				CurrentOp->nStep = STEP4;
				LiftUpDnMoveCnt = 0;
			}
		break;
		case STEP7:   //According upper transfer of position go absolute steps
			sm_run_abs(SM_LIFT_UP_DN,parameter[PAR_LIFT_UP_DN_SPEED],PULSE_TO_MILLIMETER(gAbsLiftUpDnMove[LiftUpDnPos]));	
			CurrentOp->nStep= STEP8;
		break;
		case STEP8:
			if(IsSmRunFinish(SM_LIFT_UP_DN))
			{
				sm_stop(SM_LIFT_UP_DN); 
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			gLiftUpDnPosition = LiftUpDnPos; //Synchronize upper transfer of position
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);			
}
/****************************************************
Function Name: Op_LiftHorizonReset
*****************************************************
Descriptions:  Lift Horizontal Reset
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
void Op_LiftHorizonReset(void)
{
	stOpCtrl *CurrentOp = &OpLiftHorizonReset;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			gLiftHorizonPosition = STATUS_UNKNOW;
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if (input_get_one(SN_LIFT_HORIZON_ORG) == SENSOR_TYPE_BEAM_OFF) 	//不在原点	
			{
				sm_run(SM_LIFT_HORIZON_MOVE, DIR_SM_LIFT_HORIZON_BACK,50,1000); //Backward move a distance
				CurrentOp->nStep = STEP4;
			}
			else 																
			{
				sm_run(SM_LIFT_HORIZON_MOVE, DIR_SM_LIFT_HORIZON_FRONT,50, 220); //离开原点
				CurrentOp->nStep = STEP2;
			}			
		break;
		case STEP2:
			if(IsSmRunFinish(SM_LIFT_HORIZON_MOVE))  
			{
				if (input_get_one(SN_LIFT_HORIZON_ORG) == SENSOR_TYPE_BEAM_OFF) //Leave origin
				{
					SetOpTimeDelay(CurrentOp,300);   
					CurrentOp->nStep = STEP3;	
				}
				else
				{
					CurrentOp->nResult = ERROR_LIFT_CAR_LEAVE_HORIZON_ORIGIN_FAIL; //ERROR:电梯小车离开水平原点失败
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
			if (input_get_one(SN_LIFT_HORIZON_ORG) == SENSOR_TYPE_BEAM_ON) //Go to origin
			{
				sm_stop(SM_LIFT_HORIZON_MOVE); 	
				CurrentOp->nStep = STEP5;
			}	
			else if (IsSmRunFinish(SM_LIFT_HORIZON_MOVE)) //Motor action finish
			{
				CurrentOp->nResult = ERROR_LIFT_CAR_RETURN_HORIZON_ORIGIN_FAIL; //ERROR:电梯小车回水平原点失败
			}		
		break;
		case STEP5:
			sm_run(SM_LIFT_HORIZON_MOVE,DIR_SM_LIFT_HORIZON_BACK,20,180);  //Continue move a distance
			CurrentOp->nStep = STEP6;
		break;
		case STEP6:
			if (IsSmRunFinish(SM_LIFT_HORIZON_MOVE))
			{
				sm_set_abs_zero(SM_LIFT_HORIZON_MOVE); //Clean step motor of absolute steps
				gLiftHorizonPosition = ENUM_LIFT_HORIZON_ORG; //Connection at origin
				CurrentOp->nResult  = 0xffff;	
			}
		break;

		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_LiftHorizonMove
*****************************************************
Descriptions: Lift car horizontal Move
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
void Op_LiftHorizonMove(void)
{
	stOpCtrl *CurrentOp = &OpLiftHorizonMove;
	static unsigned char LiftHorizonPos = 0; //Receive Connection of position
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (flag_Lift_H_MOVE == true) //Bottom call
			{
				flag_Lift_H_MOVE = false;
				LiftHorizonPos = gLiftHorizonPosition; 
			}
			else //Upper call
			{
				LiftHorizonPos = cParamBuf[10];
			}
			if(LiftHorizonPos > 3)
			{
				CurrentOp->nResult = ERROR_LIFT_CAR_HORIZON_POSITION_PARA_MAX;//ERROR:电梯水平移动接收参数错误
			}	
			CurrentOp->nStep = STEP1;					
		break;
		case STEP1: 
		 	if(gLiftHorizonPosition == ENUM_LIFT_HORIZON_UNKNOW) //Connection at position
			{
				StartChildOp(CurrentOp, &OpLiftHorizonReset);  //Reset connection
				CurrentOp->nStep = STEP2;			
			}
			else
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpLiftHorizonReset)) 
			{
				CurrentOp->nStep = STEP3;
			}
		break; 
		case STEP3:   //According Upper transfer of position go absolute step
			sm_run_abs(SM_LIFT_HORIZON_MOVE,20,gAbsLiftHorizonMove[LiftHorizonPos]);	
			CurrentOp->nStep= STEP4;
		break;
		case STEP4:
			if(IsSmRunFinish(SM_LIFT_HORIZON_MOVE))	
			{
				gLiftHorizonPosition = LiftHorizonPos; //SynchronizeUpper transfer of position
				CurrentOp->nResult = 0xffff;
			}
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
Function Name: Op_RWChannelInCardFromLiftSide
*****************************************************
Descriptions: Read write channel in card from lift side
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
void Op_RWChannelInCardFromLiftSide(void)
{
	stOpCtrl *CurrentOp = &OpRWChannelInCardFromLiftSide;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			if (input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_HAVE_CARD_NOT_ENTER_CARD; //ERROR:通道有卡不能进卡
			}
			CurrentOp->nStep = STEP1;
		break;
		case STEP1: //Read write module forward run
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],5000); 
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON)  
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP3;
			}
			else if(IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_ENTER_CARD_FAIL;		 //ERROR:读写器通道进卡失败
			}
		break;
		case STEP3: //Go to out card exit
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],10);
			CurrentOp->nStep = STEP4;
		break;
		case STEP4: 
			if(IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nStep = STEP5;
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
Function Name: Op_RWChannelOutCardToLiftSide
*****************************************************
Descriptions:  Read write channel out card to lift side
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
void Op_RWChannelOutCardToLiftSide(void)
{
	stOpCtrl *CurrentOp = &OpRWChannelOutCardToLiftSide;
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
			//Read write module out card
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],3000);
			CurrentOp->nStep = STEP2;
		break;
		case STEP2: //Read write module enter card position sensor detected have card
			if (input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP3;	
			}
			else if(IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_OUT_CARD_TO_LIFT_NO_CARD; //ERROR:读写通道出卡到电梯无卡
			}
		break;
		case STEP3: 
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],3000);
			CurrentOp->nStep = STEP4;	
		break;			
		case STEP4: 
	   		if ((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF)
			&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF))
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_OUT_CARD_TO_LIFT_FAIL; //ERROR:读写通道出卡到电梯失败;
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_LiftSuckSendCardToRWChannel
*****************************************************
Descriptions:  Lift send card to read write channel
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
void Op_LiftSuckSendCardToRWChannel(void)
{
	stOpCtrl *CurrentOp = &OpLiftSuckSendCardToRWChannel;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);			//Gas on
			dm_ctrl_one(DM_RECEIVE_CARD, RELAY_ON); 		    //Receive card position vacuum open
			sm_set_enable(SM_RWMODULE,SM_DISABLE);				//将读写模块电机释放这样容易把卡送进去
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			flag_LiftUpDnMove = true; 					   	   //Elevator car bottom call
			gLiftUpDnPosition = ENUM_LIFT_SUCK_TO_RW_CHANNEL;  //Elevator car move read write module position
			StartChildOp(CurrentOp,&OpLiftUpDnMove); 	  	   //Elevator car move
			CurrentOp->nStep 	 = STEP2;			
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				if(input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_VACUUM_ON)   //Lift car suck of card
				{
					flag_Lift_H_MOVE  = true; 				
					gLiftHorizonPosition   = ENUM_LIFT_HORIZON_REC_AND_SEND;  // Receive send card position
					StartChildOp(CurrentOp,&OpLiftHorizonMove); 
					CurrentOp->nStep = STEP3;
				}
				else
				{
					dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF); 		   		  //Receive card position vacuum close
					dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_OFF);
					CurrentOp->nResult = ERROR_RECEIVE_CARD_POSITION_NOT_CARD;//ERROR:电梯接卡位没有吸到卡
				} 
			}
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				StartChildOp(CurrentOp,&OpRWChannelInCardFromLiftSide); 		//Read write module in card
				SetOpTimeDelay(CurrentOp,70);
				CurrentOp->nStep = STEP10;
			}
		break;
		case STEP10:
			if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_RECEIVE_CARD, RELAY_OFF);  		//Receive card position vacuum close
				dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_OFF);	//Gas relay close
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpRWChannelInCardFromLiftSide))
			{
				StartChildOp(CurrentOp,&OpLiftHorizonReset); 
				CurrentOp->nStep = STEP5;
			}			
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonReset))
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
			StartChildOp(CurrentOp,&OpRWChannelSendCardToLiftSlot); 	   //读写通道发卡到电梯卡槽
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpRWChannelSendCardToLiftSlot))
			{
				CurrentOp->nStep = STEP3; 
			}
		break;
		case STEP3:
			flag_LiftUpDnMove = true; 				
			gLiftUpDnPosition = ENUM_LIFT_SLOT_TO_OCR;  		
			StartChildOp(CurrentOp,&OpLiftUpDnMove); 
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			flag_Lift_H_MOVE     = true; 				
			gLiftHorizonPosition = ENUM_LIFT_HORIZON_OUT_TO_OCR;  // Out to OCR position
			StartChildOp(CurrentOp,&OpLiftHorizonMove); 
			CurrentOp->nStep 	 = STEP6;
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				flag_Lift_H_MOVE  	 = true; 				
				gLiftHorizonPosition = ENUM_LIFT_HORIZON_GET_FROM_OCR;  // Get card form OCR position
				StartChildOp(CurrentOp,&OpLiftHorizonMove); 
				CurrentOp->nStep     = STEP7;
			}
		break;
		case STEP7:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
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
Descriptions: OCR out card to read write module 
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
			flag_LiftUpDnMove = true; 					//Lift car bottom call
			gLiftUpDnPosition = ENUM_LIFT_SLOT_TO_OCR;  //Lift car move OCR position
			StartChildOp(CurrentOp,&OpLiftUpDnMove); 	//Lift car move
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				flag_Lift_H_MOVE  	   = true; 				
				gLiftHorizonPosition   = ENUM_LIFT_HORIZON_GET_FROM_OCR;  // Get card form OCR position
				StartChildOp(CurrentOp,&OpLiftHorizonMove); 
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP3;	
			}
		break;	
		case STEP3:
			if(CheckOpTimeDelay(CurrentOp))
			{
				flag_Lift_H_MOVE  	   = true; 				
				gLiftHorizonPosition   = ENUM_LIFT_HORIZON_REC_AND_SEND;  // Receive send card position
				StartChildOp(CurrentOp,&OpLiftHorizonMove); 
				CurrentOp->nStep = STEP4;
			}
		break;	
		case STEP4:
			if(IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				if(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON)  //Card slot have card
				{
					SetOpTimeDelay(CurrentOp,300);
					CurrentOp->nStep = STEP5;
				}
				else if(CheckOpTimeDelay(CurrentOp))
				{
					CurrentOp->nResult = ERROR_OCR_TO_RWCHANNEL_CARD_SLOT_NOT_CARD; //ERROR:OCR到读写通道卡槽无卡
				}
			}
		break;	
		case STEP5:	
			if(CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp,&OpLiftHorizonReset);
				CurrentOp->nStep = STEP6;	
			}
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonReset))
			{
				flag_LiftUpDnMove = true; 							//Lift car bottom call
				gLiftUpDnPosition = ENUM_LIFT_SLOT_GET_RW_CHANNEL;  //Lift car move read write module position
				StartChildOp(CurrentOp,&OpLiftUpDnMove); 			//Lift car move
				CurrentOp->nStep = STEP7; 
			}
		break;
		case STEP7:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				flag_Lift_H_MOVE  	  = true; 				
				gLiftHorizonPosition  = ENUM_LIFT_HORIZON_REC_AND_SEND;   // Receive send card position
				StartChildOp(CurrentOp,&OpLiftHorizonMove); 
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				StartChildOp(CurrentOp,&OpRWChannelInCardFromLiftSide);
				CurrentOp->nStep =STEP9;
			}
		break;
		case STEP9:
			if(IsChildOpOK(CurrentOp,&OpRWChannelInCardFromLiftSide))
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
Function Name: Op_CutSendCardToRWModule
*****************************************************
Descriptions:  Cut send card
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
void Op_CutSendCardToRWModule(void)
{
	stOpCtrl *CurrentOp = &OpCutSendCardToRWModule;
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
			flag_LiftUpDnMove = true; 							//Lift car bottom call
			gLiftUpDnPosition = ENUM_LIFT_SUCK_RECEIVE_CARD;    //Lift to receive card position
			StartChildOp(CurrentOp,&OpLiftUpDnMove); 			//Lift car move
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				sm_run(SM_LIFT_HORIZON_MOVE,DIR_SM_LIFT_HORIZON_FRONT,20,200); //因为电梯到剪切位前有较大空隙会掉卡下来，这里电梯水平向前走一点
				CurrentOp->nStep = STEP3;
			}		
		break;
		case STEP3:
			if(IsSmRunFinish(SM_LIFT_HORIZON_MOVE))
			{
				CurrentOp->nStep = STEP4;				
			}
		case STEP4:
			StartChildOp(CurrentOp,&OpCuttingPlatformWork);				//Start cut card process
			CurrentOp->nStep = STEP5;				
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpCuttingPlatformWork))
			{
				StartChildOp(CurrentOp,&OpLiftSuckSendCardToRWChannel); //Lift send card to read write channel
				CurrentOp->nStep = STEP6;
			}	
		break;	
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpLiftSuckSendCardToRWChannel))
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
Function Name: Op_PerfectCardExit
*****************************************************
Descriptions:  Prefect card exit card
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
void Op_PerfectCardExit(void)
{
	stOpCtrl *CurrentOp = &OpPerfectCardExit;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],2000);
			CurrentOp->nStep = STEP1;
		break;
		case STEP1: 
			if (input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON) //Exit detection have card
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP2;
			}
			else if (IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_EXIT_CARD_NOT_CARD; //ERROR:读写通道出卡无卡
			}	

		break;
		case STEP2: 
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],parameter[PAR_RWMODULE_OUT_CARD]);
			CurrentOp->nStep = STEP3;
		break;
		case STEP3: 
			if (IsSmRunFinish(SM_RWMODULE))
			{
				if (input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON)
				{
					sm_set_enable(SM_RWMODULE,SM_DISABLE);	//释放读写通道电机,取卡时比较方面，不然要很大力才能拔出来
					CurrentOp->nResult = 0xffff;
				}
				else
				{
					CurrentOp->nResult = ERROR_CARD_NOT_MOV_TO_PERFECT_CARD_POSITON; //ERROR:卡片未移动到好卡位
				}
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_PerfectCardEnter
*****************************************************
Descriptions:  Prefect card enter card
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
void Op_PerfectCardEnter(void)
{
	stOpCtrl *CurrentOp = &OpPerfectCardEnter;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			if (input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF)
			{
				CurrentOp->nResult = ERROR_NONE_CARD_NOT_START_ENTER_CARD_ACTION;	//ERROR:无卡不能进行收卡动作
			}
			CurrentOp->nStep = STEP1;
		break;
		case STEP1: 
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],5000);
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if (input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON) //Enter card sensor detection have card
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP3;
			}
			else if (IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_PERFECT_CARD_ENTER_FAIL;	 	 //ERROR:好卡进卡失败
			}
		break;
		case STEP3: 
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],2000);
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP5;
			}
			else if (IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_PERFECT_CARD_ENTER_TO_EXIT_CARD_SENSOR_FAIL;	//ERROR:好卡进卡到出卡口位置失败
			}
		break;
		case STEP5: 
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],10);
			CurrentOp->nStep = STEP6;
		break;
		case STEP6:
			if (IsSmRunFinish(SM_RWMODULE))
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
Function Name: UpdateStatus
*****************************************************
Descriptions:  Update sensor status
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
void UpdateStatus(void)
{
	//判断预定位工位是否有卡
	if (input_get_one(SN_PRETARGETING_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON)		  		
	{
		CardStatus[STATION_LOADING_PLATFORM_HAS_CARD]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_LOADING_PLATFORM_HAS_CARD]=  STATION_HAS_CARD_NO;
	}

	/* 	//判断热压位检卡传感器是否有卡
		if (input_get_one(SN_WARM_CHECK_CARD) == SENSOR_TYPE_BEAM_ON)		  		
		{
			CardStatus[STATION_HOT_PRESS_OUT_SENSOR_HAS_CARD]=  STATION_HAS_CARD_YES;
		}
		else
		{
			CardStatus[STATION_HOT_PRESS_OUT_SENSOR_HAS_CARD]=  STATION_HAS_CARD_NO;
		}

		//判断冷压位检卡传感器是否有卡
		if (input_get_one(SN_COOL_CHECK_CARD) == SENSOR_TYPE_BEAM_ON)		  		
		{
			CardStatus[STATION_COLD_PRESS_OUT_SENSOR_HAS_CARD]=  STATION_HAS_CARD_YES;
		}
		else
		{
			CardStatus[STATION_COLD_PRESS_OUT_SENSOR_HAS_CARD]=  STATION_HAS_CARD_NO;
		} */

	//判断电梯吸盘处是否有卡
	if (input_get_one(SN_RECEIVE_CARD) == SENSOR_TYPE_VACUUM_ON) 	//Lift car suck of card	  		
	{
		CardStatus[STATION_LIFT_SUCK_HAS_CARD]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_LIFT_SUCK_HAS_CARD]=  STATION_HAS_CARD_NO;
	}

	//判断电梯卡槽是否有卡
	if (input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON)		  		
	{
		CardStatus[STATION_LIFT_MID_HAS_CARD]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_LIFT_MID_HAS_CARD]=  STATION_HAS_CARD_NO;
	}

	//判断出卡通道有卡是否有卡
	if ((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON)||(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON))		  		
	{
		CardStatus[STATION_OUT_PORT_HAS_CARD]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_OUT_PORT_HAS_CARD]=  STATION_HAS_CARD_NO;
	}

	//判断回收箱满
	if (input_get_one(SN_WASTER_CARD_BOX_FULL) == SENSOR_TYPE_BEAM_ON)		  		
	{
		CardStatus[STATION_RECYCLE_BOX_FULL]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_RECYCLE_BOX_FULL]=  STATION_HAS_CARD_NO;
	}

	//判断回收箱到位
	if (input_get_one(SN_WASTER_CARD_BOX_CHECK) == SENSOR_TYPE_BEAM_OFF)		  		
	{
		CardStatus[STATION_RECYCLE_BOX_IN_PLACE]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_RECYCLE_BOX_IN_PLACE]=  STATION_HAS_CARD_NO;
	}

	//判断回收箱是否为空
	if (input_get_one(SN_WASTER_CARD_BOX_EMPTY) == SENSOR_TYPE_REFLECT_ON)		  		
	{
		CardStatus[STATION_RECYCLE_BOX_HAS_CARD]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_RECYCLE_BOX_HAS_CARD]=  STATION_HAS_CARD_NO;
	}
	//判断废料箱满
	if (input_get_one(SN_WASTE_BOX_FULL) == SENSOR_TYPE_REFLECT_ON)		  		
	{
		CardStatus[STATION_WASTER_BOX_FULL]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_WASTER_BOX_FULL]=  STATION_HAS_CARD_NO;
	}
	//判断废料箱到位
	if (input_get_one(SN_WASTE_BOX_CHECK) == SENSOR_TYPE_BEAM_ON)		  		
	{
		CardStatus[STATION_WASTER_BOX_IN_PLACE]=  STATION_HAS_CARD_YES;
	}
	else
	{
		CardStatus[STATION_WASTER_BOX_IN_PLACE]=  STATION_HAS_CARD_NO;
	}
}
/****************************************************
Function Name: Op_LiftSlotSendCardToRWChannel
*****************************************************
Descriptions:  Lift card slot send card to RWChannel
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
void Op_LiftSlotSendCardToRWChannel(void)
{
	static unsigned char retry_cnt1 = 0,retry_cnt2 = 0; //Lift horizontal move retry and read write channel retry
	stOpCtrl *CurrentOp = &OpLiftSlotSendCardToRWChannel;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON)   //Read write channel lift direction enter card sensor detection have card
			||(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON))//Perfect card out card sensor detection have card
			{
				CurrentOp->nResult = ERROR_LIFT_SLOT_TO_RWCH_SD_CARD_DETEC_HAV_CARD;//ERROR:电梯槽往读写通道送卡时检测到读写通道已经有卡
			}

			if(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_OFF)
			{
				CurrentOp->nResult = ERROR_LIFT_SLOT_NO_CARD_NO_SEND_CARD_TO_RWCH;	//ERROR:电梯槽无卡,无法送卡到读写通道
			}
			retry_cnt1 = 0;
			retry_cnt2 = 0;
			CurrentOp->nStep = STEP1;
		break;

		case STEP1:
			sm_set_enable(SM_RWMODULE,SM_DISABLE);				//释放读写通道电机,容易送卡进读写通道
			CurrentOp->nStep = STEP2;
		break;

		case STEP2:
			flag_LiftUpDnMove	 = true; 					   
			gLiftUpDnPosition = ENUM_LIFT_SLOT_GET_RW_CHANNEL;  //电梯槽对准读写通道
			StartChildOp(CurrentOp,&OpLiftUpDnMove); 	   
			CurrentOp->nStep 	 = STEP3;			
		break;

		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				CurrentOp->nStep = STEP4;
			}
		break;

		case STEP4:
			flag_Lift_H_MOVE  = true; 				
			gLiftHorizonPosition   = ENUM_LIFT_HORIZON_REC_AND_SEND;  // OCR exit card connection
			StartChildOp(CurrentOp,&OpLiftHorizonMove); 
			CurrentOp->nStep = STEP5;
		break;

		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				CurrentOp->nStep = STEP6;	
			}
		break;

		case STEP6:
			if(input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON)
			{
				CurrentOp->nStep = STEP7;	
			}
			else
			{
				if(retry_cnt1<3)
				{
					retry_cnt1++;
					CurrentOp->nStep = STEP1;
				}
				else
				{
					retry_cnt1 = 0;
					CurrentOp->nResult = ERROR_LIFT_SLOT_SEND_CARD_TO_RWCH_RETRY_FAIL;	//ERROR:电梯卡槽送卡到读写通道多次重试失败失败
				}
			}
		break;
		case STEP7:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],5000);
			CurrentOp->nStep = STEP8;
		break;	
		case STEP8: 
			if (input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON) //Perfect card out sensor detection have card
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP_SUCCESS;	
			}
			else if(IsSmRunFinish(SM_RWMODULE))
			{
				if(retry_cnt2<3)
				{
					retry_cnt2++;
					CurrentOp->nStep = STEP9;
				}
				else
				{
					retry_cnt2 = 0;
					CurrentOp->nResult = ERROR_RWCHANNEL_ENTER_CARD_TO_EXIT_CARD_SENSOR_FAIL;//ERROR:读写通道进卡到出卡传感器位置失败
				}
			}	
		break;
		case STEP9:
			sm_set_enable(SM_RWMODULE,SM_DISABLE);		//释放读写通道电机
			CurrentOp->nStep = STEP10;
		break;
 		case STEP10:
			StartChildOp(CurrentOp,&OpLiftHorizonReset);
			CurrentOp->nStep = STEP11;
		break;
 		case STEP11:
		if (IsChildOpOK(CurrentOp,&OpLiftHorizonReset))
		{
			CurrentOp->nStep = STEP4;
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
Function Name: Op_RWChannelSendCardToLiftSlot
*****************************************************
Descriptions:  Read write channel send card to lift slot
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
void Op_RWChannelSendCardToLiftSlot(void)
{
	stOpCtrl *CurrentOp = &OpRWChannelSendCardToLiftSlot;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			if(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON)
			{
				CurrentOp->nResult = ERROR_LIFT_CARD_SLOT_HAVE_CARD_RWCHANNEL_NOT_SEND_CARD; //ERROR:电梯卡槽有卡,读写通道无法送卡到电梯卡槽
			}
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF)
			&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF))  		//读写通道没有检测到有卡
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP2:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],500);
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:
			if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON)
			||(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON))
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP4;
			}
			else if (IsSmRunFinish(SM_RWMODULE)) 		
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_NO_CARD_NO_EXIT_CARD_TO_LIFT_SLOT;	//ERROR:读写通道无卡，无法出卡到电梯卡槽
			}
		break;
		case STEP4:
			flag_LiftUpDnMove	 = true; 					   
			gLiftUpDnPosition = ENUM_LIFT_SLOT_GET_RW_CHANNEL;  //电梯槽对准读写通道
			StartChildOp(CurrentOp,&OpLiftUpDnMove); 	   
			CurrentOp->nStep 	 = STEP5;			
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			flag_Lift_H_MOVE  	 = true; 				
			gLiftHorizonPosition = ENUM_LIFT_HORIZON_REC_AND_SEND;  // 
			StartChildOp(CurrentOp,&OpLiftHorizonMove); 
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				CurrentOp->nStep = STEP8;	
			}
		break;
		case STEP8:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],5000);
			CurrentOp->nStep = STEP9;
		break;
	    case STEP9:
			if(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON) //Lift card slot detection have card
			{
				CurrentOp->nStep = STEP11;
			}
			else if (IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_EXIT_CARD_TO_LIFT_CARD_SLOT_FAIL;//ERROR:读写通道出卡到电梯卡槽失败
			}
		break;
		case STEP11:
	   		StartChildOp(CurrentOp,&OpLiftHorizonReset);
			CurrentOp->nStep = STEP12;	
		break;
		case STEP12:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonReset))
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP13;
			}
		break;
	   	case STEP13:
			if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF)
			&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF)
			&&(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_ON))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else
			{
				CurrentOp->nResult = ERROR_LIFT_CARD_SLOT_FROM_RWCHANNEL_PULL_CARD_FAIL;//ERROR:电梯卡槽从读写器通道拔卡失败
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
Function Name: Op_LiftSlotSendCardToRecycleBox
*****************************************************
Descriptions:  Lift slot send card to recucle box
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
void Op_LiftSlotSendCardToRecycleBox(void)
{
	stOpCtrl *CurrentOp = &OpLiftSlotSendCardToRecycleBox;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			if(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_OFF) //Lift card slot no detection card
			{
				CurrentOp->nResult = ERROR_LIFT_CARD_NO_CARD_NO_EXIT_CARD_TO_RECYCLE_BOX; //ERROR:电梯卡槽无卡,无法出卡到回收箱
			}
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			sm_set_enable(SM_RWMODULE,SM_DISABLE);				//释放读写通道电机
			flag_LiftUpDnMove	 = true; 					   
			gLiftUpDnPosition = ENUM_LIFT_SLOT_TO_RECYCLE_BOX;  //电梯槽对准回收通道
			StartChildOp(CurrentOp,&OpLiftUpDnMove); 	   
			CurrentOp->nStep 	 = STEP2;			
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpLiftUpDnMove))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			flag_Lift_H_MOVE  	 = true; 				
			gLiftHorizonPosition = ENUM_LIFT_HORIZON_REC_AND_SEND;  // 
			StartChildOp(CurrentOp,&OpLiftHorizonMove); 
			CurrentOp->nStep = STEP4;
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpLiftHorizonMove))
			{
				CurrentOp->nStep = STEP5;	
			}
		break;
		case STEP5:
			if(input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON) //Read write channel detection have card
			{
<<<<<<< HEAD
				sm_run(SM_RWMODULE,DIR_SM_ENTER_RECYCLE_BOX,parameter[PAR_RWMODULE_SPEED],10000);
=======
				sm_run(SM_RWMODULE,DIR_SM_ENTER_RECYCLE_BOX,parameter[PAR_RWMODULE_SPEED],5000);
>>>>>>> e58908bfac133081514ec13bb451e3754d11a839
				CurrentOp->nStep = STEP6;
			}
			else
			{
				CurrentOp->nResult = ERROR_LIFT_CARD_SLOT_SEND_CARD_TO_RECYCLE_CHANNEL_FAIL;//ERROR:电梯卡槽送卡到回收通道入口失败
			}
		break;
		case STEP6:
			if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF)
			&&(input_get_one(SN_CARD_SLOT) == SENSOR_TYPE_REFLECT_OFF))
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else if (IsSmRunFinish(SM_RWMODULE))
			{
				CurrentOp->nResult = ERROR_LIFT_CARD_SLOT_TO_RECYCLE_BOX_FAIL;//ERROR:电梯卡槽送卡到回收箱失败
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
Function Name: Op_RWModuleReset
*****************************************************
Descriptions: Read write module reset
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
void Op_RWModuleReset(void)
{
	stOpCtrl *CurrentOp = &OpRWModuleReset;
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
			if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF)    	 //Read write and waster channel no detection card 
			 &&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF))   //Exit card side no detection card 
			{
				CurrentOp->nStep = STEP2;
			}
			else if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON) //Read write and waster channel detection card 
			&&(input_get_one(SN_RW_CH_LIFT_ENTER_CARD) == SENSOR_TYPE_BEAM_ON) //读写通道电梯侧传感器用于和SN_RW_CH_NEAR_LIFT对比区分卡在读写还是废卡通道
			&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF))
			{
			 	CurrentOp->nStep = STEP4;
			}
			else if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF)
			&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON))  //Exit card side detection card 
			{
				CurrentOp->nStep = STEP6;
			}
			else if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON)	  //waster channel detection card
			&&(input_get_one(SN_RW_CH_LIFT_ENTER_CARD) == SENSOR_TYPE_BEAM_OFF)   //读写通道电梯侧传感器用于和SN_RW_CH_NEAR_LIFT对比区分卡在读写还是废卡通道
			&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF))     //Exit card side detection no card 
			{
				CurrentOp->nStep = STEP8;
			}
			else
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_HAVE_MULTI_CARD; //读写通道有多张卡,或者传感器坏
			}
		break;
		case STEP2:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],3000);
			CurrentOp->nStep = STEP3;
		break;
		case STEP3:
			if(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON) //好卡出卡口传感器检测到有卡
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else if (IsSmRunFinish(SM_RWMODULE)) 		
			{
				if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF)
				&&(input_get_one(SN_RW_CH_LIFT_ENTER_CARD) == SENSOR_TYPE_BEAM_OFF) //Read write channel no detection card 
				&&(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_OFF))
				{
		
					CurrentOp->nStep = STEP_SUCCESS;					  	//通道无卡
				}
				else
				{
					CurrentOp->nResult = ERROR_RWCHANNEL_SEND_CARD_EIXT_CARD_FAIL; //读写通道送卡到出卡口传感器X128位置失败
				}
			}
		break;
		case STEP4:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_IN_FROM_LIFT,parameter[PAR_RWMODULE_SPEED],5000);
			CurrentOp->nStep = STEP5;
		break;

		case STEP5:
			if(input_get_one(SN_RW_CH_NEAR_OUTSIDE) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP_SUCCESS;
			}
			else if (IsSmRunFinish(SM_RWMODULE)) 		
			{
				CurrentOp->nResult = ERROR_RWCHANNEL_ENTER_CARD_SENSOR_TO_EXIT_CARD_SENSOR_FAIL;//ERROR:读写通道进卡传感器X124位到出卡传感器X128位失败
			}
		break;
		case STEP6:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],5000);
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if(input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP4;
			}
			else if (IsSmRunFinish(SM_RWMODULE)) 		
			{
				CurrentOp->nResult = ERROR_RWCHANNLE_EXIT_CARD_SENSOR_TO_ENTER_CARD_SENSOR_FAIL; //ERROR:读写通道出卡传感器X128位到进卡传感器X124位失败
			}
		break;
		case STEP8:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],10000); //把卡送进废卡箱
			CurrentOp->nStep = STEP9;
		break;
		case STEP9:
			if((input_get_one(SN_RW_CH_NEAR_LIFT) == SENSOR_TYPE_BEAM_OFF) //废卡通道没检测到卡
			&&(input_get_one(SN_RW_CH_LIFT_ENTER_CARD) == SENSOR_TYPE_BEAM_OFF)) //Read write channel no detection card 
			{
				sm_stop(SM_RWMODULE);
				CurrentOp->nStep = STEP10;					  	
			}
		break;
		case STEP10:
			sm_run(SM_RWMODULE,DIR_SM_RW_CH_OUT_LIFT,parameter[PAR_RWMODULE_SPEED],1000); //把卡送进废卡箱
			CurrentOp->nStep = STEP11;
		break;
		case STEP11:
			if(IsSmRunFinish(SM_RWMODULE))
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
	Op_OpenGasSwitch();			  		//25 打开总气开关
	Op_CloseGasSwitch();				//26 关闭总气开关
	Op_WarmCoolMakeCard();				//27 冷热制卡
	Op_GotoLoadingPlatform();			//28 去装料平台	
	Op_CuttingPlatformWork();			//29 裁剪平台工作
	Op_LiftUpDnReset();					//30 电梯升降复位
	Op_ResetLiftModule();				//31 复位电梯模块(包含水平和垂直复位) 
	Op_LiftUpDnMove();					//32 电梯升降移动
	Op_LiftHorizonReset();				//33 电梯水平复位
	Op_LiftHorizonMove();				//34 电梯水平移动
	Op_ReceiveCardVacuumOn();			//35 接卡位真空开
	Op_ReceiveCardVacuumOff();			//36 接卡位真空关
	Op_HeaterPowerOn();					//37 发热管电源开
	Op_HeaterPowerOff();				//38 发热管电源关
 ////////////////////////////////////////////////////////////////////////
	Op_RWChannelInCardFromLiftSide();	//39 读写模块从电梯侧进卡
	Op_RWChannelOutCardToLiftSide();	//40 读写模块出卡到电梯侧
	Op_LiftSuckSendCardToRWChannel();	//41 电梯吸盘发卡到读写通道
	Op_RWModuleSendCardToOCR();			//42 读写模块发卡到OCR
	Op_OCRToRWModuleOutCard();			//43 OCR到读写模块出卡
	Op_CutSendCardToRWModule();			//44 剪发卡到读写模块
	Op_PerfectCardEnter();				//45 好卡回读写通道
	Op_PerfectCardExit();				//46 好卡出卡	
/////////////////////////////////////////////////////////////////////////
	Op_LiftSlotSendCardToRWChannel(); 	//47电梯槽发卡到读写通道
	Op_RWChannelSendCardToLiftSlot();	//48读写通道发卡到电梯槽
	Op_LiftSlotSendCardToRecycleBox();	//49电梯槽发卡到回收箱
	Op_RWModuleReset();					//50读写模块复位
	UpdateStatus();
}






