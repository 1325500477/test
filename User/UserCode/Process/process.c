#include "../Parameter/Parameter.h"
#include "../Command/command.h"
#include "../IOCfg.h"
#include "../../BSP/bsp.h"
#include "../../BSP/Command/bsp_command.h"
#include "process.h"
#include <string.h>
#include <stdio.h>

/******** ��ض����ṹ��������� ******************************/
stOpCtrl OpResetModule;	            					//��λ
stOpCtrl OpResetClampCardCar;   						//��λ�Ϳ�ͨ��
stOpCtrl OpWarmDown; 			 						//�����½�
stOpCtrl OpWarmRise; 			 		  				//��������
stOpCtrl OpCoolDown; 				  					//��ȴ�½�
stOpCtrl OpCoolRise; 			 	  					//��ȴ����
stOpCtrl OpPresDown; 				  					//��ѹ�½�
stOpCtrl OpPresRise; 			 	  					//��ѹ����
stOpCtrl OpOpeningPressure;	 							//ѹ����
stOpCtrl OpClosingPressure; 							//ѹ����
stOpCtrl OpRightClampClose; 							//�ұ߼��ӱպ�
stOpCtrl OpRightClampOpen; 								//�ұ߼����ɿ�
stOpCtrl OpLeftClampClose; 								//��߼��ӱպ�
stOpCtrl OpLeftClampOpen; 								//��߼����ɿ�
stOpCtrl OpClampClose; 									//���ӱպ�
stOpCtrl OpClampCardCarMove; 							//ץ��С���ƶ�λ��	
stOpCtrl OpPretargetingReset;            				//Ԥ��λ��λ
stOpCtrl OpPretargetingUp;                				//Ԥ��λ��	
//hww
stOpCtrl  OpResetClamp;									//צ�Ӹ�λ
stOpCtrl  OpClampOpen;									//צ���ſ�
stOpCtrl  OpClampClose;									//צ�ӱպ�
stOpCtrl OpWasteStorageReset;  							//���ϳ������λ
stOpCtrl OpWasteStorageDown;	 						//���ϳ�����½�
stOpCtrl OpPretargetingVacuumCupOpen;					//Ԥ��λ�������
stOpCtrl OpPretargetingVacuumCupClose;					//Ԥ��λ������ر�
stOpCtrl OpSuckVacuumCupOpen;							//�����������
stOpCtrl OpSuckVacuumCupClose;							//����������ر�
stOpCtrl OpOpenGasSwitch;			 				    //��·����
stOpCtrl OpCloseGasSwitch;								//��·�ر�
stOpCtrl OpWarmCoolMakeCard;							//����ѹ�ƿ�
stOpCtrl OpGotoLoadingPlatform;			   				//ȥԤ��λƽ̨
stOpCtrl OpCuttingPlatformWork;		 					//�ü�ƽ̨����
stOpCtrl OpScramProcess;								//����ֹͣ�������ƿ�����
//����С��
stOpCtrl OpSuckCardCarMove;								//����С���ƶ�λ��
stOpCtrl OpResetSuckCardCar;							//����С����λ
stOpCtrl OpResetSuck;									//���̸�λ
//����
stOpCtrl OpSuckMove;									//�����½�
//�׿���
stOpCtrl OpResetBlankCardBox;							//�׿��临λ
stOpCtrl OpBlankCardBoxUp;								//�׿�������
stOpCtrl OpSendCardToPretargeting;						//�Ϳ���Ԥ��λ
stOpCtrl OpAutoMakeCard;								//�Զ��ƿ�
stOpCtrl OpReturnCard;									//����
stOpCtrl OpPretargetingToAccessory;						//Ԥ��λ������λ



unsigned char nStatusClipOpen 	  = STATUS_UNKNOW;		//��е�ֿ���״̬
unsigned char nStatusPretarget 	  = STATUS_UNKNOW;		//Ԥ��λλ��״̬
unsigned char nStatusWastestorage = STATUS_UNKNOW;  	//����λ��״̬
unsigned char nStatusSuck		  = STATUS_UNKNOW;  	//���̵�λ��״̬
unsigned char nStatusBlankCardBox = STATUS_UNKNOW;      //�հ׿���λ��״̬

unsigned char bFlagReset = FALSE;        				//��λ��־λ
/******** ��������*********************************/

stOpCtrl *gMachineOperation[50]; // ����һ��֧��50�����̵�ָ������
unsigned char gnOperationNo = 1; // ִ�еĶ�����ţ���1��ʼ�Ը��������ṹ����������ʼ����
extern unsigned char cParamBuf[];//���ղ���
unsigned char gStartBottonPressed  	 = false;  		//��ʼ������״̬��ǣ�0-δ���°�����1-���°�����
unsigned char gPauseBottonPressed  	 = false;   	//��ͣ������״̬��ǣ�0-δ���°�����1-���°�����
//ץ��С��
unsigned char gTrolleyCarMove_Flag 	 = false;  		//ͨ��С�����б�־
unsigned char gTrolleyCarPosition  	 = ENUM_UNKNOW; //ͨ��С����λ��
unsigned char gTrolleyCarMoveCount 	 = 0;           //С���ƶ�����,�ﵽһ��������λ�����ۻ����
//����С��
unsigned char gSuckCardCarMove_Flag  = false;       //����С�����б�־
unsigned char gSuckCardCarPosition   = ENUM_UNKNOW; //����С��λ��
unsigned char gSuckCardCarMoveCount  = 0;           //����С���ƶ�����,�ﵽһ��������λ�����ۻ����
//����
unsigned char gSuckMove_Flag  		 = false;       //�������б�־
unsigned char gSuckPosition   		 = ENUM_UNKNOW; //����λ��

unsigned int iAbsPosTrolleyCarMov[7] = { 			//С���ƶ�������λ�ľ��Բ�����ʼֵ
	0, 	//����
	140, //��ѹλ 
	1960,//��ѹλ
	3720,//Ԥ��λ
	5050,//����λ
	6590,//����λ
	0 	//����
 };		
unsigned int iAbsSuckCardCarMov[8] = { //����С���ƶ�������λ�ľ��Բ�����ʼֵ
	0,	//����
	23300,//����λ
	21000,//Ԥ��λ
	18800,//��ʱ�ſ�λ
	9500,//�¸���λ
	5000,//��ӡλ
	300,//�׿�λ
	0 	//����
}; 
unsigned int iAbsSuckMove[8] = { //���̵���ڸ�����λ�½��ľ��Բ�����ʼֵ
	0,	//����
	600,//����λ
	1450,//Ԥ��λ
	600,//��ʱ�ſ�λ
	200,//�¸���λ
	1500,//��ӡλ
	270,//�׿�λ
	0 	//����
}; 
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
		iAbsPosTrolleyCarMov[i+1] = parameter[i];
//12 ����С������λ
//13 ����С��Ԥ��λ
//14 ����С���ݷſ�λ
//15 ����С���¸���λ
//16 ����С����ӡλ
//17 ����С���հ׿�λ	
		iAbsSuckCardCarMov[i+1]   = parameter[i+11];
//18 ���̸���λ 
//19 ����Ԥ��λ
//20 �����ݷſ�λ
//21 �����¸���λ
//22 ���̴�ӡλ
//23 ���̿հ׿�λ	
		iAbsSuckMove[i+1]  		  = parameter[i+17];
	} 
	
