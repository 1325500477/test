#include "../Parameter/Parameter.h"
#include "../Command/command.h"
#include "../IOCfg.h"
#include "../../BSP/bsp.h"
#include "../../BSP/Command/bsp_command.h"
#include "process.h"
#include <string.h>
#include <stdio.h>

/******** ��ض����ṹ��������� ******************************/
stOpCtrl OpResetModule;	            			//1 ��λ
stOpCtrl OpResetClampCardCar;   				//2 ��λ�п�С��
stOpCtrl OpClampCardCarMove; 					//3 �п�С���ƶ�λ��	
stOpCtrl OpResetClamp;							//4 צ�Ӹ�λ
stOpCtrl OpClampOpen;							//5 צ���ſ�
stOpCtrl OpClampClose;							//6 צ�ӱպ�
stOpCtrl OpRightClampClose; 					//7 �ұ߼��ӱպ�
stOpCtrl OpRightClampOpen; 						//8 �ұ߼����ɿ�
stOpCtrl OpLeftClampClose; 						//9 ��߼��ӱպ�
stOpCtrl OpLeftClampOpen; 						//10 ��߼����ɿ�
stOpCtrl OpOpeningPressure;	 					//11 ѹ����
stOpCtrl OpClosingPressure; 					//12 ѹ����
stOpCtrl OpWarmDown; 			 				//13 �����½�
stOpCtrl OpWarmRise; 			 		  		//14 ��������
stOpCtrl OpCoolDown; 				  			//15 ��ȴ�½�
stOpCtrl OpCoolRise; 			 	  			//16 ��ȴ����
stOpCtrl OpPresDown; 				  			//17 ��ѹ�½�
stOpCtrl OpPresRise; 			 	  			//18 ��ѹ����	
stOpCtrl OpPretargetingReset;            		//19 Ԥ��λ��λ
stOpCtrl OpPretargetingUp;                		//20 Ԥ��λ����
stOpCtrl OpWasteStorageReset;  					//21 ���ϳ������λ
stOpCtrl OpWasteStorageDown;	 				//22 ���ϳ�����½�
stOpCtrl OpPretargetingVacuumCupOpen;			//23 Ԥ��λ�������
stOpCtrl OpPretargetingVacuumCupClose;			//24 Ԥ��λ������ر�
stOpCtrl OpOpenGasSwitch;			 			//25 ��·����
stOpCtrl OpCloseGasSwitch;						//26 ��·�ر�
//��϶���
stOpCtrl OpWarmCoolMakeCard;					//27 ����ѹ�ƿ�
stOpCtrl OpGotoLoadingPlatform;			   		//28 ȥԤ��λƽ̨
stOpCtrl OpCuttingPlatformWork;		 			//29 �ü�ƽ̨����
stOpCtrl OpScramProcess;						//30 ����ֹͣ�������ƿ�����
//����ģ��
stOpCtrl OpResetElevatorCar;					//31 ��λ����С��
stOpCtrl OpElevatorCarMove;						//32 ����С���ƶ�
stOpCtrl OpResetConnection;						//33 ��λ�ν�ģ��
stOpCtrl OpConnectionSendCard;					//34 �ν�ģ�鵽����λ
stOpCtrl OpReceiveCardVacuumOn;					//35 �ӿ�λ��տ�
stOpCtrl OpReceiveCardVacuumOff;				//36 �ӿ�λ��չ�
stOpCtrl OpHeaterPowerOn;						//37 ���ȹܵ�Դ��
stOpCtrl OpHeaterPowerOff;						//38 ���ȹܵ�Դ��
stOpCtrl OpRWModuleInCard;						//39 ��дģ�����
stOpCtrl OpRWModuleOutCard;						//40 ��дģ�����
//�������
stOpCtrl OpElevatorSendCardToRWModule;			//41 ���ݷ�������дģ��
stOpCtrl OpRWModuleSendCardToOCR;				//42 ��дģ�鷢����OCR
stOpCtrl OpOCRToRWModuleOutCard;				//43 OCR����дģ�����


stOpCtrl OpAutoMakeCard;						//44 �Զ��ƿ�


