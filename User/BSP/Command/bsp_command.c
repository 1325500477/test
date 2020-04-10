//Tab size: 4;	Indent size: 4
#include <string.h>	

#include "bsp.h"
#include "./Parameter/parameter.h"
#include "./Process/process.h"

#include "./Command/command.h"


#include "bsp_command.h"

//因为bsp_command.c文件和process.c文件都用到了这两个变量，所以放到bsp_command.c里面来定义
//从此process.c里面的文件只能用bsp_command.c定义的这两个变量


extern unsigned int txlen;
extern unsigned int cmd_rx_length;


void JumpToAddress(uint32 ApplicationAddress)
{	
	pFunction Jump_To_Application; 
	uint32 JumpAddress;
	uint32 nValue; 
	char cBuf[64];
	memset(cBuf, 0 ,sizeof(cBuf)); 
    
	nValue = (*(vu32*)ApplicationAddress) & 0x2FFF0000; 
    //if (((*(vu32*)ApplicationAddress) & 0x2FFF0000 ) == 0x20000000)
	if (nValue == 0x20000000)
    {		
		sprintf(cBuf, "OK,[StackTop=0x%x]!\n",nValue);
		debug_info(cBuf);
		//NVIC_DeInit();

       JumpAddress = *(vu32*) (ApplicationAddress+4);
       Jump_To_Application = (pFunction) JumpAddress;
       /* Initialize user application's Stack Pointer */
       __set_MSP(*(vu32*) ApplicationAddress); 
       Jump_To_Application();
    }
	else
	{
		sprintf(cBuf, "Fail,[StackTop=0x%x]!\n",nValue);
		debug_info(cBuf);
	}
}

void JumpToApp(void)
{
	//bProgramStart = FALSE;
	//bProgramFinished = TRUE;
	FLASH_Lock();
	debug_info("Jump To APP!\n");
	JumpToAddress(UserAppAddress);
}

void JumpToBootload(void)
{
	//bProgramStart = FALSE;
	//bProgramFinished = FALSE;
	debug_info("Jump To BootLoader!\n");
	JumpToAddress(BootloaderAddress);
}

unsigned char cmd_get_firmware(void)
{
    unsigned char nLen = 0;
	unsigned char i;
	for(i = 0; i < strlen(prj_version); i++)
	{
		comm_send_buf[POS_DATA_0+i] = prj_version[i];
	}
	nLen = strlen(prj_version);
    comm_send_buf[POS_DATA_0+nLen] = '\n';
    nLen = nLen + 1;
	for(i = 0; i < strlen(BSP_version); i++)
	{
		comm_send_buf[POS_DATA_0+nLen+i] = BSP_version[i];
	}
	nLen += strlen(BSP_version);
    return nLen;
}

void test_step_motor(void)
{
    unsigned int nSmSpeed;
	unsigned int nSmStep;

    // 步进马达测试输出 ID(0)+方向(1)+运行/停止(2)+使能(3)+速度(4,5)+步数(6,7)
    if (comm_recv_buf[POS_DATA_0+3] == 0)
    {
        sm_set_enable(comm_recv_buf[POS_DATA_0],SM_ENABLE);  //上使能
        if(comm_recv_buf[POS_DATA_0+2] == 1)
        {
            sm_stop(comm_recv_buf[POS_DATA_0]);
            // 走固定步数
            nSmSpeed = comm_recv_buf[POS_DATA_0+4];
            nSmSpeed <<= 8;
            nSmSpeed += comm_recv_buf[POS_DATA_0+5];
            nSmStep = comm_recv_buf[POS_DATA_0+6];
            nSmStep <<= 8;
            nSmStep += comm_recv_buf[POS_DATA_0+7];
            sm_run(comm_recv_buf[POS_DATA_0],comm_recv_buf[POS_DATA_0+1],nSmSpeed,nSmStep);
        }
        else if (comm_recv_buf[POS_DATA_0+2] == 2)
        {
            // 一直转动
            nSmSpeed = comm_recv_buf[POS_DATA_0+4];
            nSmSpeed <<= 8;
            nSmSpeed += comm_recv_buf[POS_DATA_0+5];
            nSmStep = comm_recv_buf[POS_DATA_0+6];
            nSmStep <<= 8;
            nSmStep += comm_recv_buf[POS_DATA_0+7];
            StepMotorCtr[comm_recv_buf[POS_DATA_0]-1].bRunNoStop = TRUE;
            sm_run(comm_recv_buf[POS_DATA_0],comm_recv_buf[POS_DATA_0+1],nSmSpeed,nSmStep);
        }
        else if (comm_recv_buf[POS_DATA_0+2]==0)
        {
            sm_stop(comm_recv_buf[POS_DATA_0]);
        }
    }
    else if (comm_recv_buf[POS_DATA_0+3]==1)
    {
        sm_stop(comm_recv_buf[POS_DATA_0]);
        sm_set_enable(comm_recv_buf[POS_DATA_0],SM_DISABLE);
    }
}