/* 	iAbsPosTrolleyCarMov[ENUM_INSIDE_WARE] 	       = parameter[PAR_WARM_STEP];		   //1 ����ѹ��λ
	iAbsPosTrolleyCarMov[ENUM_INSIDE_COOL]         = parameter[PAR_COOL_STEP];		   //2 ����ѹ��λ
	iAbsPosTrolleyCarMov[ENUM_INSIDE_WASTE]    	   = parameter[PAR_WASTE];	   		   //3 ��˺Ĥ��λ
	iAbsPosTrolleyCarMov[ENUM_INSIDE_PRETARGETING] = parameter[PAR_PRETARGETING];	   //4 ��Ԥ��λ
	iAbsPosTrolleyCarMov[ENUM_INSIDE_CUT_CARD] 	   = parameter[PAR_CUT_CARD_POSITION]; //5 ������λ

	iAbsSuckCardCarMov[ENUM_ACCESSORY]     = parameter[PAR_SUCK_CARD_CAR_ACCESSORY]; //С���ڸ��Ϲ�λ	    //1
	iAbsSuckCardCarMov[ENUM_PRETARGETING]  = parameter[PAR_SUCK_CARD_CAR_PRETARGETING]; //С����Ԥ��λ��λ    //2	
	iAbsSuckCardCarMov[ENUM_TEMPORARY]     = parameter[PAR_SUCK_CARD_CAR_TEMPORARY]; //С������ʱ�ſ���λ  //3
	iAbsSuckCardCarMov[ENUM_NEW_ACCESSORY] = parameter[PAR_SUCK_CARD_CAR_NEW_ACCESSORY]; //С�����¸��Ϲ�λ	//4
	iAbsSuckCardCarMov[ENUM_PRINT]		   = parameter[PAR_SUCK_CARD_CAR_PRINT]; //С���ڴ�ӡ��λ 	    //5
	iAbsSuckCardCarMov[ENUM_BLANK] 		   = parameter[PAR_SUCK_CARD_CAR_BLANK]; //С���ڿհ׿���λ    //7

 	iAbsSuckMove[ENUM_ACCESSORY]     = parameter[PAR_SUCK_ACCESSORY]; //18 ���̸���λ
	iAbsSuckMove[ENUM_PRETARGETING]  = parameter[PAR_SUCK_PRETARGETING]; //19 ����Ԥ��λ	
	iAbsSuckMove[ENUM_TEMPORARY]     = parameter[PAR_SUCK_TEMPORARY]; //20 �����ݷſ�λ
	iAbsSuckMove[ENUM_NEW_ACCESSORY] = parameter[PAR_SUCK_NEW_ACCESSORY]; //21 �����¸���λ
	iAbsSuckMove[ENUM_PRINT]		 = parameter[PAR_SUCK_PRINT];//22 ���̴�ӡλ
	iAbsSuckMove[ENUM_BLANK] 		 = parameter[PAR_SUCK_BLANK]; //23 ���̿հ׿�λ
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
	gMachineOperation[gnOperationNo++] = &OpSuckVacuumCupOpen;			//25 ������մ�
	gMachineOperation[gnOperationNo++] = &OpSuckVacuumCupClose;			//26 ��������չر�
	gMachineOperation[gnOperationNo++] = &OpOpenGasSwitch;				//27 ������	
	gMachineOperation[gnOperationNo++] = &OpCloseGasSwitch;				//28 �����ر�
	//��϶���
	gMachineOperation[gnOperationNo++] = &OpWarmCoolMakeCard;			//29 ����ѹ�ƿ�	
	gMachineOperation[gnOperationNo++] = &OpGotoLoadingPlatform;		//30 ��Ԥ��λ+Ԥ��λ����+�򿪼���
	gMachineOperation[gnOperationNo++] = &OpCuttingPlatformWork;		//31 ����+����				 
	gMachineOperation[gnOperationNo++] = &OpScramProcess;				//32 ����ֹͣ�������ƿ�����
	//����С��
	gMachineOperation[gnOperationNo++] = &OpSuckCardCarMove;            //33 ����С���ƶ�λ��
	gMachineOperation[gnOperationNo++] = &OpResetSuckCardCar;			//34 ����С����λ
	gMachineOperation[gnOperationNo++] = &OpResetSuck;					//35 ���̸�λ
	gMachineOperation[gnOperationNo++] = &OpSuckMove;					//36 �����½�
	//�׿���
	gMachineOperation[gnOperationNo++] = &OpResetBlankCardBox;			//37 �׿��临λ
	gMachineOperation[gnOperationNo++] = &OpBlankCardBoxUp;				//38 �׿�������	
	gMachineOperation[gnOperationNo++] = &OpSendCardToPretargeting;		//39 �Ϳ���Ԥ��λ
	gMachineOperation[gnOperationNo++] = &OpAutoMakeCard;				//40 �Զ��ƿ�
	gMachineOperation[gnOperationNo++] = &OpReturnCard;					//41 ����
	gMachineOperation[gnOperationNo++] = &OpPretargetingToAccessory;	//42 //Ԥ��λ������λ

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
Function Name: Op_ResetClampCardCar
*****************************************************
Descriptions:  ��λС��
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
			StartChildOp(CurrentOp, &OpPretargetingReset); //Ԥ��λ��λ
			CurrentOp->nStep = STEP21;
	   	break;
		case STEP21:
		   	if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset);//���ϳ���λ
				CurrentOp->nStep = STEP22;
			}
	   	break;
		case STEP22:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				StartChildOp(CurrentOp, &OpPresRise);  	//�ü���λ��λ
				CurrentOp->nStep = STEP23;
			}
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);  	//��ѹ��λ��λ
				CurrentOp->nStep = STEP24;
			}
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//��ѹ��λ��λ
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
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) 	//����ԭ��	
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 80, 50000); //�������һ�ξ���
				CurrentOp->nStep = STEP5;
			}
			else if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //��ԭ��
			{
				sm_run(SM_CHANNEL, DIR_SM_CHANNEL_FORWARD, 80, 1000); //��ǰ����һ�ξ���
				CurrentOp->nStep = STEP2;
			}			
		break;
		case STEP2:
			if(IsSmRunFinish(SM_CHANNEL) == true)  
			{
				if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_OFF) //�뿪ԭ��
				{
					SetOpTimeDelay(CurrentOp,300);   //��ת��ʱ
					CurrentOp->nStep = STEP3;	
				}
				else if(IsSmRunFinish(SM_CHANNEL))        	
				{
					CurrentOp->nResult = ERROR_CHANNAL_LEAVE_ORG_FAIL;   //�뿪ԭ�㴫����ʧ��
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
			if (input_get_one(SN_CAR_CHANNAL_ORG) == SENSOR_TYPE_BEAM_ON) //��ԭ��
			{
				sm_stop(SM_CHANNEL); 			//ֹͣ���
				CurrentOp->nStep = STEP6;
			}	
			else if (IsSmRunFinish(SM_CHANNEL) == true) //����������
			{
				CurrentOp->nResult = ERROR_CHANNAL_RETURN_ORG_FAIL;		  //10002 ͨ�����ش�����ʧ��			
			}		
		break;
		case STEP6:
			sm_run(SM_CHANNEL, DIR_SM_CHANNEL_BACKWARD, 10, 5); 		//����������һ�ξ���
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if (IsSmRunFinish(SM_CHANNEL) == true)
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			sm_set_abs_zero(SM_CHANNEL); //���㲽������Ĳ���
			gTrolleyCarPosition = ENUM_INSIDE_ORIGIN; //��ԭ��
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
			if (input_get_one(SN_WARM_DOWN) == SENSOR_TYPE_BEAM_ON)	 //�����洫������
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_WARM_CONTROL, RELAY_ON); //�½�
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
				CurrentOp->nResult = ERROR_WARMING_DOWN_FAIL; //10003�����½�ʧ��
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
			if (input_get_one(SN_WARM_UP) == SENSOR_TYPE_BEAM_ON)	 //�����洫������
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_WARM_CONTROL, RELAY_OFF); //����
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
				CurrentOp->nResult = ERROR_WARMING_RISE_FAIL;	 //10004��������ʧ��
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
			if (input_get_one(SN_COOL_DOWN) == SENSOR_TYPE_BEAM_ON)	 //�����洫������
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_COOL_CONTROL, RELAY_ON); //�½�
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
				CurrentOp->nResult = ERROR_COOLING_DOWN_FAIL;	//10005��ȴ�½�ʧ��
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
			if (input_get_one(SN_COOL_UP) == SENSOR_TYPE_BEAM_ON)  //�����洫������
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_COOL_CONTROL, RELAY_OFF); //����
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
				CurrentOp->nResult = ERROR_COOLING_RISE_FAIL;  //10006��ȴ����ʧ��
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
			if (input_get_one(SN_PUNCH_DOWN) == SENSOR_TYPE_BEAM_ON)   //�����洫������
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_PRES_CONTROL, RELAY_ON);		//�½�
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
				CurrentOp->nResult = ERROR_PRESS_DOWN_FAIL;	 //10007��ѹ�½�ʧ��
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
			if (input_get_one(SN_PUNCH_UP) == SENSOR_TYPE_BEAM_ON)	 //�����洫������
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_PRES_CONTROL, RELAY_OFF);	 //����
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
				CurrentOp->nResult = ERROR_PRESS_RISE_FAIL;	  		//10008 ��ѹ����ʧ��
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
Descriptions:  ��ѹ��
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
			dm_ctrl_one(DM_ALL_CONTROL, RELAY_ON);	//��ѹ��
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
			dm_ctrl_one(DM_ALL_CONTROL, RELAY_OFF);  //��ѹ��
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
Descriptions: �ұ߼��ӹر�
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
			if (input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON) //�Ѿ��ڹر�λ
			{
				CurrentOp->nResult = 0xffff;
			} 
			else //����
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_CW);  //�ұ߼��ӵ��
				SetOpTimeDelay(CurrentOp,3000);
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_RIGHT_CLOSE) == SENSOR_TYPE_BEAM_ON)  //����պ�λ
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE); //ֹͣ���
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);	//ֹͣ���
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_CLOSE_FAIL;	 //10009 �ұ߼��ӹر�ʧ��
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
Descriptions:  �ұ߼��Ӵ�
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
			if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON)  //�Ѿ��ڴ�λ
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_CW); //������� 
				SetOpTimeDelay(CurrentOp,3000);    //��ʱ
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_RIGHT_OPEN) == SENSOR_TYPE_BEAM_ON)  //���е���λ
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);	//���ֹͣ
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_RIGHT,DM_BRAKE);
				CurrentOp->nResult = ERROR_RIGHT_CLAMP_OPEN_FAIL;	//10010 �ұ߼��Ӵ�ʧ��
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
Descriptions: ��߼��ӹر�
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
			if (input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON)	//�Ѿ��ڹر�λ
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_CW); //�������
				SetOpTimeDelay(CurrentOp,3000); 
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_CLAMP_LEFT_CLOSE) == SENSOR_TYPE_BEAM_ON) //���е��ر�λ
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //ֹͣ���
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE);
				CurrentOp->nResult = ERROR_LEFT_CLAMP_CLOSE_FAIL;	//10011 ��߼��ӹر�ʧ��
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
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON) //�Ѿ��ڴ�λ
			{
				CurrentOp->nResult = 0xffff;
			} 
			else
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_CW);   //���е��
				SetOpTimeDelay(CurrentOp,3000); 	//��ʱ3S
				CurrentOp->nStep = STEP1;
			}
		break;	
		case STEP1:
			if (input_get_one(SN_CLAMP_LEFT_OPEN) == SENSOR_TYPE_BEAM_ON) //���е���λ
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //ֹͣ���
				CurrentOp->nResult = 0xffff;
			} 
			else if (CheckOpTimeDelay(CurrentOp))
			{
				dm_ctrl_one(DM_CLAMP_LEFT,DM_BRAKE); //ֹͣ���
				CurrentOp->nResult = ERROR_LEFT_CLAMP_OPEN_FAIL; //10012 ��ߴ��������ϵ��¼��Ӵ�ʧ��
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
Descriptions: ���ӹر�
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
				StartChildOp(CurrentOp, &OpRightClampClose); //�ر��ұ߼���
				StartChildOp(CurrentOp, &OpLeftClampClose);  //�ر���߼���
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
		DealResult(CurrentOp); //�������ʹ��
}
/****************************************************
Function Name: Op_ClampOpen
*****************************************************
Descriptions: ���Ӵ�
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
			StartChildOp(CurrentOp, &OpRightClampOpen); //�ұ߼��Ӵ�
			StartChildOp(CurrentOp, &OpLeftClampOpen);  //��߼��Ӵ�
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
Descriptions: ���Ӹ�λ
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
				StartChildOp(CurrentOp, &OpClampOpen); //���Ӵ� 
				CurrentOp->nStep = STEP1;
			break;	
			case STEP1:	
				if(IsChildOpOK(CurrentOp, &OpClampOpen) == true) //���
				{
					CurrentOp->nStep = STEP2;
				}	
			break;
			case STEP2:	
				StartChildOp(CurrentOp, &OpClampClose); //���ӹر�
				CurrentOp->nStep = STEP3;
			break;
			case STEP3:	
				if(IsChildOpOK(CurrentOp, &OpClampClose) == true) //���
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
Descriptions:  С���ƶ�
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
	static unsigned char ChannelTrolleyCarPos = 0; //����ͨ��С����λ��
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (gTrolleyCarMove_Flag == true) //�ײ����
			{
				gTrolleyCarMove_Flag = false;
				ChannelTrolleyCarPos = gTrolleyCarPosition; 
			}
			else //�ϲ����
			{
				ChannelTrolleyCarPos = cParamBuf[10];
			}
			if(ChannelTrolleyCarPos>5)
			{
				CurrentOp->nResult = ERROR_TROLLEY_CAR_POSITION_PARA_MAX;	  	//10013 ץ��С���ϲ㴫������ݳ����������ֵ
			}		
			CurrentOp->nStep = STEP20;					
		break;
/////////////////////////////////////////////////////////////////////////////
/*����С���ƶ�ʱ��ı���,С���ƶ�ʱ�������Ԥ��λ(˺Ĥλ)�½��ͷ���λ����,
��ѹ��ѹ��������,���ӱպ�,С���ƶ�ʱ��ײ�����µ��ʧ�i��Ƥ���Բ�׼��λ  */
		case STEP20:
			StartChildOp(CurrentOp, &OpPretargetingReset);  //Ԥ��λ��λ
			CurrentOp->nStep = STEP21;
		break;

		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //���ϳ���λ
				CurrentOp->nStep = STEP22;
			}
		break;
		case STEP22:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				StartChildOp(CurrentOp, &OpPresRise);  		//�ü���λ��λ
				CurrentOp->nStep = STEP23;
			}
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);  		//��ѹ��λ��λ
				CurrentOp->nStep = STEP24;
			}
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//��ѹ��λ��λ
				CurrentOp->nStep = STEP25;
			}
		break;
		case STEP25:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				StartChildOp(CurrentOp, &OpClampClose);  		//צ�ӱպ�
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
			if (gTrolleyCarPosition == ENUM_UNKNOW) //λ��δ֪
			{				
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}             	
		break;
		case STEP2:
			StartChildOp(CurrentOp,&OpResetClampCardCar); //��λС��		
			CurrentOp->nStep = STEP3;	
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpResetClampCardCar) == true) //���
			{
					CurrentOp->nStep = STEP4;	
			}
		break;
		case STEP4:
			if(gTrolleyCarMoveCount<100) //���е�һ��������λ����ۻ����
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
		case STEP5:   //���ݴ�������λ���߾��Բ���
			sm_run_abs(SM_CHANNEL,parameter[PAR_SM_CHANNAL_SPEED],iAbsPosTrolleyCarMov[ChannelTrolleyCarPos]);	
			CurrentOp->nStep= STEP6;
		break;
		case STEP6:
			if(IsSmRunFinish(SM_CHANNEL) == true)	//С������˶�
			{
				sm_stop(SM_CHANNEL); //ֹͣ���
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;

		case STEP_SUCCESS:
			gTrolleyCarPosition = ChannelTrolleyCarPos; //ͬ���ϲ㴫������λ��
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
Descriptions:  Ԥ��λ��λ
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
			if(nStatusPretarget == STATUS_ORG) //��־��ԭ��
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_ON)	 //�����������ԭ��
			{
					nStatusPretarget = STATUS_UNKNOW; //���־λ 
					CurrentOp->nStep = STEP20;
			} 
			else //����ԭ��  
			{
				nStatusPretarget = STATUS_UNKNOW;
				sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_BACKWARD,10,6000);	 //��ԭ��
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_ON) //��⵽�ص�ԭ��
			{
				sm_stop(SM_PRETARGETING); //ֹͣ���
				sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_BACKWARD,10,350);		 //����һ��
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_PRETARGETING) == true)
			{
				CurrentOp->nResult = ERROR_PRETARGETING_RETURN_ORIGIN_FAIL;	  //10014 Ԥ��λ����ԭ��ʧ��
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_PRETARGETING) == true)
			{
				nStatusPretarget = STATUS_ORG; //��ԭ��
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_FORWARD,10,400);	 			//�뿪ԭ��
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_PRETARGETING) == true)
			{
				if (input_get_one(SN_PREPOSITION_ORG) == SENSOR_TYPE_BEAM_OFF)	//�뿪ԭ��
				{
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_PRETARGETING_LEAVE_ORIGIN_FAIL; //10015 Ԥ��λ�뿪ԭ��ʧ��
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
Descriptions:  Ԥ��λ����
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
			if(nStatusPretarget == STATUS_POSITION) //������λ
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(nStatusPretarget == STATUS_ORG)	 //��ԭ�㴦
			{
				CurrentOp->nStep = STEP2; 									
			} 
			else //����״̬
			{
				StartChildOp(CurrentOp, &OpPretargetingReset); //Ԥ��λ��λ
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
			sm_run(SM_PRETARGETING,DIR_SM_PRETARGETING_FORWARD,10,parameter[PAR_PRETARGET_UP_POSITION]);	 //�뿪ԭ��	
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
Descriptions:  ���ϳ������λ
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
			if(	nStatusWastestorage == STATUS_ORG) //��־λ��ԭ��
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON)	 //��������⵽��ԭ��
			{
				nStatusWastestorage = STATUS_UNKNOW;
				CurrentOp->nStep = STEP20;
			} 
			else
			{
				nStatusWastestorage = STATUS_UNKNOW;
				sm_run(SM_WASTESTORAGE,SM_CCW,10,2000);	 //��ԭ��
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_WASTESTORAGE); //ֹͣ���
				sm_run(SM_WASTESTORAGE,SM_CCW,10,100);	 //����һ��
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				CurrentOp->nResult = ERROR_WASTESTORAGE_RETURN_ORIGIN_FAIL;	 //10016 �Ͽ�λ����ԭ��ʧ��
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				nStatusWastestorage = STATUS_ORG;  //��ԭ��
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_WASTESTORAGE,SM_CW,10,200);	 			//�뿪ԭ��
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
			if (input_get_one(SN_WASTESTORAGE_ORG) == SENSOR_TYPE_BEAM_OFF)	//�뿪ԭ��
			{
					CurrentOp->nStep = START;
			}else
			{
					CurrentOp->nResult = ERROR_WASTESTORAGE_LEAVE_ORIGIN_FAIL; //10017 �Ͽ�λ�뿪ԭ��ʧ��
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
Descriptions:  ���ϳ�����½�
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
			if(nStatusWastestorage == STATUS_POSITION) //���½�λ
			{
				CurrentOp->nResult = 0xffff;
			}
			else if(nStatusWastestorage == STATUS_ORG) //��ԭ�㴦
			{
				CurrentOp->nStep = STEP2; 									
			} 
			else
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset); //��λ
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
			sm_run(SM_WASTESTORAGE,SM_CW,10, parameter[PAR_WASTESTORAGE_DN_POSTION]); //������һ������
			CurrentOp->nStep = STEP3; 
		break;
		case STEP3:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				nStatusWastestorage = STATUS_POSITION; //���½�λ
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
Descriptions:  Ԥ��λ�������
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
			dm_ctrl_one(DM_PRETARGETING_VACUUMCUP, RELAY_ON); //������̵�����
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
Descriptions:  Ԥ��λ������ر�
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
			dm_ctrl_one(DM_PRETARGETING_VACUUMCUP, RELAY_OFF); //Ԥ��λ������̵����ر�
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
Function Name: Op_SuckVacuumCupOpen
*****************************************************
Descriptions: �����������
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
void Op_SuckVacuumCupOpen(void)
{
 	stOpCtrl *CurrentOp = &OpSuckVacuumCupOpen;

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
			dm_ctrl_one(DM_SUCK_VACUUMCUP, RELAY_ON);  //�����������
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
Function Name: Op_SuckVacuumCupClose
*****************************************************
Descriptions: ����������ر�
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
void Op_SuckVacuumCupClose(void)
{
	stOpCtrl *CurrentOp = &OpSuckVacuumCupClose;
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
			dm_ctrl_one(DM_SUCK_VACUUMCUP, RELAY_OFF); //����������ر�
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
Descriptions: ��·��
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
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_ON);	  //��·�̵�����
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
Descriptions: ��·�ر�
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
			dm_ctrl_one(DM_PNEUMATIC_SWITCH, RELAY_OFF);	  //��·�̵����ر�
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
Descriptions: ��λģ��
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
			nStatusPretarget = STATUS_UNKNOW;		//Ԥ��λ״̬δ֪
			nStatusWastestorage = STATUS_UNKNOW;	//���ϳ�״̬δ֪
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpOpeningPressure);  //����ѹ
			StartChildOp(CurrentOp, &OpOpenGasSwitch);	  //����ѹ
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if((IsChildOpOK(CurrentOp, &OpOpeningPressure) == true) && (IsChildOpOK(CurrentOp, &OpOpenGasSwitch) == true))
			{
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			StartChildOp(CurrentOp, &OpPretargetingReset);  		//Ԥ��λ��λ
			CurrentOp->nStep = STEP4;
	   	break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageReset);  	//���ϳ���λ
				CurrentOp->nStep = STEP5;
			}
	   	break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				StartChildOp(CurrentOp, &OpPresRise);  					//�ü���λ��λ
				CurrentOp->nStep = STEP6;
			}
	   	break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);  		//��ѹ��λ��λ
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);  		//��ѹ��λ��λ
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				StartChildOp(CurrentOp, &OpResetClamp);  		//צ�Ӹ�λ
				CurrentOp->nStep = STEP9;
			}
		break;
		case STEP9:
			if(IsChildOpOK(CurrentOp, &OpResetClamp) == true)
			{
				StartChildOp(CurrentOp, &OpResetClampCardCar);  //С����λ
				CurrentOp->nStep = STEP10;
			}
		break;	
		case STEP10:
			if(IsChildOpOK(CurrentOp, &OpResetClampCardCar) == true)
			{
				StartChildOp(CurrentOp, &OpResetSuckCardCar);  	//����С����λ
				CurrentOp->nStep = STEP11;	
			}
		break;
		case STEP11:
			if (IsChildOpOK(CurrentOp,&OpResetSuckCardCar) == true)
			{
				StartChildOp(CurrentOp,&OpResetSuck);  //���̸�λ
				CurrentOp->nStep = STEP12;
			}	
		break;
		case STEP12:
			if(IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				StartChildOp(CurrentOp,&OpResetBlankCardBox);  //�׿��临λ
				CurrentOp->nStep = STEP13;		
			}
		break;
		case STEP13:
			if(IsChildOpOK(CurrentOp,&OpResetBlankCardBox) == true)
			{
				bFlagReset = TRUE;  //��λ��־
				CurrentOp->nResult = 0xffff;				
			}
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
			if (input_get_one(SN_PRETARGETING_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //Ԥ��λ��⵽�п�
			{
				CurrentOp->nStep = STEP1;					
			}
			else
			{
				CurrentOp->nResult = ERROR_PRETARGETING_NONE_CARD;	//10028 Ԥ��λ�޿�
			}
		break;
	   	case STEP1:
			StartChildOp(CurrentOp, &OpClampClose);		//����צ�ӱպ�
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpClampClose) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);	//Ԥ��λ��λ
				CurrentOp->nStep = STEP3;		
			}
		break;
		case STEP3:
			if(IsChildOpOK(CurrentOp, &OpPretargetingReset) == true)
			{
				StartChildOp(CurrentOp, &OpCoolRise);	//��ѹ��λ
				CurrentOp->nStep = STEP4;
			}
		break;
		case STEP4:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);	//��ѹ��λ
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
			StartChildOp(CurrentOp, &OpClampCardCarMove); //ȥ��ѹ��λ
			SetOpTimeDelay(CurrentOp,500); //��⿨��������ʱ
			CurrentOp->nStep = STEP14;
		break;
		case STEP14:
			if(CheckOpTimeDelay(CurrentOp) == true)
			{
				if (input_get_one(SN_COOL_CHECK_CARD) == SENSOR_TYPE_BEAM_ON) //��⵽�п�
				{
					CurrentOp->nStep = STEP15;	
				}
				else
				{
					sm_stop(SM_CHANNEL);
					CurrentOp->nResult = ERROR_PRETARGETING_FROM_COOL_NONE_CARD; //10029 Ԥ��λ����ѹλ�޿�
				}
			}
		break;
		case STEP15:	
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpWarmDown);	//ȥ��ѹ�½�
				CurrentOp->nStep = STEP16;		
			}
	   	break;
		case STEP16:
			if(IsChildOpOK(CurrentOp, &OpWarmDown) == true)
			{
				SetOpTimeDelay(CurrentOp, parameter[PAR_WARM_TIME]*1000);	//��ѹ��ѹ��ʱ
				CurrentOp->nStep = STEP17;		
			}
	   	break;
		case STEP17:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{
				StartChildOp(CurrentOp, &OpWarmRise);	//ȥ��ѹ����
				CurrentOp->nStep = STEP18;		
			}
		break;
		case STEP18:
			if(IsChildOpOK(CurrentOp, &OpWarmRise) == true)
			{
				gTrolleyCarMove_Flag = true;
				gTrolleyCarPosition = ENUM_INSIDE_COOL;
				StartChildOp(CurrentOp, &OpClampCardCarMove);	//ȥ��ѹ��λ
				SetOpTimeDelay(CurrentOp,500);
				CurrentOp->nStep = STEP19;		
			}
		break;
		case STEP19:
			if(CheckOpTimeDelay(CurrentOp) == true)
			{
				if (input_get_one(SN_WARM_CHECK_CARD) == SENSOR_TYPE_BEAM_ON) //��⵽�п�
				{
					CurrentOp->nStep = STEP20;	
				}
				else
				{
					sm_stop(SM_CHANNEL);
					CurrentOp->nResult = ERROR_WARM_FROM_COOL_NONE_CARD; //10030 ��ѹλ����ѹλ�޿�
				}
			}			
		break;
		case STEP20:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpCoolDown);//ȥ��ѹ�½�
				CurrentOp->nStep = STEP21;		
			}
	   	break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpCoolDown) == true)
			{
				SetOpTimeDelay(CurrentOp, parameter[PAR_COOL_TIME]*1000);//��ѹ��ѹ��ʱ
				CurrentOp->nStep = STEP22;		
			}
	   	break;
		case STEP22:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpCoolRise); //ȥ��ѹ����
				CurrentOp->nStep = STEP23;		
			}
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpCoolRise) == true)
			{
				gTrolleyCarMove_Flag = true;
				gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING;
				StartChildOp(CurrentOp, &OpClampCardCarMove); //��Ԥ��λ��λ
				CurrentOp->nStep = STEP24;		
			}
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);	//����Ԥ��λ����
				CurrentOp->nStep = STEP25;		 
			}
	   	break;
		case STEP25:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp) == true)
			{
				StartChildOp(CurrentOp, &OpClampOpen);	//����צ���ſ�
				CurrentOp->nStep = STEP26;	
			}
		break;
		case STEP26:
			if (input_get_one(SN_PRETARGETING_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //Ԥ��λ��⵽�п�
			{
				CurrentOp->nStep = STEP27;					
			}
			else
			{
				CurrentOp->nResult = ERROR_COOL_FROM_PRETARGETING_NONE_CARD;//10031 ��ѹλ��Ԥ��λ�޿�
			}	
		break;

		case STEP27:
			if(IsChildOpOK(CurrentOp, &OpClampOpen) == true)
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
Descriptions: ��Ԥ��λ+Ԥ��λ����+�򿪼���
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
			if(bFlagReset) //�Ѿ���λ��
			{
				CurrentOp->nStep = STEP2;
			}
			else
			{
				StartChildOp(CurrentOp, &OpResetModule); //��λѹ��ģ��
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
			if(nStatusPretarget == STATUS_ORG)	//ȷ��Ԥ��λ��ԭ�㣬��ֹ��צ��ײ
			{
				CurrentOp->nStep = STEP4;	
			}
			else
			{
				StartChildOp(CurrentOp, &OpPretargetingReset);	//Ԥ��λ��λ
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
			gTrolleyCarMove_Flag = true;  					//С�����б�־
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING; //���е�λ��
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//��Ԥ��λ��λ
			CurrentOp->nStep = STEP5;		
		break;
		case STEP5:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);//����Ԥ��λ����
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp) == true)
			{
				StartChildOp(CurrentOp, &OpClampOpen); //�򿪼���
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
Descriptions: ����+����
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
			if (input_get_one(SN_PRETARGETING_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //Ԥ��λ��⵽�п�
			{
				CurrentOp->nStep = STEP1;					
			}
			else
			{
				CurrentOp->nResult = ERROR_PRETARGETING_FROM_WASTE_NONE_CARD;//10032 Ԥ��λ������λ�޿�
			}
		break;
		case STEP1:
			StartChildOp(CurrentOp, &OpClampClose);	//����צ�ӱպ�
			CurrentOp->nStep = STEP2;
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp, &OpClampClose) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingReset); //Ԥ��λ��λ
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
				StartChildOp(CurrentOp, &OpWasteStorageReset);//��������ƽ̨��λ			
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
			StartChildOp(CurrentOp, &OpPresRise);	//������ѹ��λ
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				CurrentOp->nStep = STEP8;		
			}
		break;
		case STEP8:
			gTrolleyCarMove_Flag = true;  //צ��С�����б�־
			gTrolleyCarPosition = ENUM_INSIDE_CUT_CARD; //����λ��
			StartChildOp(CurrentOp, &OpClampCardCarMove);	//���ü���λ
			SetOpTimeDelay(CurrentOp,1000); //�쿨��ʱ
			CurrentOp->nStep = STEP9;	
		break;
		case STEP9:
			if(CheckOpTimeDelay(CurrentOp) == true)
			{
				if (input_get_one(SN_WASTER_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //����λ��⵽�п�
				{
					CurrentOp->nStep = STEP10;					
				}
				else
				{
					CurrentOp->nResult = ERROR_WASTE_FROM_PRESS_NONE_CARD;//10033 ����λ������λ�޿�
				}
			}		
		break;
		case STEP10:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP11;		 
			}
	   	break;
		case STEP11:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpPresDown);	//�ü��½�
				CurrentOp->nStep = STEP12;		
			}
		break;
		case STEP12:
			if(IsChildOpOK(CurrentOp, &OpPresDown) == true)
			{					
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP13;	
			}
		break;
		case STEP13:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpPresRise);	//�ü�����
				CurrentOp->nStep = STEP14;		
			}
		break;
		case STEP14:
			if(IsChildOpOK(CurrentOp, &OpPresRise) == true)
			{
				CurrentOp->nStep = STEP15;		
			}
		break;
		case STEP15:
			gTrolleyCarMove_Flag = true; 
			gTrolleyCarPosition = ENUM_INSIDE_WASTE;
			StartChildOp(CurrentOp, &OpClampCardCarMove);//�����ϳ���λ
			CurrentOp->nStep = STEP16;	
		break;
		case STEP16:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				SetOpTimeDelay(CurrentOp, 1000);			
				CurrentOp->nStep = STEP17;		 
			}
	   	break;