unsigned char gStatusPretarget 	  = STATUS_UNKNOW;		//Ԥ��λλ��״̬
unsigned char gStatusWastestorage = STATUS_UNKNOW;  	//����λ��״̬
unsigned char bFlagReset 		  = FALSE;        		//��λ��־λ
/******** ��������*********************************/
stOpCtrl *gMachineOperation[50]; // ����һ��֧��50�����̵�ָ������
unsigned char gnOperationNo = 1; // ִ�еĶ�����ţ���1��ʼ�Ը��������ṹ����������ʼ����
extern unsigned char cParamBuf[];//���ղ���
//Clamp card car
unsigned char gTrolleyCarMove_Flag 	 = false;  		//ͨ��С�����б�־
unsigned char gTrolleyCarPosition  	 = ENUM_UNKNOW; //ͨ��С����λ��
unsigned int gTrolleyCarMoveCount 	 = 0;           //С���ƶ�����,�ﵽһ��������λ�����ۻ����
unsigned int gAbsPosTrolleyCarMov[6] = { 	//ץ��С���ƶ�������λ�ľ��Բ�����ʼֵ
	0, 	 	//����
	13, 	//1��ѹλ 
	165,	//2��ѹλ
	311,	//3Ԥ��λ
	448,	//4����λ
	580,	//5����λ
 };	