unsigned char cmd_get_all_info(void)
{
    // 获取所有IO状态(输入、输出以及步进电机)
    unsigned char nLen = 0;
    comm_send_buf[POS_DATA_0 + 0]=m_Input_All_now & 0xFF;
    comm_send_buf[POS_DATA_0 + 1]=(m_Input_All_now>>8) & 0xFF;
    comm_send_buf[POS_DATA_0 + 2]=(m_Input_All_now>>16) & 0xFF;
    comm_send_buf[POS_DATA_0 + 3]=(m_Input_All_now>>24) & 0xFF;
    comm_send_buf[POS_DATA_0 + 4]=0;
    comm_send_buf[POS_DATA_0 + 5]=0;
    comm_send_buf[POS_DATA_0 + 6]=0;
    comm_send_buf[POS_DATA_0 + 7]=0;

    comm_send_buf[POS_DATA_0 + 8]=m_DM_Output_All & 0xFF;
    comm_send_buf[POS_DATA_0 + 9]=(m_DM_Output_All>>8) & 0xFF;
    comm_send_buf[POS_DATA_0 + 10]=(m_DM_Output_All>>16) & 0xFF;
    comm_send_buf[POS_DATA_0 + 11]=(m_DM_Output_All>>24) & 0xFF;
    comm_send_buf[POS_DATA_0 + 12]=0;
    comm_send_buf[POS_DATA_0 + 13]=0;
    comm_send_buf[POS_DATA_0 + 14]=0;
    comm_send_buf[POS_DATA_0 + 15]=0;

    comm_send_buf[POS_DATA_0 + 16]=StepMotorCtr[0].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 17]=StepMotorCtr[1].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 18]=StepMotorCtr[2].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 19]=StepMotorCtr[3].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 20]=StepMotorCtr[4].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 21]=StepMotorCtr[5].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 22]=StepMotorCtr[6].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 23]=StepMotorCtr[7].bMotorRunStatus;
    comm_send_buf[POS_DATA_0 + 24]=0;
    comm_send_buf[POS_DATA_0 + 25]=0;
    comm_send_buf[POS_DATA_0 + 26]=0;
    comm_send_buf[POS_DATA_0 + 27]=0;
    comm_send_buf[POS_DATA_0 + 28]=0;
    comm_send_buf[POS_DATA_0 + 29]=0;
    comm_send_buf[POS_DATA_0 + 30]=0;
    comm_send_buf[POS_DATA_0 + 31]=0;
    nLen = 32;
    return nLen;
}