///////////////////////////////�ȴ򿪼���Ȼ������ѹ�ѷ���ѹ����ȥ/////////////////////////////
		case STEP17:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp, &OpClampOpen);//�򿪼���
				CurrentOp->nStep = STEP18;
			}
		break;
		case STEP18:
			if (IsChildOpOK(CurrentOp,&OpClampOpen) == true)
			{
				StartChildOp(CurrentOp, &OpWasteStorageDown);	//��������ƽ̨�½�			
				CurrentOp->nStep = STEP19;			
			}
			
		break;
		case STEP19:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageDown) == true)
			{
				SetOpTimeDelay(CurrentOp, 1000);
				CurrentOp->nStep = STEP20;
			}
		break;
		case STEP20:
			if (CheckOpTimeDelay(CurrentOp))
			{
				StartChildOp(CurrentOp,&OpWasteStorageReset); //����ƽ̨����
				CurrentOp->nStep = STEP21;		
			}
		break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpWasteStorageReset) == true)
			{
				CurrentOp->nStep = STEP22;
			}
		break;
		case STEP22:
			gTrolleyCarMove_Flag = true; 					 //���б�־
			gTrolleyCarPosition = ENUM_INSIDE_PRETARGETING;  //����λ��
			StartChildOp(CurrentOp, &OpClampCardCarMove);	 //��Ԥ��λ��λ
			CurrentOp->nStep = STEP23;		
		break;
		case STEP23:
			if(IsChildOpOK(CurrentOp, &OpClampCardCarMove) == true)
			{
				StartChildOp(CurrentOp, &OpPretargetingUp);//����Ԥ��λ����
				CurrentOp->nStep = STEP24;
			}
		break;
		case STEP24:
			if(IsChildOpOK(CurrentOp, &OpPretargetingUp) == true)
			{
				StartChildOp(CurrentOp, &OpClampOpen); //�򿪼���
				CurrentOp->nStep = STEP25;
			}
		break;
		case STEP25:
			if(IsChildOpOK(CurrentOp, &OpClampOpen) == true)
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
Descriptions:  ����ֹͣ�Ƽ�������
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
			OpCuttingPlatformWork.bEn = false; 	 //ʧ�ܼ�������
			OpWarmCoolMakeCard.bEn = false;			   	 //ʧ����ѹ����
			sm_stop(SM_CHANNEL);			     //ֹͣ���
			StartChildOp(CurrentOp,&OpWarmRise); //������ѹ
			CurrentOp->nStep = STEP1;
		break;
		case STEP1:
			if(IsChildOpOK(CurrentOp,&OpWarmRise) == true)
			{
				StartChildOp(CurrentOp,&OpCoolRise); //��ѹ����
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if(IsChildOpOK(CurrentOp,&OpCoolRise) == true)
			{
				StartChildOp(CurrentOp,&OpPresRise); //��������
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
Descriptions:  ����С���ƶ�
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
	static unsigned char SuckCardCarPos = 0; //����ͨ��С����λ��
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:				
			if (gSuckCardCarMove_Flag == true) //�ײ����
			{
				gSuckCardCarMove_Flag= false;
				SuckCardCarPos = gSuckCardCarPosition; //ȡ���ײ���õ�λ��
			}
			else //�ϲ����
			{
				SuckCardCarPos = cParamBuf[10]; //����λ
			}
			if(SuckCardCarPos > 6)
			{
				CurrentOp->nResult = ERROR_SUCK_CARD_CAR_POSITION_PARA_MAX;   //10018 ����С���ϲ㴫������ݳ����������ֵ
			}		
			CurrentOp->nStep = STEP20;					
		break;	
 		case STEP20: //����,����С���ƶ�ǰ�����̸�λ,����ײ��
			StartChildOp(CurrentOp, &OpResetSuck);  //���̸�λ 
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpResetSuck) == true)
			{
				CurrentOp->nStep = STEP1;
			}
		break; 
		case STEP1:
			if (gSuckCardCarPosition == ENUM_SUCK_UNKNOW) //λ��δ֪
			{				
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}             	
		break;
		case STEP2:
			StartChildOp(CurrentOp,&OpResetSuckCardCar); //��λ����С��		
			CurrentOp->nStep = STEP3;	
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpResetSuckCardCar) == true) //���
			{
				CurrentOp->nStep = STEP4;	
			}
		break;
		case STEP4:
			if(gSuckCardCarMoveCount < 100) //���е�һ��������λ����ۻ����
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
		case STEP5:   //���ݴ�������λ���߾��Բ���
			sm_run_abs(SM_SUCK_CARD_CAR,parameter[PAR_SUCK_CARD_CAR_SPEED],iAbsSuckCardCarMov[SuckCardCarPos]);	
			CurrentOp->nStep= STEP6;
		break;
		case STEP6:
			if(IsSmRunFinish(SM_SUCK_CARD_CAR) == true)	//С������˶�
			{
				sm_stop(SM_SUCK_CARD_CAR); //ֹͣ���
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;

		case STEP_SUCCESS:
			gSuckCardCarPosition = SuckCardCarPos; //ͬ���ϲ㴫������λ��
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
Descriptions:  ��λ����С��
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
			StartChildOp(CurrentOp, &OpResetSuck); //���̸�λ
			CurrentOp->nStep = STEP21;
	   	break;
	
		case STEP21:
			if(IsChildOpOK(CurrentOp, &OpResetSuck) == true)
			{
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:

			if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_OFF) 	//����ԭ��	
			{
				sm_run(SM_SUCK_CARD_CAR, DIR_SM_SUCK_CARD_CAR_BACKWARD,parameter[PAR_SUCK_CARD_CAR_SPEED], 50000); //�������һ�ξ���
				CurrentOp->nStep = STEP5;
			}
			else if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_ON) //��ԭ��
			{
				sm_run(SM_SUCK_CARD_CAR, DIR_SM_SUCK_CARD_CAR_FORWARD,parameter[PAR_SUCK_CARD_CAR_SPEED], 1000); //��ǰ����һ�ξ���
				CurrentOp->nStep = STEP2;
			}			
		break;
		case STEP2:
			if(IsSmRunFinish(SM_SUCK_CARD_CAR) == true)  
			{
				if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_OFF) //�뿪ԭ��
				{
					SetOpTimeDelay(CurrentOp,300);   //��ת��ʱ
					CurrentOp->nStep = STEP3;	
				}
				else if(IsSmRunFinish(SM_SUCK_CARD_CAR))        	
				{
					CurrentOp->nResult = ERROR_SUCK_CARD_CAR_LEAVE_ORIGIN_FAIL;  //10019 ����С���뿪ԭ��ʧ��
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
			if (input_get_one(SN_SUCK_CARD_CAR_ORG) == SENSOR_TYPE_BEAM_ON) //��ԭ��
			{
				sm_stop(SM_SUCK_CARD_CAR); 			//ֹͣ���
				CurrentOp->nStep = STEP6;
			}	
			else if (IsSmRunFinish(SM_SUCK_CARD_CAR) == true) //����������
			{
				CurrentOp->nResult = ERROR_SUCK_CARD_CAR_RETURN_ORIGIN_FAIL; //10020 ����С���뿪ԭ��ʧ��			
			}		
		break;
		case STEP6:
			sm_run(SM_SUCK_CARD_CAR, DIR_SM_SUCK_CARD_CAR_BACKWARD, parameter[PAR_SUCK_CARD_CAR_SPEED], 1);  //����������һ�ξ���
			CurrentOp->nStep = STEP7;
		break;
		case STEP7:
			if (IsSmRunFinish(SM_CHANNEL) == true)
			{
				CurrentOp->nStep = STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			sm_set_abs_zero(SM_SUCK_CARD_CAR); //���㲽������Ĳ���
			gSuckCardCarPosition = ENUM_SUCK_ORIGIN; //����С����ԭ��
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_ResetSuck
*****************************************************
Descriptions:  ���̸�λ
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
void Op_ResetSuck(void)
{
	stOpCtrl *CurrentOp = &OpResetSuck;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(	(nStatusSuck == ENUM_SUCK_ORIGIN) 
				&& (input_get_one(SN_SUCK_ORG) == SENSOR_TYPE_BEAM_ON)) //��־λ��ԭ��,����������ԭ��
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_SUCK_ORG) == SENSOR_TYPE_BEAM_ON)	 //��������⵽��ԭ��
			{
				nStatusSuck = ENUM_SUCK_UNKNOW;
				CurrentOp->nStep = STEP20;
			} 
			else
			{
				nStatusSuck = ENUM_SUCK_UNKNOW;
				sm_run(SM_SUCK,DIR_SM_SUCK_UP,parameter[PAR_SUCK_SPEED],35000);	 //��ԭ��
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_SUCK_ORG) == SENSOR_TYPE_BEAM_ON) //��������⵽��ԭ��
			{
				sm_stop(SM_SUCK); //ֹͣ���
				sm_run(SM_SUCK,DIR_SM_SUCK_UP,parameter[PAR_SUCK_SPEED],10);	 //����һ��
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_SUCK) == true)
			{
				CurrentOp->nResult = ERROR_SUCK_RETURN_ORIGIN_FAIL;	 //10021 ���̷���ԭ��ʧ��
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_SUCK) == true)
			{
				nStatusSuck = ENUM_SUCK_ORIGIN;  //��ԭ��
				sm_set_abs_zero(SM_SUCK); //���㲽������Ĳ���
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_SUCK,DIR_SM_SUCK_DOWN,50,200);  //�뿪ԭ��
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_SUCK) == true)
			{
				if (input_get_one(SN_SUCK_ORG) == SENSOR_TYPE_BEAM_OFF)	//�뿪ԭ��
				{
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_SUCK_LEAVE_ORIGIN_FAIL;	//10022 �����뿪ԭ��ʧ��
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
Descriptions:  �����½�
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
	static unsigned char SuckPos = 0; //�������̵�λ��
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:	
			if (gSuckMove_Flag == true) //�ײ����
			{
				gSuckMove_Flag= false;
				SuckPos = gSuckPosition; //ȡ���ײ���õ�λ��
			}
			else //�ϲ����
			{
				SuckPos = cParamBuf[10]; //����λ
			}
			if(SuckPos > 6)
			{
				CurrentOp->nResult = ERROR_SUCK_POSITION_PARA_MAX;//10023 �������ϲ㴫������ݳ����������ֵ
			}		
			CurrentOp->nStep = STEP1;					
		break;	
		case STEP1:
			if (gSuckPosition == ENUM_SUCK_UNKNOW) //λ��δ֪
			{				
				CurrentOp->nStep = STEP2;
			}
			else
			{
				CurrentOp->nStep = STEP4;
			}             	
		break;
		case STEP2:
			StartChildOp(CurrentOp,&OpResetSuck); //��λ����		
			CurrentOp->nStep = STEP3;	
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true) //���
			{
				CurrentOp->nStep = STEP4;	
			}
		break;
		case STEP4:
			if (SuckPos == gSuckCardCarPosition) //���̺�����С��λ�ö�Ӧ
			{
				CurrentOp->nStep = STEP6;				
			}
			else
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition  = SuckPos; //���е����������Ӧ��λ��
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���ʱ�ſ�λ
				CurrentOp->nStep = STEP5;	
			}
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{		
				CurrentOp->nStep = STEP6;	
			}
		break;
		case STEP6:   //���ݴ�������λ���߾��Բ���
			sm_run_abs(SM_SUCK,parameter[PAR_SUCK_SPEED],iAbsSuckMove[SuckPos]*24);	
			CurrentOp->nStep= STEP7;
		break;
		case STEP7:
			if(IsSmRunFinish(SM_SUCK) == true)	//С������˶�
			{
				sm_stop(SM_SUCK); //ֹͣ���
				CurrentOp->nStep= STEP_SUCCESS;
			}
		break;
		case STEP_SUCCESS:
			nStatusSuck = STATUS_POSITION;	
			gSuckPosition = SuckPos; //ͬ���ϲ㴫������λ��
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
Descriptions: �׿��临λ
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
void Op_ResetBlankCardBox(void)
{
	stOpCtrl *CurrentOp = &OpResetBlankCardBox;
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;	
	}	
	switch (CurrentOp->nStep)
	{
		case START:
			if(	nStatusBlankCardBox == STATUS_ORG) //��־λ��ԭ��
			{
				CurrentOp->nResult = 0xffff;
			}
			else if (input_get_one(SN_BLANK_CARD_BOX_ORG) == SENSOR_TYPE_BEAM_ON)  //��������⵽��ԭ��
			{
				nStatusBlankCardBox = STATUS_UNKNOW;
				CurrentOp->nStep = STEP20;
			} 
			else
			{
				nStatusBlankCardBox = STATUS_UNKNOW;
				sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_DOWN,parameter[PAR_SUCK_SPEED],80000);	 //��ԭ��
				CurrentOp->nStep = STEP1;
			}
		break;
		case STEP1:
			if (input_get_one(SN_BLANK_CARD_BOX_ORG) == SENSOR_TYPE_BEAM_ON)
			{
				sm_stop(SM_BLANK_CARD_BOX); //ֹͣ���
				sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_DOWN,parameter[PAR_SUCK_SPEED],5);	 //����һ��
				CurrentOp->nStep = STEP2;
			} 
			else if(IsSmRunFinish(SM_BLANK_CARD_BOX) == true)
			{
				CurrentOp->nResult = ERROR_BLANK_CARD_BOX_RESET_FAIL;  //10024 �׿��临λʧ��
			}
		break;
		case STEP2:
			if(IsSmRunFinish(SM_WASTESTORAGE) == true)
			{
				nStatusBlankCardBox = STATUS_ORG;  //��ԭ��
				CurrentOp->nResult = 0xffff;
			}
		break;
		case STEP20:
			sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_UP,parameter[PAR_SUCK_SPEED],500);	 //�뿪ԭ��
			CurrentOp->nStep = STEP21;
		break;
		case STEP21:
			if(IsSmRunFinish(SM_BLANK_CARD_BOX) == true)
			{
				if (input_get_one(SN_BLANK_CARD_BOX_ORG) == SENSOR_TYPE_BEAM_OFF)	//�뿪ԭ��
				{
					CurrentOp->nStep = START;
				}
				else
				{
					CurrentOp->nResult = ERROR_BLANK_CARD_BOX_LEAVE_ORIGIN_FAIL; //10025 �׿����뿪ԭ��ʧ��
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
Descriptions:  �׿�������
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
			if(nStatusBlankCardBox == STATUS_UNKNOW)
			{
				StartChildOp(CurrentOp,&OpResetBlankCardBox);
				CurrentOp->nStep = STEP1;
			}
			else
			{
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP1:
			if (IsChildOpOK(CurrentOp,&OpResetBlankCardBox) == true)
			{
				CurrentOp->nStep = STEP2;
			}
			
		break;
		case STEP2:
			if(input_get_one(SN_BLANK_CARD_BOX_CHECK) == SENSOR_TYPE_REFLECT_ON) //�п�
			{
				CurrentOp->nResult = 0xffff;
			}
			else //����״̬
			{
				sm_run(SM_BLANK_CARD_BOX,DIR_SM_BLANK_CARD_BOX_UP,parameter[PAR_SUCK_SPEED],parameter[PAR_BLANK_CARD_BOX_UP]*20); //����һ������
				CurrentOp->nStep = STEP3;
			}	
		break;
		case STEP3:
			if(input_get_one(SN_BLANK_CARD_BOX_UP) == SENSOR_TYPE_BEAM_ON) //���ﶥ��
			{
				sm_stop(SM_BLANK_CARD_BOX); //ֹͣ���
				nStatusBlankCardBox = STATUS_POSITION;  //��ԭ��
				CurrentOp->nResult = ERROR_BLANK_CARD_BOX_EMPTY;  //10026 �׿����
			}
			else if(input_get_one(SN_BLANK_CARD_BOX_CHECK) == SENSOR_TYPE_REFLECT_ON) //�п�
			{

				sm_stop(SM_BLANK_CARD_BOX); //ֹͣ���
				CurrentOp->nStep = STEP_SUCCESS;	
			}
		break;
		case STEP_SUCCESS:
			nStatusBlankCardBox = STATUS_POSITION;  //��ԭ��
			CurrentOp->nResult = 0xffff;
		break;
		default:
		break;
	}	
	DealResult(CurrentOp);
}
/****************************************************
Function Name: Op_SendCardToPretargeting
*****************************************************
Descriptions:  �Ϳ���Ԥ��λ
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
void Op_SendCardToPretargeting(void)
{
	stOpCtrl *CurrentOp = &OpSendCardToPretargeting;
	static unsigned char count = 0; //��������
	if((CurrentOp->bEn == false) || (CurrentOp->bFlagPause) == true)
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:
			StartChildOp(CurrentOp,&OpBlankCardBoxUp); //�׿�������
			CurrentOp->nStep = STEP1;				
		break;
		case STEP1:
			if (IsChildOpOK(CurrentOp,&OpBlankCardBoxUp) == true)
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition   = ENUM_BLANK; //�հ׿�λ
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ����׿�λ
				CurrentOp->nStep = STEP2;					
			}
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_BLANK; //�հ׿�λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½����׿�λ
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupOpen);	//�����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ�����
				CurrentOp->nStep = STEP4;
			}	
		break;
		case STEP4:
			if ((IsChildOpOK(CurrentOp,&OpSuckVacuumCupOpen) == true)
				 && (CheckOpTimeDelay(CurrentOp) == true))
			{
				StartChildOp(CurrentOp,&OpResetSuck); //���̸�λ����
				CurrentOp->nStep = STEP5;
			}
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				if ((input_get_one(SN_SUCK_BAROMETER) == SENSOR_TYPE_VACUUM_ON) //���ѹ������
					&& (input_get_one(SN_SUCK_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON)) //���䴫�������
				{
					CurrentOp->nStep = STEP6;//������Ƭ						
				}
				else
				{
					if(count < 3) //����3��
					{
						count++;
						CurrentOp->nStep = STEP2; //�����½����׿�λ����
					}
					else
					{
						count = 0;
						dm_ctrl_one(DM_SUCK_VACUUMCUP, RELAY_OFF); //����������ر�
						CurrentOp->nResult = ERROR_SUCK_VACUUM_CARD_FAIL;//10027 �����������ʧ��
					}
					
				}
			}
		break;	
		case STEP6:
			count = 0;
			gSuckCardCarMove_Flag = true;
			gSuckCardCarPosition   = ENUM_PRETARGETING; //Ԥ��λ
			StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���Ԥ��λ
			CurrentOp->nStep = STEP7;	
		break;		
		case STEP7:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				if ((input_get_one(SN_SUCK_BAROMETER) == SENSOR_TYPE_VACUUM_ON) //���ѹ������
					&& (input_get_one(SN_SUCK_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON)) //���䴫�������
				{
					gSuckMove_Flag = true;
					gSuckPosition   = ENUM_PRETARGETING; //Ԥ��λ��λ
					StartChildOp(CurrentOp,&OpSuckMove); //�����½���Ԥ��λ
					CurrentOp->nStep = STEP8;
				}
				else
				{
					dm_ctrl_one(DM_SUCK_VACUUMCUP, RELAY_OFF); //����������ر�
					CurrentOp->nResult = ERROR_SUCK_CARD_DROP_OUT;//10039 ���̿�Ƭ����					
				}
				
			}
		break;
		case STEP8:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupClose);	//�ر����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ���Ƭ����
				CurrentOp->nStep = STEP9;
			}			
		break;
		case STEP9:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{			
				StartChildOp(CurrentOp,&OpResetSuck); //��λ����
				SetOpTimeDelay(CurrentOp,2000); //��ʱ1S
				CurrentOp->nStep = STEP10;				
			}	
		break;
		case STEP10:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{	
				if (input_get_one(SN_SUCK_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON)
				{
					sm_stop(SM_SUCK);
					CurrentOp->nResult = ERROR_SUCK_FREE_CARD_FAIL;//10034 �����ͷſ�ʧ��
				}
				else
				{
					CurrentOp->nStep = STEP11;
				}
			}			
		break;
		case STEP11:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
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
Function Name: Op_PretargetingToTemporary
*****************************************************
Descriptions:  Ԥ��λ������λ
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
 void Op_PretargetingToAccessory(void)
{
	stOpCtrl *CurrentOp = &OpPretargetingToAccessory;
	static unsigned char count = 0; //��������
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}	
	switch (CurrentOp->nStep)
	{
		case START:	
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_PRETARGETING; //Ԥ��λ��λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½���Ԥ��λ
				CurrentOp->nStep = STEP1;	
		break;
		case STEP1:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupOpen);	//�����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ�����
				CurrentOp->nStep = STEP2;
			}	
		break;
		case STEP2:
			if ((IsChildOpOK(CurrentOp,&OpSuckVacuumCupOpen) == true)
				 && (CheckOpTimeDelay(CurrentOp) == true))
			{
				StartChildOp(CurrentOp,&OpResetSuck); //���̸�λ����
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				if ((input_get_one(SN_SUCK_BAROMETER) == SENSOR_TYPE_VACUUM_ON) //��ձ���ͷ��䴫�������
					&& (input_get_one(SN_SUCK_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON))//������Ƭ
				{
					CurrentOp->nStep = STEP4;						
				}
				else
				{
					if(count < 3) //����3��
					{
						count++;
						CurrentOp->nStep = START; //�����½����׿�λ����
					}
					else
					{
						count = 0;
						CurrentOp->nResult = ERROR_PRETARGETING_SUCK_VACUUM_CARD_FAIL;//10035 Ԥ��λ����ʧ��
					}
				}
			}
		break;	
		case STEP4:
			gSuckCardCarMove_Flag = true;
			gSuckCardCarPosition   = ENUM_ACCESSORY; //����λ
			StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ�������λ
			CurrentOp->nStep = STEP5;	
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_ACCESSORY;    //����λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½�������λ
				CurrentOp->nStep = STEP6;	
			}
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupClose);	//�ر����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ���Ƭ����
				CurrentOp->nStep = STEP7;
			}	
		break;				
		case STEP7:
			if ((IsChildOpOK(CurrentOp,&OpSuckVacuumCupClose) == true)
				 && (CheckOpTimeDelay(CurrentOp) == true))
			{
				StartChildOp(CurrentOp,&OpResetSuck); //���̸�λ����
				SetOpTimeDelay(CurrentOp,1000); //��ʱ��⿨Ƭ�Ƿ����
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{	
				if (input_get_one(SN_SUCK_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON) //���̷��䴫������⵽��Ƭ
				{
					sm_stop(SM_SUCK); //ֹͣ���̵��
					CurrentOp->nResult = ERROR_ACCESSORY_SUCK_FREE_CARD_FAIL;//10036 ����λ�����ͷſ�ʧ��
				}
				else
				{
					CurrentOp->nStep = STEP9;
				}
			}			
		break;
		case STEP9:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition   = ENUM_PRETARGETING; //Ԥ��λ
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���Ԥ��λ
				CurrentOp->nStep = STEP10;	
			}	
		break;	
		case STEP10:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				CurrentOp->nResult = 0xffff;	
			}
		break;
	}
	DealResult(CurrentOp);
} 
/****************************************************
Function Name: Op_AutoMakeCard
*****************************************************
Descriptions:  �Զ��ƿ�
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
	static unsigned char count = 0; //��������
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:	
			count = 0;
			StartChildOp(CurrentOp,&OpGotoLoadingPlatform); //��Ԥ��λ������Ԥ��λƽ̨�ʹ򿪼���
			CurrentOp->nStep = STEP1;
		break;
		case STEP1: 
			if (IsChildOpOK(CurrentOp,&OpGotoLoadingPlatform) == true)
			{
				StartChildOp(CurrentOp,&OpSendCardToPretargeting); //�Զ�����
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2: 
			if (IsChildOpOK(CurrentOp,&OpSendCardToPretargeting) == true)
			{
				count++;
				CurrentOp->nStep = STEP3;
			}
		break;
		case STEP3: 
			if (count > 6)
			{
				count = 0;
				CurrentOp->nStep = STEP4;
			}
			else
			{
				CurrentOp->nStep = STEP1;
			}
		break; 
		case STEP4:
			gSuckMove_Flag = true;
			gSuckPosition   = ENUM_PRETARGETING; //Ԥ��λ��λ
			StartChildOp(CurrentOp,&OpSuckMove); //�����½���Ԥ��λ
			CurrentOp->nStep = STEP5;				
		break;
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpClampClose); //�رռ���
				CurrentOp->nStep = STEP6;
			}
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpClampClose) == true)
			{
				StartChildOp(CurrentOp,&OpResetSuck); //��λ����
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				StartChildOp(CurrentOp,&OpWarmCoolMakeCard); //����ѹ�ƿ�
				CurrentOp->nStep = STEP8;
			}
		break;
		case STEP8:
			if (IsChildOpOK(CurrentOp,&OpWarmCoolMakeCard) == true)
			{
				StartChildOp(CurrentOp,&OpPretargetingToAccessory); //Ԥ��λ������λ
				CurrentOp->nStep = STEP9;
			}	
		break;	
		case STEP9:
			if (IsChildOpOK(CurrentOp,&OpPretargetingToAccessory) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_PRETARGETING; //Ԥ��λ��λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½���Ԥ��λ
				CurrentOp->nStep = STEP10;	
			}
		break;
		case STEP10:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupOpen);	//�����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ�����
				CurrentOp->nStep = STEP11;
			}	
		break;
		case STEP11:
			if ((IsChildOpOK(CurrentOp,&OpSuckVacuumCupOpen) == true)
				 && (CheckOpTimeDelay(CurrentOp) == true))
			{
				StartChildOp(CurrentOp,&OpResetSuck); //���̸�λ����
				CurrentOp->nStep = STEP12;
			}
		break;
	case STEP12:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition   = ENUM_TEMPORARY; //��ʱ�ſ�λ
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���ʱ�ſ�λ
				CurrentOp->nStep = STEP13;	
			}
		break;
		case STEP13:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_TEMPORARY;    //��ʱ�ſ�λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½�����ʱ�ſ�λ
				CurrentOp->nStep = STEP14;	
			}
		break;
		case STEP14:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupClose);	//�ر����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ���Ƭ����
				CurrentOp->nStep = STEP15;
			}	
		break;				
		case STEP15:
			if ((IsChildOpOK(CurrentOp,&OpSuckVacuumCupClose) == true)
				 && (CheckOpTimeDelay(CurrentOp) == true))
			{
				StartChildOp(CurrentOp,&OpResetSuck); //���̸�λ����
				CurrentOp->nStep = STEP16;
			}
		break;
		case STEP16:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition   = ENUM_PRETARGETING; //Ԥ��λ
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���Ԥ��λ
				CurrentOp->nStep = STEP17;	
			}	
		break;					
		case STEP17:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				StartChildOp(CurrentOp,&OpPretargetingToAccessory); //Ԥ��λ������λ
				CurrentOp->nStep = STEP18;
			}
		break;
	case STEP18:
			if (IsChildOpOK(CurrentOp,&OpPretargetingToAccessory) == true)
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition   = ENUM_TEMPORARY; //��ʱ�ſ�λ
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���ʱ�ſ�λ
				CurrentOp->nStep = STEP19;	
			}
		break;
		case STEP19:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_TEMPORARY;    //��ʱ�ſ�λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½�����ʱ�ſ�λ
				CurrentOp->nStep = STEP20;	
			}
		break;
		case STEP20:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupOpen);	//�����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ���Ƭ��ס
				CurrentOp->nStep = STEP21;
			}	
		break;				
		case STEP21:
			if ((IsChildOpOK(CurrentOp,&OpSuckVacuumCupOpen) == true)
				 && (CheckOpTimeDelay(CurrentOp) == true))
			{
				StartChildOp(CurrentOp,&OpResetSuck); //���̸�λ����
				CurrentOp->nStep = STEP22;
			}
		break;
		case STEP22:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition   = ENUM_PRETARGETING; //Ԥ��λ
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���Ԥ��λ
				CurrentOp->nStep = STEP23;	
			}	
		break;	
		case STEP23:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_PRETARGETING; //Ԥ��λ��λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½���Ԥ��λ
				CurrentOp->nStep = STEP24;	
			}
		break;
		case STEP24:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupClose);	//�ر����������
				CurrentOp->nStep = STEP25;
			}	
		break;
		case STEP25:
			if (IsChildOpOK(CurrentOp,&OpSuckVacuumCupClose) == true)
			{
				StartChildOp(CurrentOp,&OpClampClose);	//���ӱպ�
				CurrentOp->nStep = STEP26;
			}	
		break;
		case STEP26:
			if (IsChildOpOK(CurrentOp,&OpClampClose) == true)
			{
				StartChildOp(CurrentOp,&OpResetSuck);	//��λ����
				CurrentOp->nStep = STEP27;
			}	
		break;
		case STEP27:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				StartChildOp(CurrentOp,&OpCuttingPlatformWork);	//��������
				CurrentOp->nStep = STEP28;
			}	
		break;
		case STEP28:
			if (IsChildOpOK(CurrentOp,&OpCuttingPlatformWork) == true)
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
Descriptions:  �Զ��ƿ�
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
void Op_ReturnCard(void)
{
	stOpCtrl *CurrentOp = &OpReturnCard;
	static unsigned char count = 0; //��������
	if ((CurrentOp->bEn == false) || (CurrentOp->bFlagPause == true))
	{
		return;
	}
	switch (CurrentOp->nStep)
	{
		case START:	
			gSuckCardCarMove_Flag = true;
			gSuckCardCarPosition   = ENUM_PRETARGETING; //Ԥ��λ
			StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ���Ԥ��λ
			CurrentOp->nStep = STEP1;	
		break;		
		case STEP1:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_PRETARGETING; //Ԥ��λ��λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½���Ԥ��λ
				CurrentOp->nStep = STEP2;
			}
		break;
		case STEP2:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupOpen);	//�����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ���Ƭ��ס
				CurrentOp->nStep = STEP3;
			}			
		break;
		case STEP3:
			if ((CheckOpTimeDelay(CurrentOp) == true) 
				&& (IsChildOpOK(CurrentOp,&OpSuckVacuumCupOpen) == true))
			{			
				StartChildOp(CurrentOp,&OpResetSuck); //��λ����
				CurrentOp->nStep = STEP4;				
			}	
		break;
		case STEP4:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				if ((input_get_one(SN_SUCK_BAROMETER) == SENSOR_TYPE_VACUUM_ON) 
					&& (input_get_one(SN_SUCK_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON))//������Ƭ
				{
					CurrentOp->nStep = STEP5;						
				}
				else
				{
					if(count < 3) //����3��
					{
						count++;
						CurrentOp->nStep = STEP1; //�����½����׿�λ����
					}
					else
					{
						count = 0;
						dm_ctrl_one(DM_SUCK_VACUUMCUP, RELAY_OFF); //����������ر�
						CurrentOp->nResult = ERROR_RETURN_CARD_SUCK_CARD_FAIL;//10037 ���̻�������ʧ��
					}
				}
			}
		break;	
		case STEP5:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				gSuckCardCarMove_Flag = true;
				gSuckCardCarPosition   = ENUM_BLANK; //�հ׿�λ
				StartChildOp(CurrentOp,&OpSuckCardCarMove); //����С���ƶ����׿�λ
				CurrentOp->nStep = STEP6;					
			}
		break;
		case STEP6:
			if (IsChildOpOK(CurrentOp,&OpSuckCardCarMove) == true)
			{
				gSuckMove_Flag = true;
				gSuckPosition   = ENUM_BLANK; //�հ׿�λ
				StartChildOp(CurrentOp,&OpSuckMove); //�����½����׿�λ
				CurrentOp->nStep = STEP7;
			}
		break;
		case STEP7:
			if (IsChildOpOK(CurrentOp,&OpSuckMove) == true)
			{
				StartChildOp(CurrentOp,&OpSuckVacuumCupClose);	//�ر����������
				SetOpTimeDelay(CurrentOp,500); //�ȴ���Ƭ����
				CurrentOp->nStep = STEP8;
			}	
		break;
		case STEP8:
			if ((IsChildOpOK(CurrentOp,&OpSuckVacuumCupClose) == true)
				 && (CheckOpTimeDelay(CurrentOp) == true))
			{
				StartChildOp(CurrentOp,&OpResetSuck); //���̸�λ����
				SetOpTimeDelay(CurrentOp,1000); //�ȴ���Ƭ����
				CurrentOp->nStep = STEP9;
			}
		break;
		case STEP9:
			if (CheckOpTimeDelay(CurrentOp) == true)
			{	
				if (input_get_one(SN_SUCK_CHECK_CARD) == SENSOR_TYPE_REFLECT_ON)
				{
					sm_stop(SM_SUCK);
					CurrentOp->nResult = ERROR_RETURN_CARD_SUCK_FREE_CARD_FAIL;//10038 ���̻����ͷſ�ʧ��
				}
				else
				{
					CurrentOp->nStep = STEP10;
				}
			}			
		break;
		case STEP10:
			if (IsChildOpOK(CurrentOp,&OpResetSuck) == true)
			{
				CurrentOp->nResult = 0xffff;
			}	
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
	Op_ResetModule();						//1 ��λ
	Op_ResetClampCardCar(); 				//2 ��λ�п�С��ͨ��
	Op_ClampCardCarMove(); 					//3 �п�С���ƶ�
	Op_ResetClamp();						//4 צ�Ӹ�λ
	Op_ClampOpen();							//5 צ���ſ�
	Op_ClampClose();						//6 צ�ӱպ�
	Op_RightClampClose();			     	//7 �Ҳ���ӹر� 			
  	Op_RightClampOpen();				   	//8 �Ҳ���Ӵ�          
	Op_LeftClampClose();					//9 �����ӹر�
	Op_LeftClampOpen();					   	//10 �����Ӵ�
	Op_OpeningPressure();				   	//11 ѹ����
	Op_ClosingPressure();				   	//12 ѹ����
	Op_WarmRise();						    //13 �����½�
	Op_WarmDown();						   	//14 ��������
	Op_CoolRise();						   	//15 ��ȴ�½�
	Op_CoolDown();						   	//16 ��ȴ����
	Op_PresRise();						   	//17 ��ѹ�½�
	Op_PresDown();						   	//18 ��ѹ����
	Op_PretargetingReset();					//19 Ԥ��λ��λ
	Op_PretargetingUp();					//20 Ԥ��λ����
	Op_WasteStorageReset();		 			//21 ���ϳ���λ
	Op_WasteStorageDown();	   				//22 ���ϳ��½�
	Op_PretargetingVacuumCupOpen();			//23 Ԥ��λ�������
	Op_PretargetingVacuumCupClose();		//24 Ԥ��λ������ر�
	Op_SuckVacuumCupOpen();					//25 �����������
	Op_SuckVacuumCupClose();				//26 ����������ر�
	Op_OpenGasSwitch();			  			//27 ����������
	Op_CloseGasSwitch();				  	//28 �ر���������
	Op_WarmCoolMakeCard();							//29 �ƿ�
	Op_GotoLoadingPlatform();				//30 ȥװ��ƽ̨	
	Op_CuttingPlatformWork();				//31 �ü�ƽ̨����
	Op_ScramProcess();						//32 ����ֹͣ�Ƽ�������
	Op_SuckCardCarMove();					//33 ����С���ƶ�λ��
	Op_ResetSuckCardCar();					//34 ����С����λ
	Op_ResetSuck();							//35 ���̸�λ
	Op_SuckMove();							//36 �����½�
	Op_ResetBlankCardBox();					//37 �׿��临λ
	Op_BlankCardBoxUp();					//38 �׿�������
	Op_SendCardToPretargeting();						//39 �Զ�����
	Op_AutoMakeCard();						//40 �Զ��ƿ�
	Op_ReturnCard();						//41 ����
	Op_PretargetingToAccessory();			//42 Ԥ��λ������λ
}