//����С��
unsigned char gRWCheckCar_Flag	    = false; //����С�����б�־λ
unsigned char gElevatorCarPosition  = ENUM_ELEVATOR_UNKNOW;  //����С��λ��
unsigned int  gRWCheckCarCount	    = 0;	//����С�����дﵽһ��������λ�����ۻ����
//�ν�ģ�� 
unsigned char gConnectionPosition   = STATUS_UNKNOW; //Connection module position flag
unsigned int gAbsElevatorCarMove[6] = { //����С���ƶ�
	0,	  //����
	250,  //1�ӿ�λ
	42,   //2���ݵ���дģ��λ(�ÿ�����λ)
	195,  //3��дģ�鵽����λ
	177,  //4�Ͽ�λ
	13,   //5OCRλ 
}; 
/****************************************************
Function Name: ProcessUpdateParameter
*****************************************************
Descriptions:  
ˢ�²���������Բ��������ṩ����������ƶ�����ʹ��
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
		//1 ����ѹ��λ
		//2 ����ѹ��λ
		//3 �����Ϲ�λ
		//4 ��Ԥ��λ
		//5 �ü���λ��		
		gAbsPosTrolleyCarMov[i+1] = parameter[i];
		//12�ӿ�λ
		//13���ݵ���дģ��
		//14��дģ�鵽����
		//15�Ͽ�λ
		//16OCR���λ 	 		
		gAbsElevatorCarMove[i+1] = parameter[i+11];
	} 
	/* 	gAbsPosTrolleyCarMov[ENUM_INSIDE_WARE] 	       = parameter[PAR_WARM_STEP];		   //1 ����ѹ��λ
		gAbsPosTrolleyCarMov[ENUM_INSIDE_COOL]         = parameter[PAR_COOL_STEP];		   //2 ����ѹ��λ
		gAbsPosTrolleyCarMov[ENUM_INSIDE_WASTE]    	   = parameter[PAR_WASTE];	   		   //3 ��˺Ĥ��λ
		gAbsPosTrolleyCarMov[ENUM_INSIDE_PRETARGETING] = parameter[PAR_PRETARGETING];	   //4 ��Ԥ��λ
		gAbsPosTrolleyCarMov[ENUM_INSIDE_CUT_CARD] 	   = parameter[PAR_CUT_CARD_POSITION]; //5 ������λ
	*/
}

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
	gMachineOperation[gnOperationNo++] = &OpResetModule;				//1 ��λ
	gMachineOperation[gnOperationNo++] = &OpResetClampCardCar; 			//2 ��λ�п�С��
	gMachineOperation[gnOperationNo++] = &OpClampCardCarMove;   		//3 �п�С���ƶ�λ��
	gMachineOperation[gnOperationNo++] = &OpResetClamp;					//4 ��λצ��
	gMachineOperation[gnOperationNo++] = &OpClampOpen; 					//5 צ���ſ�
	gMachineOperation[gnOperationNo++] = &OpClampClose; 				//6 צ�ӱպ�
	gMachineOperation[gnOperationNo++] = &OpRightClampClose; 			//7 �Ҳ�צ�ӱպ�
	gMachineOperation[gnOperationNo++] = &OpRightClampOpen; 			//8 �Ҳ�צ���ſ�
	gMachineOperation[gnOperationNo++] = &OpLeftClampClose; 			//9 ���צ�ӱպ�
	gMachineOperation[gnOperationNo++] = &OpLeftClampOpen; 				//10 ���צ���ſ�
	gMachineOperation[gnOperationNo++] = &OpOpeningPressure;			//11 ��Դ��
	gMachineOperation[gnOperationNo++] = &OpClosingPressure;			//12 ��Դ��
	gMachineOperation[gnOperationNo++] = &OpWarmRise;				 	//13 ��ѹ����
	gMachineOperation[gnOperationNo++] = &OpWarmDown;				 	//14 ��ѹ�½�
	gMachineOperation[gnOperationNo++] = &OpCoolDown;					//15 ��ȴ�½�
	gMachineOperation[gnOperationNo++] = &OpCoolRise;					//16 ��ȴ����
	gMachineOperation[gnOperationNo++] = &OpPresDown;					//17 ��ѹ�½�
	gMachineOperation[gnOperationNo++] = &OpPresRise;					//18 ��ѹ����
	gMachineOperation[gnOperationNo++] = &OpPretargetingReset;          //19 Ԥ��λ��λ
	gMachineOperation[gnOperationNo++] = &OpPretargetingUp;             //20 Ԥ��λ����
	gMachineOperation[gnOperationNo++] = &OpWasteStorageReset;			//21 ���ϳ������λ
	gMachineOperation[gnOperationNo++] = &OpWasteStorageDown;	  		//22 ���ϳ�����½�
	gMachineOperation[gnOperationNo++] = &OpPretargetingVacuumCupOpen;	//23 Ԥ��λ��մ�
 	gMachineOperation[gnOperationNo++] = &OpPretargetingVacuumCupClose;	//24 Ԥ��λ��չر�
	gMachineOperation[gnOperationNo++] = &OpOpenGasSwitch;				//25 ������	
	gMachineOperation[gnOperationNo++] = &OpCloseGasSwitch;				//26 �����ر�
	//��϶���
	gMachineOperation[gnOperationNo++] = &OpWarmCoolMakeCard;			//27 ����ѹ�ƿ�	
	gMachineOperation[gnOperationNo++] = &OpGotoLoadingPlatform;		//28 ��Ԥ��λ+Ԥ��λ����+�򿪼���
	gMachineOperation[gnOperationNo++] = &OpCuttingPlatformWork;		//29 ����+����				 
	gMachineOperation[gnOperationNo++] = &OpScramProcess;				//30 ����ֹͣ�������ƿ�����
	//����ģ��
	gMachineOperation[gnOperationNo++] = &OpResetElevatorCar;			//31 ��λ����С��
	gMachineOperation[gnOperationNo++] = &OpElevatorCarMove;			//32 ����С���ƶ�
	gMachineOperation[gnOperationNo++] = &OpResetConnection;			//33 ��λ�ν�ģ��
	gMachineOperation[gnOperationNo++] = &OpConnectionSendCard;			//34 �ν�ģ�鵽����λ
	gMachineOperation[gnOperationNo++] = &OpReceiveCardVacuumOn;		//35 �ӿ�λ��ѹ��
	gMachineOperation[gnOperationNo++] = &OpReceiveCardVacuumOff;		//36 �ӿ�λ��ѹ��
	gMachineOperation[gnOperationNo++] = &OpHeaterPowerOn;				//37 ���ȹܵ�Դ��
	gMachineOperation[gnOperationNo++] = &OpHeaterPowerOff;				//38 ���ȹܵ�Դ��
	gMachineOperation[gnOperationNo++] = &OpRWModuleInCard;				//39 ��дģ�����
	gMachineOperation[gnOperationNo++] = &OpRWModuleOutCard;			//40 ��дģ�����
	gMachineOperation[gnOperationNo++] = &OpElevatorSendCardToRWModule;	//41 ���ݷ�������дģ��
	gMachineOperation[gnOperationNo++] = &OpRWModuleSendCardToOCR;		//42 ��дģ�鷢����OCR
	gMachineOperation[gnOperationNo++] = &OpOCRToRWModuleOutCard;		//43 OCR����дģ�����

	gMachineOperation[gnOperationNo++] = &OpAutoMakeCard;				//44 �Զ��ƿ�
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
			else  //��ԭ��
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
					CurrentOp->nResult = ERROR_CHANNAL_LEAVE_ORG_FAIL;   //11001�뿪ԭ�㴫����ʧ��
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
				CurrentOp->nResult = ERROR_CHANNAL_RETURN_ORG_FAIL;	//11002 ͨ�����ش�����ʧ��			
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
				CurrentOp->nResult = ERROR_WARMING_DOWN_FAIL; //11003�����½�ʧ��
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
				CurrentOp->nResult = ERROR_WARMING_RISE_FAIL; //11004��������ʧ��
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
				CurrentOp->nResult = ERROR_COOLING_DOWN_FAIL; //11005��ȴ�½�ʧ��
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
				CurrentOp->nResult = ERROR_COOLING_RISE_FAIL;  //11006��ȴ����ʧ��
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
				CurrentOp->nResult = ERROR_PRESS_DOWN_FAIL;	 //11007��ѹ�½�ʧ��
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
				CurrentOp->nResult = ERROR_PRESS_RISE_FAIL;	 //11008 ��ѹ����ʧ��
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
			else //����
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
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_CLOSE_FAIL;	 //11009 �ұ߼��ӹر�ʧ��
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
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
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_OPEN_FAIL;	//11010 �ұ߼��Ӵ�ʧ��
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
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
				CurrentOp->nResult = ERROR_LEFT_CLAMP_CLOSE_FAIL;	//11011 ��߼��ӹر�ʧ��
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
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
				CurrentOp->nResult = ERROR_LEFT_CLAMP_OPEN_FAIL; //11012 ��ߴ��������ϵ��¼��Ӵ�ʧ��
			}
		break;
		default:
		break;
	}
	DealResult(CurrentOp); //������������ʹ��
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
				CurrentOp->nResult = ERROR_TROLLEY_CAR_POSITION_PARA_MAX; //11013 ץ��С���ϲ㴫������ݳ����������ֵ
			}		
			CurrentOp->nStep = STEP1;					
		break;
	/////////////////////////////////////////////////////////////////////////////
	/*����С���ƶ�ʱ��ı���,С���ƶ�ʱ�������Ԥ��λ(˺Ĥλ)�½��ͷ���λ����,
	��ѹ��ѹ��������,���ӱպ�,С���ƶ�ʱ��ײ�����µ��ʧ�i��Ƥ���Բ�׼��λ  */	
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
			if(gTrolleyCarMoveCount<100) //���е�һ��������λ����ۻ����
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
				sm_stop(SM_PRETARGETING); //ֹͣ���
				sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_BACKWARD,10,350); //Again go a distance
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_PRETARGETING))
			{
				CurrentOp->nResult = ERROR_PRETARGETING_RETURN_ORIGIN_FAIL;	  //11014 Ԥ��λ����ԭ��ʧ��
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
					CurrentOp->nResult = ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL; //11015 Ԥ��λ�뿪ԭ��ʧ��
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
			sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_FORWARD,10,parameter[PAR_PRETARGET_UP_POSITION]);	 //�뿪ԭ��	
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
					CurrentOp->nResult = ERROR_PRETARGETING_UP_LEAVE_ORIGIN_FAIL;//11033 Ԥ��λ����ʱ�뿪ԭ��ʧ��
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
				CurrentOp->nResult = ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL;	 //11016 �Ͽ�λ����ԭ��ʧ��
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
					CurrentOp->nResult = ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL; //11017 �Ͽ�λ�뿪ԭ��ʧ��
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
					CurrentOp->nResult = ERROR_WASTESTORAGE_DOWN_LEAVE_ORIGIN_FAIL;//11034 �Ͽ�λ�½��뿪ԭ��ʧ�� 
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
Descriptions: ����ѹ�ƿ�ģ��
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
				CurrentOp->nResult = ERROR_PRETARGETING_NONE_CARD;	//11018 Ԥ��λ�޿�
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
					CurrentOp->nResult = ERROR_PRETARGETING_FROM_COOL_NONE_CARD; //11019 Ԥ��λ����ѹλ�޿�
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
				CurrentOp->nResult = ERROR_WARM_FROM_COOL_NONE_CARD; //11020 ��ѹλ����ѹλ�޿�
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
				CurrentOp->nResult = ERROR_COOL_FROM_PRETARGETING_NONE_CARD;//11021 ��ѹλ��Ԥ��λ�޿�
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
			if(gStatusPretarget == STATUS_ORG)	//ȷ��Ԥ��λ��ԭ�㣬��ֹ��צ��ײ
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
				CurrentOp->nResult = ERROR_PRETARGETING_FROM_WASTE_NONE_CARD;//11022 Ԥ��λ������λ�޿�
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
	///////////////////////////////�ȴ򿪼���Ȼ������ѹ�ѷ���ѹ����ȥ/////////////////////////////
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
					CurrentOp->nResult = ERROR_ELEVATOR_CAR_LEAVE_ORIGIN_FAIL; //11023 ����С���뿪ԭ��ʧ��
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
				CurrentOp->nResult = ERROR_ELEVATOR_CAR_RETURN_ORIGIN_FAIL; //11024 ����С������ԭ��ʧ��
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
				CurrentOp->nResult = ERROR_ELEVATOR_CAR_POSITION_PARA_MAX;   //11025 ����С���ϲ㴫������ݳ����������ֵ
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
			if(gRWCheckCarCount < 100) //���е�һ��������λ����ۻ����
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
					CurrentOp->nResult = ERROR_CONNECTION_LEAVE_ORIGIN_FAIL; //11026 �ν�ģ���뿪ԭ��ʧ��
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
				CurrentOp->nResult = ERROR_CONNECTION_RETURN_ORIGIN_FAIL; //10027 �ν�ģ�鷵��ԭ��ʧ��
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
					CurrentOp->nResult = ERROR_CONNECTION_TO_SENDCARD_LEAVE_ORIGIN_FAIL; //11028 �ν�ģ�鵽����λ�뿪ԭ��ʧ��
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
				CurrentOp->nResult = ERROR_RWMODULE_NOT_CHECK_CARD;//11029 ��дģ��û�м�⵽��Ƭ	
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
					count = 0; //����count����,û��������Ϊÿ��ִ�ж���START���������
					CurrentOp->nResult = ERROR_RWMODULE_NOT_IN_CARD;//10030 ��дģ��û�п�Ƭ����		
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
					CurrentOp->nResult = ERROR_CARD_SLOT_NOT_IN_CARD;//11031 ����û�п�Ƭ����
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
					CurrentOp->nResult = ERROR_RECEIVE_CARD_POSITION_NOT_CARD;//11032 ���ݽӿ�λû��������
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
	Op_ResetModule();					//1 ��λģ��
	Op_ResetClampCardCar(); 			//2 ��λ�п�С��ͨ��
	Op_ClampCardCarMove(); 				//3 �п�С���ƶ�
	Op_ResetClamp();					//4 צ�Ӹ�λ
	Op_ClampOpen();						//5 צ���ſ�
	Op_ClampClose();					//6 צ�ӱպ�
	Op_RightClampClose();			    //7 �Ҳ���ӹر� 			
  	Op_RightClampOpen();				//8 �Ҳ���Ӵ�          
	Op_LeftClampClose();				//9 �����ӹر�
	Op_LeftClampOpen();					//10 �����Ӵ�
	Op_OpeningPressure();				//11 ѹ����
	Op_ClosingPressure();				//12 ѹ����
	Op_WarmRise();						//13 �����½�
	Op_WarmDown();						//14 ��������
	Op_CoolRise();						//15 ��ȴ�½�
	Op_CoolDown();						//16 ��ȴ����
	Op_PresRise();						//17 ��ѹ�½�
	Op_PresDown();						//18 ��ѹ����
	Op_PretargetingReset();				//19 Ԥ��λ��λ
	Op_PretargetingUp();				//20 Ԥ��λ����
	Op_WasteStorageReset();		 		//21 ���ϳ���λ
	Op_WasteStorageDown();	   			//22 ���ϳ��½�
	Op_PretargetingVacuumCupOpen();		//23 Ԥ��λ�������
	Op_PretargetingVacuumCupClose();	//24 Ԥ��λ������ر�
	Op_OpenGasSwitch();			  		//27 ����������
	Op_CloseGasSwitch();				//28 �ر���������
	Op_WarmCoolMakeCard();				//29 �����ƿ�
	Op_GotoLoadingPlatform();			//30 ȥװ��ƽ̨	
	Op_CuttingPlatformWork();			//31 �ü�ƽ̨����
	Op_ScramProcess();					//32 ����ֹͣ�Ƽ�������
	Op_ResetElevatorCar();				//31 ��λ��д����³� 
	Op_ElevatorCarMove();				//32 ����С���ƶ�
	Op_ResetConnection();				//33 ��λ�ν�ģ��
	Op_ConnectionSendCard();			//34 �ν�ģ���ƶ�������λ
	Op_ReceiveCardVacuumOn();			//35 �ӿ�λ��տ�
	Op_ReceiveCardVacuumOff();			//36 �ӿ�λ��չ�
	Op_HeaterPowerOn();					//37 ���ȹܵ�Դ��
	Op_HeaterPowerOff();				//38 ���ȹܵ�Դ��
	Op_RWModuleInCard();				//39 ��дģ�����
	Op_RWModuleOutCard();				//40 ��дģ�����
	Op_ElevatorSendCardToRWModule();	//41 ���ݷ�������дģ��
	Op_RWModuleSendCardToOCR();			//42 ��дģ�鷢����OCR
	Op_OCRToRWModuleOutCard();			//43 OCR����дģ�����


	Op_AutoMakeCard();					//44 �Զ��ƿ�
}