unsigned char cmd_get_op_info(void)
{
    unsigned char nLen = 0;
	stOpCtrl *OpTemp = NULL;
	unsigned char nCMD = comm_recv_buf[POS_DATA_0];
	comm_send_buf[POS_DATA_0] = gMachineOperation[nCMD]->nResult&0xff;
	comm_send_buf[POS_DATA_0+1] = gMachineOperation[nCMD]->nResult>>8;

//	comm_send_buf[POS_DATA_0] = 117;
//	comm_send_buf[POS_DATA_0+1] = 39;
	
	comm_send_buf[POS_DATA_0+2] = gMachineOperation[nCMD]->nAlarmMode;
	comm_send_buf[POS_DATA_0+3] = gMachineOperation[nCMD]->nStep;
	comm_send_buf[POS_DATA_0+4] = gMachineOperation[nCMD]->nUseTime&0xff;
	comm_send_buf[POS_DATA_0+5] = gMachineOperation[nCMD]->nUseTime>>8;
	comm_send_buf[POS_DATA_0+6] = gMachineOperation[nCMD]->nPreStep;

	OpTemp = gMachineOperation[nCMD];
	while(OpTemp->OpChildError != OpTemp)	// 说明有子动作
	{		
		OpTemp = OpTemp->OpChildError ;		// 找到执行错误的子动作
	}
	comm_send_buf[POS_DATA_0+7] =  OpTemp->nCmd;						// 上传子流程编号，方便错误处理
	comm_send_buf[POS_DATA_0+8] =  OpTemp->bFlagPause;					// 0-运行中；1-暂停
	comm_send_buf[POS_DATA_0+9] =  gMachineOperation[nCMD]->bFlagPopUp;	// 首次出错，上位机可弹窗
	gMachineOperation[nCMD]->bFlagPopUp = 0;

    nLen = 10;
    return nLen;
}

//处理系统命令 大于200,小于255的命令
void ProcessSystemCmd(void)
{
	unsigned char i=0;//, j=0;
    unsigned int nTime;
 	unsigned int nResult;
//	unsigned char InStatus;

//	comm_recv_buf[POS_CMD_TYPE] = 210;

	switch(comm_recv_buf[POS_CMD_TYPE])
	{
    case CMD_PROGRAM:           //#define CMD_PROGRAM         		200			// 进入Bootloader更新程序
		(*((void(*)(void))(BootStart)))();
        break;
	case CMD_UPDATE_FLASH:
		//bProgramStart = TRUE;
		//bProgramFinished = FALSE;
		//FLASH_Unlock();
		//cmd_program();
		debug_info("Please go to Bootloader First!\n");
		comm_send_buf[POS_DATA_0] = 0xFD;	//不允许写
		txlen = 1;
		break;
    case CMD_SET_DC_MOTOR:      //#define CMD_SET_DC_MOTOR			201			// 设置直流马达输出
		dm_ctrl_one(comm_recv_buf[POS_DATA_0],comm_recv_buf[POS_DATA_0+1]);
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
        break;
    case CMD_SET_STEP_MOTOR:    //#define CMD_SET_STEP_MOTOR		202			// 设置步进马达测试
        test_step_motor();
        comm_send_buf[POS_DATA_0] = 0xE0;
	    txlen = 1;
        break;
    case CMD_GET_FIRMWARE:      //#define CMD_GET_FIRMWARE		    203			// 获取APP固件信息（包含BSP固件版本信息）
        txlen = cmd_get_firmware();
        break;
    case CMD_GET_ALL_INFO:      //#define CMD_GET_ALL_INFO			204			// --（可不用）获取所有IO状态（输入、输出以及步进电机）
		txlen = cmd_get_all_info();
        break;
    case CMD_GET_SENSOR:        //#define CMD_GET_SENSOR			205			// 获取传感器信号
      	// 查询传感器
	    comm_send_buf[POS_DATA_0+0] = m_Input_All_now & 0xFF;
        comm_send_buf[POS_DATA_0+1] = (m_Input_All_now>>8) & 0xFF;
        comm_send_buf[POS_DATA_0+2] = (m_Input_All_now>>16) & 0xFF;
        comm_send_buf[POS_DATA_0+3] = (m_Input_All_now>>24) & 0xFF;
        txlen = 4;
        break;
    case CMD_GET_PARAMETER:     //#define CMD_GET_PARAMETER			206			// 获取设备参数配置
	    // 获取参数
	    txlen = get_parameter();
        break;
    case CMD_SET_PARAMETER:     //#define CMD_SET_PARAMETER			207			// 设置设备参数配置
	    // 设置参数_设置设备配置参数
        set_parameter();
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
        break;
    case CMD_GET_DEFAULT_VALUE: //#define CMD_GET_DEFAULT_VALUE		208			// 恢复出厂值
	    // 恢复出厂值
        Get_Default_Value();
        comm_send_buf[POS_DATA_0] = 0xE0;
        txlen = 1;
        break;
    case CMD_SET_DEFAULT_VALUE: //#define CMD_SET_DEFAULT_VALUE		209			// 设置成出厂值
	    // 设置为出厂值
        Set_Default_Value();
        comm_send_buf[POS_DATA_0] = 0xE0;
        txlen = 1;
        break;
    case CMD_GET_OP_INFO:       //#define CMD_GET_OP_INFO 		    210			// 获取某个指定的流程动作结果，时间，状态等相关信息
        txlen = cmd_get_op_info();
        break;
    case CMD_GET_ALL_OP_RESULT: //#define CMD_GET_ALL_OP_RESULT     211         // --（可不用）获取流程结果（批量上传所有动作结果，可能用于需要提速的场合）
	    // 查询结果_获取所有动作执行结果
        for (i=1; i<gnOperationNo; i++ )
	    {
		    nResult = gMachineOperation[i]->nResult;
            comm_send_buf[8+i*2] = nResult >> 8;			
            comm_send_buf[9+i*2] = nResult & 0xFF;
        }
        txlen = (gnOperationNo-1)*2;
        break;
    case CMD_GET_ALL_OP_TIME:   //#define CMD_GET_ALL_OP_TIME       212         // --（可不用）获取流程时间（批量上传所有动作时间）
	    // 查询动作执行时间
        for(i=1; i<gnOperationNo; i++ )
        {
		    nTime = gMachineOperation[i]->nUseTime;
//            comm_send_buf[POS_DATA_0+(i-1)*2] = nTime/256;
//            comm_send_buf[POS_DATA_0+(i-1)*2+1] = nTime%256;
            comm_send_buf[8+i*2] = nTime >> 8;
            comm_send_buf[9+i*2] = nTime & 0xFF;
	    }
	    txlen = (gnOperationNo-1)*2;
        break;
    case CMD_RESUME_OP:         //#define CMD_RESUME_OP				213			// 恢复流程(设备错误后人工恢复继续运行)
		if(ResumeOp(gMachineOperation[comm_recv_buf[POS_DATA_0]], comm_recv_buf[POS_DATA_1]))   //动作成功执行
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
        break;
    case CMD_START_OP_BY_MODE:  //#define CMD_START_OP_BY_MODE      214			// 启动流程从START步开始运行，带有参数（0为直接运行，1为单节拍方式运行)
		//gMachineOperation[comm_recv_buf[POS_DATA_0]]->bRunMode = comm_recv_buf[POS_DATA_1];
        StartOpByMode(gMachineOperation[comm_recv_buf[POS_DATA_0]], comm_recv_buf[POS_DATA_1]);
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
        break;
    case CMD_DO_OP_NEXT_STEP:   //#define CMD_DO_OP_NEXT_STEP		215			// 执行流程下一步
		//gMachineOperation[comm_recv_buf[POS_DATA_0]]->bFlagPause = 0;
        OpUnplugOneStub(gMachineOperation[comm_recv_buf[POS_DATA_0]]);
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
        break;
    case CMD_SET_OP_MODE:       //#define CMD_SET_OP_MODE		    216			// 设置流程模式
        SetOpRunMode(gMachineOperation[comm_recv_buf[POS_DATA_0]], comm_recv_buf[POS_DATA_1]);
        comm_send_buf[POS_DATA_0] = 0xE0;
        txlen = 1;
        break;
	case CMD_SUB_NET:
		if('M' == comm_recv_buf[POS_DATA_0]) 		//别的主板发送过来的请求
		{
			//左移动一个Byte;
			for(i = 0; i < cmd_rx_length+12; i++)
			{
				comm_recv_buf[i] = comm_recv_buf[i+1];
			} 
			ProcessSystemCmd();	
		}
		else if('S' == comm_recv_buf[POS_DATA_0])	//别的主板发送过来的应答
		{
			comm_send_buf[POS_DATA_0] = 0xE0;
			txlen = 1;       
		}
		else
		{
			//未知，需要报错
			comm_send_buf[POS_DATA_0] = 0xE0;
			txlen = 1;       
		}
		break;
	case CMD_HAND_SHAKE:
		comm_send_buf[POS_DATA_0] = 0xE0;
		txlen = 1;
		break;
    default:
        break;
    }
}
