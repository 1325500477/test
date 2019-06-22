#include <string.h>
#include "stm32f10x.h"
#include "bsp.h"
#include "Speed.h" 

#include "IOCfg.h"

#define  MAX_CNT        200

stStepCtr volatile StepMotorCtr[16];

void init_sm_ctl()
{
	unsigned char i = 0;
//	SM1_DIR = 1;
//	SM2_DIR = 1;
//	SM3_DIR = 1;
//	SM4_DIR = 1;
//	SM5_DIR = 1;
//	SM6_DIR = 1;
//	SM7_DIR = 1;
//	SM8_DIR = 1;
//
//	SM1_EN = 1;			//ʹ�ܣ�
//	SM2_EN = 1;
//	SM3_EN = 1;
//	SM4_EN = 1;
//	SM5_EN = 1;
//	SM6_EN = 1;
//	SM7_EN = 1;
//	//SM8_EN = 1;
//
//	SM1_PLUS = 1;
//	SM2_PLUS = 1;
//	SM3_PLUS = 1;
//	SM4_PLUS = 1;
//	SM5_PLUS = 1;
//	SM6_PLUS = 1;
//	SM7_PLUS = 1;
//	SM8_PLUS = 1;

	for(i=0;i<16;i++)
	{
		StepMotorCtr[i].bHaveDisabledFlag=FALSE;
		StepMotorCtr[i].nWaitStableTime=0;
	}
	//memset(m_SM_Status,FREE,8);
}

GPIO_TypeDef * sm_dir_gpio_base[8] = {
	GROUP_SM_DIR1,
	GROUP_SM_DIR2,
	GROUP_SM_DIR3,
	GROUP_SM_DIR4,
	GROUP_SM_DIR5,
	GROUP_SM_DIR6,
	GROUP_SM_DIR7,
	GROUP_SM_DIR8
};
GPIO_TypeDef * sm_pul_gpio_base[8] = {
	GROUP_SM_PUL1,
	GROUP_SM_PUL2,
	GROUP_SM_PUL3,
	GROUP_SM_PUL4,
	GROUP_SM_PUL5,
	GROUP_SM_PUL6,
	GROUP_SM_PUL7,
	GROUP_SM_PUL8
};
GPIO_TypeDef * sm_ena_gpio_base[8] = {
	GROUP_SM_ENA1,
	GROUP_SM_ENA2,
	GROUP_SM_ENA3,
	GROUP_SM_ENA4,
	GROUP_SM_ENA5,
	GROUP_SM_ENA6,
	GROUP_SM_ENA7,
	GROUP_SM_ENA8
};

unsigned short int  sm_dir_gpio_pin[8] = {
	IO_SM_DIR1,
	IO_SM_DIR2,
	IO_SM_DIR3,
	IO_SM_DIR4,
	IO_SM_DIR5,
	IO_SM_DIR6,
	IO_SM_DIR7,
	IO_SM_DIR8
};
unsigned short int  sm_pul_gpio_pin[8] = {
	IO_SM_PUL1,
	IO_SM_PUL2,
	IO_SM_PUL3,
	IO_SM_PUL4,
	IO_SM_PUL5,
	IO_SM_PUL6,
	IO_SM_PUL7,
	IO_SM_PUL8
};
unsigned short int  sm_ena_gpio_pin[8] = {
	IO_SM_ENA1,
	IO_SM_ENA2,
	IO_SM_ENA3,
	IO_SM_ENA4,
	IO_SM_ENA5,
	IO_SM_ENA6,
	IO_SM_ENA7,
	IO_SM_ENA8
};

//������ﶨʱ����ʼ�������ö�ʱ��4��Ϊ4����������������ƣ�
void init_step_motor()
{	
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	DBGMCU_Config(DBGMCU_CR_DBG_TIM2_STOP,ENABLE);	//��ʱ��2��debugʱ���ϵ㴦ֹͣ���� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	DBGMCU_Config(DBGMCU_CR_DBG_TIM3_STOP,ENABLE);	//��ʱ��3��debugʱ���ϵ㴦ֹͣ���� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);							   
	DBGMCU_Config(DBGMCU_CR_DBG_TIM4_STOP,ENABLE);	//��ʱ��4��debugʱ���ϵ㴦ֹͣ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	DBGMCU_Config(DBGMCU_CR_DBG_TIM5_STOP,ENABLE);	//��ʱ��5��debugʱ���ϵ㴦ֹͣ����
	
	
	TIM_TimeBaseStructure.TIM_Period = 65535;  		//�ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Prescaler = 35;		//ʱ��Ԥ��Ƶ��,ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ

 	TIM_DeInit(TIM2);	//2MHz��ʱ��Ƶ��
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	TIM_DeInit(TIM3);	//2MHz��ʱ��Ƶ��	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 	
	//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	TIM_DeInit(TIM4);	//2MHz��ʱ��Ƶ��	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 	
	//TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	//TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

	TIM_DeInit(TIM5);	//2MHz��ʱ��Ƶ��	
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 	
	//TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	//TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
		
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1100;	//CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1300;	//CCR2_Val;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1700;	//CCR3_Val;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Disable);

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1900;	//CCR4_Val;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Disable);

  	//TIM_ARRPreloadConfig(TIM2, ENABLE);		
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);

	//TIM_ARRPreloadConfig(TIM3, ENABLE);		
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
		
	//TIM_ARRPreloadConfig(TIM4, ENABLE);		
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
	/* TIM IT enable */
	TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC2, DISABLE);		//liu
	TIM_ITConfig(TIM4, TIM_IT_CC3, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, DISABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);

	//TIM_ARRPreloadConfig(TIM5, ENABLE);		
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
	/* TIM IT enable */
	TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_CC4, DISABLE);
	/* TIM5 enable counter */
	TIM_Cmd(TIM5, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
	//memset(StepMotorCtr, 0, sizeof(StepMotorCtr));
}

//�������ʹ�ܿ���
//enable_flag:	SM_ENABLE(1)	/	SM_DISABLE(0)
void sm_set_enable(unsigned char sm_id, bool status)
{
	unsigned char nID;
	//sm_set_pul(sm_id, 1);
    unsigned char bOutputStatus;
    bOutputStatus = status & 0x01;
    if(SERVO_MOTOR == StepMotorCtr[sm_id].bMotorType)
        bOutputStatus = !bOutputStatus;

    switch(sm_id)
    {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
//		case 9:
			nID = sm_id - 1; 
            if(bOutputStatus & 0x01) 
				sm_ena_gpio_base[nID]->BSRR = sm_ena_gpio_pin[nID];	//H
            else
                sm_ena_gpio_base[nID]->BRR = sm_ena_gpio_pin[nID];	//L
            break;
//		case 10:
//           if(status & 0x01) 
//                fpga_write(20, 0);
//            else
//                fpga_write(20, 1); 
//			break;
//		case 11:
//           if(status & 0x01) 
//                fpga_write(22, 0);
//            else
//                fpga_write(22, 1); 
//			break;
//		case 12:
//           if(status & 0x01) 
//                fpga_write(24, 0);
//            else
//                fpga_write(24, 1); 
//			break;
//		case 13:
//            if(status & 0x01) 
//                fpga_write(28, 0);
//            else
//                fpga_write(28, 1); 
//			break;
//		case 14:
//			break;
//		case 15:
//			break;
//		case 16:
//			break;
		default:
			break;
    }

    if(0 == status)
    {
        StepMotorCtr[sm_id].bHaveDisabledFlag = true;
        StepMotorCtr[sm_id].nWaitStableTime = 0;
    }
}

//������﷽�����
void sm_set_dir(unsigned char sm_id, unsigned char status)
{
	unsigned char nID;
	//sm_dir[sm_id-1] = status;
	StepMotorCtr[sm_id-1].bAbsDir = status;
    switch(sm_id)
    {
        case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		//case 9:
			nID = sm_id - 1; 
            if(status & 0x01) 
				sm_dir_gpio_base[nID]->BSRR = sm_dir_gpio_pin[nID];	 //H
            else
                sm_dir_gpio_base[nID]->BRR = sm_dir_gpio_pin[nID];	 //L
            break;
//		case 10:
//           if(status & 0x01) 
//                fpga_write(10, 0);
//            else
//                fpga_write(10, 1); 
//			break;
//		case 11:
//           if(status & 0x01) 
//                fpga_write(12, 0);
//            else
//                fpga_write(12, 1); 
//			break;
//		case 12:
//           if(status & 0x01) 
//                fpga_write(14, 0);
//            else
//                fpga_write(14, 1); 
//			break;
//		case 13:
//            if(status & 0x01) 
//                fpga_write(18, 0);
//            else
//                fpga_write(18, 1); 
//			break;
//		case 14:
//			break;
//		case 15:
//			break;
//		case 16:
//			break;
		default:
			break;
	}
}

//��������������
void sm_set_pul(unsigned char sm_id, unsigned char status)
{
	unsigned char nID;
    switch(sm_id)
    {
        case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
//		case 9:
//		case 10:
//		case 11:
//		case 12:
//		case 13:
			nID = sm_id - 1;
            if(status & 0x01) 
				sm_pul_gpio_base[nID]->BSRR = sm_pul_gpio_pin[nID];		//H
            else
                sm_pul_gpio_base[nID]->BRR = sm_pul_gpio_pin[nID];		//L
            break;
//		case 14:
//			break;
//		case 15:
//			break;
//		case 16:
//			break;
		default:
			break;
	}
}

void sm_brake(unsigned char sm_id)
{
	if(sm_id >0)
		StepMotorCtr[sm_id-1].bBrakeFlag = TRUE;
}

//1����ʱ��������4���������
//TIM1/8 4��ͨ��
//TIM2/3/4/5 ����4��ͨ��
//TIM6/7 ������ʱ��
void sm_run(unsigned char sm_id, unsigned char dir, unsigned int max_speed, unsigned long step) //����������г���
{
	//char cBuf[32];
	char nID;

	char sm_id_temp = 0;
	sm_id_temp = sm_id;
		
	if((sm_id > 0) && (sm_id <= 8))
		nID = sm_id - 1;
	else
		return;

	if(step == 0)
		return;

    switch (nID)
    {
    case 0:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB1_LEN;
        break;
    case 1:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB2_LEN;
        break;
    case 2:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB3_LEN;
        break;
    case 3:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB4_LEN;
        break;
    case 4:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB5_LEN;
        break;
    case 5:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB6_LEN;
        break;
    case 6:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB7_LEN;
        break;
    case 7:
        StepMotorCtr[nID].nSpeedTableMaxStep = TAB8_LEN;
        break;
    default:
        break;
    }

	if (StepMotorCtr[nID].bMotorRunStatus == RUN)
	{
		//memset(cBuf, 0, sizeof(cBuf));
		//sprintf(cBuf, "SM[%d] BUSY! Return!\n", nID);
		//debug_info(cBuf);
		return;
	}
 
//	memset(cBuf, 0, sizeof(cBuf));
//	sprintf(cBuf, "SM[%d] Start!\n", sm_id);
// 	debug_info(cBuf);
	sm_set_dir(sm_id_temp, dir);
	sm_set_enable(sm_id_temp, SM_ENABLE);
	sm_set_pul(sm_id_temp,1);
	
	StepMotorCtr[nID].bMotorRunStatus = RUN;
	StepMotorCtr[nID].bBrakeFlag = FALSE;
	//StepMotorCtr[nID].bPulseToggleFlag = false;

	//memset(cBuf, 0, sizeof(cBuf));
	//sprintf(cBuf, "SM[%d] BUSY! Return!\n", nID)
	//debug_info(cBuf);

    if (step == 1)
    {
        StepMotorCtr[nID].nAccSteps = 0;
        StepMotorCtr[nID].nConstantSteps = 0;
        StepMotorCtr[nID].nDecSteps = 1;
        StepMotorCtr[nID].nSpeedTableIndex = 1;
    } 
    else
    {
        if (max_speed > StepMotorCtr[nID].nSpeedTableMaxStep)
        {
            max_speed = StepMotorCtr[nID].nSpeedTableMaxStep;
        }
        StepMotorCtr[nID].nSpeedTableIndex = 0;
        if (max_speed <= (step>>1))
        {
            StepMotorCtr[nID].nAccSteps = max_speed;
            StepMotorCtr[nID].nConstantSteps = step - max_speed*2;
            StepMotorCtr[nID].nDecSteps = max_speed;
        } 
        else
        {
            StepMotorCtr[nID].nAccSteps = step>>1;
            StepMotorCtr[nID].nConstantSteps = 0;
            StepMotorCtr[nID].nDecSteps = step-StepMotorCtr[nID].nAccSteps;
        }
    }	
	StepMotorCtr[nID].nTargetSpeed = StepMotorCtr[nID].nAccSteps;
	StepMotorCtr[nID].bFlagChangeSpeed = 0;

//	TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);		//liu
//	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);	
	switch(nID)
	{
		case 0:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
			TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);	
			break;
		case 1:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
			TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);		
			break;
		case 2:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
			TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);	
			break;
		case 3:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
			TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);	
			break;
		case 4:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
			TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);	
			break;
		case 5:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
			TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);	
			break;
		case 6:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
			TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);	
			break;
		case 7:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
			TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);	
			break;
/*		case 8:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
			TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);	
			break;
		case 9:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
			TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);	
			break;
		case 10:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
			TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);	
			break;
		case 11:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
			TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);	
			break;
		case 12:
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
			TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);	
			break;
		case 13:
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
			TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);	
			break;
		case 14:
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
			TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);	
			break;*/
//		case 15:
//			TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
//			TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);	
//			break;
		default:
			break;
	}
}

void sm_change_speed(uchar sm_id, int new_speed) 	//����������г���
{
	char nID;
		
	if((sm_id > 0) && (sm_id <= 15))
		nID = sm_id - 1;
	else
		return;

	if (new_speed > StepMotorCtr[nID].nSpeedTableMaxStep)
		new_speed = StepMotorCtr[nID].nSpeedTableMaxStep;
	
	if (StepMotorCtr[nID].bMotorRunStatus == RUN)
	{
		StepMotorCtr[nID].bFlagChangeSpeed =  1;
		StepMotorCtr[nID].nNewChangeSpeed = new_speed;
	}
}

//ֹͣ���
void sm_stop(uchar sm_id)
{
	char nID;
	
	if((sm_id > 0) && (sm_id <= 15))
		nID = sm_id - 1;
	else
		return;
//	memset(cBuf, 0, sizeof(cBuf));
//	sprintf(cBuf, "SM[%d] Stop!\n", sm_id);
// 	debug_info(cBuf);

	switch(nID)
	{
		case 0:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
			TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
			break;
		case 1:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
			TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);		
			break;
		case 2:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
			TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);	
			break;
		case 3:
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
			TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);	
			break;
		case 4:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
			TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);	
			break;
		case 5:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
			TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);	
			break;
		case 6:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
			TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);	
			break;
		case 7:
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
			TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);	
			break;
/*		case 8:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
			TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
			break;
		case 9:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
			TIM_ITConfig(TIM4, TIM_IT_CC2, DISABLE);		
			break;
		case 10:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
			TIM_ITConfig(TIM4, TIM_IT_CC3, DISABLE);	
			break;
		case 11:
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
			TIM_ITConfig(TIM4, TIM_IT_CC4, DISABLE);	
			break;
		case 12:
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
			TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);	
			break;
		case 13:
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
			TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);	
			break;
		case 14:
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
			TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE);	
			break;*/
//		case 15:
//			TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
//			TIM_ITConfig(TIM5, TIM_IT_CC4, DISABLE);	
//			break;			
		default:
			break;
	}
	//StepMotorCtr[nID].bPulseToggleFlag = false;
	StepMotorCtr[nID].bMotorRunStatus = STOP;
	StepMotorCtr[nID].bRunNoStop = FALSE;
	sm_set_pul(sm_id, 1);
}

//����ֹͣ���
void sm_SlowdownStop(uchar sm_id,uint step)
{
	char nID;
	
	if((sm_id > 0) && (sm_id <= 8))
		nID = sm_id - 1;
	else
		return; 
	
	if (StepMotorCtr[nID].nAccSteps)
	{
		if (step>StepMotorCtr[nID].nAccSteps)
		{
			StepMotorCtr[nID].nAccSteps = 0;
			StepMotorCtr[nID].nConstantSteps = step-StepMotorCtr[nID].nAccSteps;
			StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nAccSteps;
		}
		else
		{
			StepMotorCtr[nID].nAccSteps = 0;
			StepMotorCtr[nID].nConstantSteps = 0;
			StepMotorCtr[nID].nDecSteps = step;
		}
	}
	else if (StepMotorCtr[nID].nDecSteps)
	{
		if (step>StepMotorCtr[nID].nDecSteps)
		{
			StepMotorCtr[nID].nAccSteps = 0;
			StepMotorCtr[nID].nConstantSteps = step-StepMotorCtr[nID].nDecSteps;			
		}
		else
		{
			StepMotorCtr[nID].nAccSteps = 0;
			StepMotorCtr[nID].nConstantSteps = 0;
			StepMotorCtr[nID].nDecSteps = step;
		}
	}
	else
	{
		sm_stop(sm_id);
	}	
}

bool SM_DIR_ABS_P[16] = {
	SM1_DIR_ABS_P,
	SM2_DIR_ABS_P,
	SM3_DIR_ABS_P,
	SM4_DIR_ABS_P,
	SM5_DIR_ABS_P,
	SM6_DIR_ABS_P,
	SM7_DIR_ABS_P,
	SM8_DIR_ABS_P,
	SM8_DIR_ABS_P,
	SM8_DIR_ABS_P,
	SM8_DIR_ABS_P,
	SM8_DIR_ABS_P,
	SM8_DIR_ABS_P,
	SM8_DIR_ABS_P,
	SM8_DIR_ABS_P,
};

void sm_set_abs_zero(unsigned char sm_id)
{
	//sm_count_abs[sm_id-1] = 0;
	StepMotorCtr[sm_id-1].nAbsStepCount	= 0;
}

void sm_run_abs(unsigned char sm_id, unsigned int max_speed, signed long target_pos)   //����������г���
{
	//long step_num = (long)target_pos - (long)sm_count_abs[sm_id-1];
	long step_num = (long)target_pos - (long)(StepMotorCtr[sm_id-1].nAbsStepCount);
	unsigned char dir; 
	unsigned char nID;

	nID = sm_id - 1;

	dir = SM_DIR_ABS_P[nID];

	if(step_num>=0)
	{
		sm_run(sm_id,dir,max_speed,(unsigned int)step_num);
	}
	else
	{
		dir = (dir==SM_CW) ? SM_CCW:SM_CW;			//ȡ������
		sm_run(sm_id,dir,max_speed,(unsigned int)(-step_num));				
	}
}

signed long sm_get_abs(unsigned char sm_id)
{
	int step1,step2;
	do
	{
		//step1 = sm_count_abs[sm_id-1];
		//step2 = sm_count_abs[sm_id-1];
		step1 = StepMotorCtr[sm_id-1].nAbsStepCount;
		step2 = StepMotorCtr[sm_id-1].nAbsStepCount; 
	}while(step1!=step2);

	return step2;
}

//��ȡ�������������
signed long sm_get_abs_step(unsigned char sm_id)
{
    return sm_get_abs(sm_id);
}


bool IsSmRunFinish(unsigned char sm_id)
{
    if(0 == StepMotorCtr[sm_id-1].bMotorRunStatus)    //Ϊ0����ʾ��ǰ��������Ѿ����
      return TRUE;
    else
      return FALSE;
}

//��������жϿڴ���
void TIM2_IRQHandler(void)
{
	uint16_t capture = 0;
	uint16_t curve_gap_time = 0;
	unsigned char sm_id;
	unsigned char nID; 
	int surplus_step = 0;
	int difference = 0;
	const unsigned short* pCurve_timer;

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		pCurve_timer = SpeedTable1;
		sm_id = 1;
		nID = 0;
	}
	else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		pCurve_timer = SpeedTable2;
		sm_id = 2;
		nID = 1;
	}
	else if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{
		pCurve_timer = SpeedTable3;
		sm_id = 3;
		nID = 2;
	}
	else if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
	{
		pCurve_timer = SpeedTable4;
		sm_id = 4;
		nID = 3;
	}
	else
	{
		return;
	}

    if (StepMotorCtr[nID].bHaveDisabledFlag)
    {
        curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
        StepMotorCtr[nID].nWaitStableTime++;
        if (StepMotorCtr[nID].nWaitStableTime > MAX_CNT)
        {
            StepMotorCtr[nID].bHaveDisabledFlag = FALSE;
            StepMotorCtr[nID].nWaitStableTime = 0;
        }
    } 
    else
    {
        if (!StepMotorCtr[nID].bPulseToggleFlag)
        {
            StepMotorCtr[nID].bPulseToggleFlag = true;
            sm_set_pul(sm_id,0);	  			//��������͵�ƽ
            curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
        }
        else
        {
            StepMotorCtr[nID].bPulseToggleFlag = false;
            StepMotorCtr[nID].bMotorRunStatus = RUN;
            sm_set_pul(sm_id,1);   				//��������ߵ�ƽ
            if (StepMotorCtr[nID].bRunNoStop==FALSE)
            {
                StepMotorCtr[nID].nStepsHaveRun++;
            }
            if(StepMotorCtr[nID].bAbsDir == SM_DIR_ABS_P[nID])
            {
                StepMotorCtr[nID].nAbsStepCount++;
            }
            else
            {
                StepMotorCtr[nID].nAbsStepCount--;
            }

            if(StepMotorCtr[nID].bBrakeFlag==TRUE)
            {
                StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nAccSteps;
                StepMotorCtr[nID].nAccSteps=0;
                StepMotorCtr[nID].nConstantSteps=0;
            }

            if (StepMotorCtr[nID].nAccSteps != 0)  			//���ٶ�
            {
                curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
                StepMotorCtr[nID].nAccSteps--;

                if(StepMotorCtr[nID].nAccSteps != 0)
                {
                    StepMotorCtr[nID].nSpeedTableIndex++;
                }
            }
            else if (StepMotorCtr[nID].nConstantSteps != 0)  	//���ٶ�
            {
                curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
                if(StepMotorCtr[nID].bRunNoStop == FALSE)
                {
                    StepMotorCtr[nID].nConstantSteps--;
                }
            }
            else if (StepMotorCtr[nID].nDecSteps != 0)  	//ɲ����
            {
                curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
                StepMotorCtr[nID].nDecSteps--;
                if (StepMotorCtr[nID].nSpeedTableIndex != 0)
                {
                    StepMotorCtr[nID].nSpeedTableIndex--;
                }
            }

            if (StepMotorCtr[nID].nDecSteps == 0) 	   		//���ٶ�������
            {
                if(nID == 0)
                    TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
                else if(nID == 1)
                    TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
                else if(nID == 2)
                    TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);
                else if(nID == 3)
                    TIM_ITConfig(TIM2, TIM_IT_CC4, DISABLE);
                StepMotorCtr[nID].bMotorRunStatus = STOP;		
            }
            else if(StepMotorCtr[nID].bFlagChangeSpeed)	//���ٱ�־�����·���Ӽ��ٲ���		
            {
                StepMotorCtr[nID].bFlagChangeSpeed = 0;
                surplus_step = StepMotorCtr[nID].nAccSteps + StepMotorCtr[nID].nConstantSteps + StepMotorCtr[nID].nDecSteps;  	
                if(StepMotorCtr[nID].nNewChangeSpeed > StepMotorCtr[nID].nSpeedTableIndex) difference = StepMotorCtr[nID].nNewChangeSpeed - StepMotorCtr[nID].nSpeedTableIndex;
                else difference = StepMotorCtr[nID].nSpeedTableIndex - StepMotorCtr[nID].nNewChangeSpeed;
                if(surplus_step > (difference + StepMotorCtr[nID].nNewChangeSpeed))		//ʣ�ಽ�����㹻�����ٶ�
                {
                    StepMotorCtr[nID].nAccSteps = difference;
                    StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nNewChangeSpeed;
                    StepMotorCtr[nID].nConstantSteps = surplus_step - StepMotorCtr[nID].nAccSteps - StepMotorCtr[nID].nDecSteps;
                    StepMotorCtr[nID].nTargetSpeed = StepMotorCtr[nID].nNewChangeSpeed;
                }			
            }
        }
    }
	
	//���¸���ʱ���Ƚ�ֵ---------------------------
	if(nID == 0)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM2);
		TIM_SetCompare1(TIM2, capture + curve_gap_time);
	}
	else if(nID == 1)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM2);
		TIM_SetCompare2(TIM2, capture + curve_gap_time);
	}
	else if(nID == 2)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		capture = TIM_GetCapture3(TIM2);
		TIM_SetCompare3(TIM2, capture + curve_gap_time);
	}
	else if(nID == 3)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		capture = TIM_GetCapture4(TIM2);
		TIM_SetCompare4(TIM2, capture + curve_gap_time);
	}
}

//��������жϿڴ���
void TIM3_IRQHandler(void)
{
    uint16_t capture = 0;
    uint16_t curve_gap_time = 0;
    unsigned char sm_id;
    unsigned char nID; 
    int surplus_step = 0;
    int difference = 0;
    const unsigned short* pCurve_timer;

    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        pCurve_timer = SpeedTable5;
        sm_id = 5;
        nID = 4;
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {
        pCurve_timer = SpeedTable6;
        sm_id = 6;
        nID = 5;
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
    {
        pCurve_timer = SpeedTable7;
        sm_id = 7;
        nID = 6;
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {
        pCurve_timer = SpeedTable8;
        sm_id = 8;
        nID = 7;
    }
    else
    {
        return;
    }

    if (StepMotorCtr[nID].bHaveDisabledFlag)
    {
        curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
        StepMotorCtr[nID].nWaitStableTime++;
        if (StepMotorCtr[nID].nWaitStableTime > MAX_CNT)
        {
            StepMotorCtr[nID].bHaveDisabledFlag = FALSE;
            StepMotorCtr[nID].nWaitStableTime = 0;
        }
    } 
    else
    {
        if (!StepMotorCtr[nID].bPulseToggleFlag)
        {
            StepMotorCtr[nID].bPulseToggleFlag = true;
            sm_set_pul(sm_id,0);	  			//��������͵�ƽ
            curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
        }
        else
        {
            StepMotorCtr[nID].bPulseToggleFlag = false;
            StepMotorCtr[nID].bMotorRunStatus = RUN;
            sm_set_pul(sm_id,1);   				//��������ߵ�ƽ
            if (StepMotorCtr[nID].bRunNoStop==FALSE)
            {
                StepMotorCtr[nID].nStepsHaveRun++;
            }
            if(StepMotorCtr[nID].bAbsDir == SM_DIR_ABS_P[nID])
            {
                StepMotorCtr[nID].nAbsStepCount++;
            }
            else
            {
                StepMotorCtr[nID].nAbsStepCount--;
            }

            if(StepMotorCtr[nID].bBrakeFlag==TRUE)
            {
                StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nAccSteps;
                StepMotorCtr[nID].nAccSteps=0;
                StepMotorCtr[nID].nConstantSteps=0;
            }

            if (StepMotorCtr[nID].nAccSteps != 0)  			//���ٶ�
            {
                curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
                StepMotorCtr[nID].nAccSteps--;

                if(StepMotorCtr[nID].nAccSteps != 0)
                {
                    StepMotorCtr[nID].nSpeedTableIndex++;
                }
            }
            else if (StepMotorCtr[nID].nConstantSteps != 0)  	//���ٶ�
            {
                curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
                if(StepMotorCtr[nID].bRunNoStop == FALSE)
                {
                    StepMotorCtr[nID].nConstantSteps--;
                }
            }
            else if (StepMotorCtr[nID].nDecSteps != 0)  	//ɲ����
            {
                curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
                StepMotorCtr[nID].nDecSteps--;
                if (StepMotorCtr[nID].nSpeedTableIndex != 0)
                {
                    StepMotorCtr[nID].nSpeedTableIndex--;
                }
            }

            if (StepMotorCtr[nID].nDecSteps == 0) 	   		//���ٶ�������
            {
                if(nID == 4)
                    TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
                else if(nID == 5)
                    TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
                else if(nID == 6)
                    TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
                else if(nID == 7)
                    TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);
                StepMotorCtr[nID].bMotorRunStatus = STOP;		
            }
            else if(StepMotorCtr[nID].bFlagChangeSpeed)	//���ٱ�־�����·���Ӽ��ٲ���		
            {
                StepMotorCtr[nID].bFlagChangeSpeed = 0;
                surplus_step = StepMotorCtr[nID].nAccSteps + StepMotorCtr[nID].nConstantSteps + StepMotorCtr[nID].nDecSteps;  	
                if(StepMotorCtr[nID].nNewChangeSpeed > StepMotorCtr[nID].nSpeedTableIndex) difference = StepMotorCtr[nID].nNewChangeSpeed - StepMotorCtr[nID].nSpeedTableIndex;
                else difference = StepMotorCtr[nID].nSpeedTableIndex - StepMotorCtr[nID].nNewChangeSpeed;
                if(surplus_step > (difference + StepMotorCtr[nID].nNewChangeSpeed))		//ʣ�ಽ�����㹻�����ٶ�
                {
                    StepMotorCtr[nID].nAccSteps = difference;
                    StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nNewChangeSpeed;
                    StepMotorCtr[nID].nConstantSteps = surplus_step - StepMotorCtr[nID].nAccSteps - StepMotorCtr[nID].nDecSteps;
                    StepMotorCtr[nID].nTargetSpeed = StepMotorCtr[nID].nNewChangeSpeed;
                }			
            }
        }
    }

    //���¸���ʱ���Ƚ�ֵ---------------------------
    if(nID == 4)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
        capture = TIM_GetCapture1(TIM3);
        TIM_SetCompare1(TIM3, capture + curve_gap_time);
    }
    else if(nID == 5)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
        capture = TIM_GetCapture2(TIM3);
        TIM_SetCompare2(TIM3, capture + curve_gap_time);
    }
    else if(nID == 6)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
        capture = TIM_GetCapture3(TIM3);
        TIM_SetCompare3(TIM3, capture + curve_gap_time);
    }
    else if(nID == 7)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
        capture = TIM_GetCapture4(TIM3);
        TIM_SetCompare4(TIM3, capture + curve_gap_time);
    }
}

////��������жϿڴ���
//void TIM4_IRQHandler(void)
//{
//	uint16_t capture = 0;
//	uint16_t curve_gap_time = 0;
//	unsigned char sm_id;
//	unsigned char nID; 
//	int surplus_step = 0;
//	int difference = 0;
//	const unsigned short* pCurve_timer;
//
////	bool Number_Clear = 0;		//liu
//
//	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
//	{
//		pCurve_timer = curve_timer9;
//		sm_id = 9;
//		nID = 8;
//	}
//	//else if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
//	//{
//	//	pCurve_timer = curve_timer10;
//	//	sm_id = 10;
//	//	nID = 9;
//	//}
///*	else if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)		//liu
//	{
//		if(!input_get_one(SN_COUNTER2_CLEAR))
//		{
//			fpga_write(SN_COUNTER1_IN1,0);		//������1����
//			fpga_write(SN_COUNTER1_IN1,1);		//������1��ʼ����
//		}
//	}*/
//	else if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
//	{
//		pCurve_timer = curve_timer11;
//		sm_id = 11;
//		nID = 10;
//	}
//	else if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
//	{
//		pCurve_timer = curve_timer12;
//		sm_id = 12;
//		nID = 11;
//	}
//	else
//	{
//		return;
//	}
//
//	if (!StepMotorCtr[nID].bPulseToggleFlag)
//	{
//		StepMotorCtr[nID].bPulseToggleFlag = true;
//		sm_set_pul(sm_id,0);
//		curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//	}
//	else
//	{
//		StepMotorCtr[nID].bPulseToggleFlag = false;
//		StepMotorCtr[nID].bMotorRunStatus = RUN;
//		sm_set_pul(sm_id,1);
//		
//		if(StepMotorCtr[nID].bAbsDir == SM_DIR_ABS_P[nID])
//			StepMotorCtr[nID].nAbsStepCount++;
//		else
//			StepMotorCtr[nID].nAbsStepCount--;
//
//		if(StepMotorCtr[nID].bBrakeFlag)
//		{
//			StepMotorCtr[nID].bBrakeFlag = 0;
//			StepMotorCtr[nID].nAccSteps=0;
//			StepMotorCtr[nID].nConstantSteps=0;
//			StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nAccSteps;
//		}
//			
//		if (StepMotorCtr[nID].nAccSteps != 0)  			//���ٶ�
//		{
//			curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//			StepMotorCtr[nID].nAccSteps--;
//
//			if(StepMotorCtr[nID].nSpeedTableIndex < StepMotorCtr[nID].nTargetSpeed)
//				StepMotorCtr[nID].nSpeedTableIndex++;			 	//����
//			else 
//				StepMotorCtr[nID].nSpeedTableIndex--;				//����
//		}
//		else if (StepMotorCtr[nID].nConstantSteps != 0)  	//���ٶ�
//		{
//			curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//			if(StepMotorCtr[nID].bRunNoStop == FALSE)
//				StepMotorCtr[nID].nConstantSteps--;
//		}
//		else if (StepMotorCtr[nID].nDecSteps != 0)  	//���ٶ�
//		{
//			curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//			StepMotorCtr[nID].nDecSteps--;
//			StepMotorCtr[nID].nSpeedTableIndex--;
//
//			if(0 == StepMotorCtr[nID].nSpeedTableIndex)
//				StepMotorCtr[nID].nDecSteps = 0;
//		}
//
//		if (StepMotorCtr[nID].nDecSteps == 0) 
//		{
//			if(nID == 8)
//				TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
//			else if(nID == 9)
//				TIM_ITConfig(TIM4, TIM_IT_CC2, DISABLE);
//			else if(nID == 10)
//				TIM_ITConfig(TIM4, TIM_IT_CC3, DISABLE);
//			else if(nID == 11)
//				TIM_ITConfig(TIM4, TIM_IT_CC4, DISABLE);
//			StepMotorCtr[nID].bMotorRunStatus = STOP;
//		}
//		else if(StepMotorCtr[nID].bFlagChangeSpeed)	//���ٱ�־�����·���Ӽ��ٲ���		
//		{
//		  	StepMotorCtr[nID].bFlagChangeSpeed = 0;
//			surplus_step = StepMotorCtr[nID].nAccSteps + StepMotorCtr[nID].nConstantSteps + StepMotorCtr[nID].nDecSteps;  	
//			if(StepMotorCtr[nID].nNewChangeSpeed > StepMotorCtr[nID].nSpeedTableIndex) difference = StepMotorCtr[nID].nNewChangeSpeed - StepMotorCtr[nID].nSpeedTableIndex;
//			else difference = StepMotorCtr[nID].nSpeedTableIndex - StepMotorCtr[nID].nNewChangeSpeed;
//			if(surplus_step > (difference + StepMotorCtr[nID].nNewChangeSpeed))		//ʣ�ಽ�����㹻�����ٶ�
//			{
//				StepMotorCtr[nID].nAccSteps = difference;
//				StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nNewChangeSpeed;
//				StepMotorCtr[nID].nConstantSteps = surplus_step - StepMotorCtr[nID].nAccSteps - StepMotorCtr[nID].nDecSteps;
//				StepMotorCtr[nID].nTargetSpeed = StepMotorCtr[nID].nNewChangeSpeed;
//			}			
//		}
//	}
//
//	if(nID == 8)
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
//		capture = TIM_GetCapture1(TIM4);
//		TIM_SetCompare1(TIM4, capture + curve_gap_time);
//	}
//	else if(nID == 9)
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
//		capture = TIM_GetCapture2(TIM4);
//		TIM_SetCompare2(TIM4, capture + curve_gap_time);
//	}
//	else if(nID == 10)
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
//		capture = TIM_GetCapture3(TIM4);
//		TIM_SetCompare3(TIM4, capture + curve_gap_time);
//	}
//	else if(nID == 11)
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
//		capture = TIM_GetCapture4(TIM4);
//		TIM_SetCompare4(TIM4, capture + curve_gap_time);
//	}
//
//}
//
////��������жϿڴ���
//void TIM5_IRQHandler(void)
//{
//	uint16_t capture = 0;
//	uint16_t curve_gap_time = 0;
//	unsigned char sm_id;
//	unsigned char nID; 
//	int surplus_step = 0;
//	int difference = 0;
//	const unsigned short* pCurve_timer;
//
//	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
//	{
//		pCurve_timer = curve_timer13;
//		sm_id = 13;
//		nID = 12;
//	}
////	else if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)
////	{
////		pCurve_timer = curve_timer14;
////		sm_id = 14;
////		nID = 13;
////	}
////	else if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)
////	{
////		pCurve_timer = curve_timer15;
////		sm_id = 15;
////		nID = 14;
////	}
////	else if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)
////	{
////		pCurve_timer = curve_timer16;
////		sm_id = 16;
////		nID = 15;
////	}
//	else
//	{
//		return;
//	}
//
//	if (!StepMotorCtr[nID].bPulseToggleFlag)
//	{
//		StepMotorCtr[nID].bPulseToggleFlag = true;
//		sm_set_pul(sm_id,0);
//		curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//	}
//	else
//	{
//		StepMotorCtr[nID].bPulseToggleFlag = false;
//		StepMotorCtr[nID].bMotorRunStatus = RUN;
//		sm_set_pul(sm_id,1);
//		
//		if(StepMotorCtr[nID].bAbsDir == SM_DIR_ABS_P[nID])
//			StepMotorCtr[nID].nAbsStepCount++;
//		else
//			StepMotorCtr[nID].nAbsStepCount--;
//
//		if(StepMotorCtr[nID].bBrakeFlag)
//		{
//			StepMotorCtr[nID].bBrakeFlag = 0;
//			StepMotorCtr[nID].nAccSteps=0;
//			StepMotorCtr[nID].nConstantSteps=0;
//			StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nAccSteps;
//		}
//			
//		if (StepMotorCtr[nID].nAccSteps != 0)  			//���ٶ�
//		{
//			curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//			StepMotorCtr[nID].nAccSteps--;
//
//			if(StepMotorCtr[nID].nSpeedTableIndex < StepMotorCtr[nID].nTargetSpeed)
//				StepMotorCtr[nID].nSpeedTableIndex++;			 	//����
//			else 
//				StepMotorCtr[nID].nSpeedTableIndex--;				//����
//		}
//		else if (StepMotorCtr[nID].nConstantSteps != 0)  	//���ٶ�
//		{
//			curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//			if(StepMotorCtr[nID].bRunNoStop == FALSE)
//				StepMotorCtr[nID].nConstantSteps--;
//		}
//		else if (StepMotorCtr[nID].nDecSteps != 0)  	//���ٶ�
//		{
//			curve_gap_time = *(pCurve_timer + StepMotorCtr[nID].nSpeedTableIndex);
//			StepMotorCtr[nID].nDecSteps--;
//			StepMotorCtr[nID].nSpeedTableIndex--;
//
//			if(0 == StepMotorCtr[nID].nSpeedTableIndex)
//				StepMotorCtr[nID].nDecSteps = 0;
//		}
//
//		if (StepMotorCtr[nID].nDecSteps == 0) 
//		{
//			if(nID == 12)
//				TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
//			else if(nID == 13)
//				TIM_ITConfig(TIM5, TIM_IT_CC2, DISABLE);
//			else if(nID == 14)
//				TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE);
////			else if(nID == 15)
////				TIM_ITConfig(TIM5, TIM_IT_CC4, DISABLE);
//			StepMotorCtr[nID].bMotorRunStatus = STOP;
//		}
//		else if(StepMotorCtr[nID].bFlagChangeSpeed)	//���ٱ�־�����·���Ӽ��ٲ���		
//		{
//		  	StepMotorCtr[nID].bFlagChangeSpeed = 0;
//			surplus_step = StepMotorCtr[nID].nAccSteps + StepMotorCtr[nID].nConstantSteps + StepMotorCtr[nID].nDecSteps;  	
//			if(StepMotorCtr[nID].nNewChangeSpeed > StepMotorCtr[nID].nSpeedTableIndex) difference = StepMotorCtr[nID].nNewChangeSpeed - StepMotorCtr[nID].nSpeedTableIndex;
//			else difference = StepMotorCtr[nID].nSpeedTableIndex - StepMotorCtr[nID].nNewChangeSpeed;
//			if(surplus_step > (difference + StepMotorCtr[nID].nNewChangeSpeed))		//ʣ�ಽ�����㹻�����ٶ�
//			{
//				StepMotorCtr[nID].nAccSteps = difference;
//				StepMotorCtr[nID].nDecSteps = StepMotorCtr[nID].nNewChangeSpeed;
//				StepMotorCtr[nID].nConstantSteps = surplus_step - StepMotorCtr[nID].nAccSteps - StepMotorCtr[nID].nDecSteps;
//				StepMotorCtr[nID].nTargetSpeed = StepMotorCtr[nID].nNewChangeSpeed;
//			}			
//		}
//	}
//
//	if(nID == 12)
//	{
//		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
//		capture = TIM_GetCapture1(TIM5);
//		TIM_SetCompare1(TIM5, capture + curve_gap_time);
//	}
////	else if(nID == 13)
////	{
////		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
////		capture = TIM_GetCapture2(TIM5);
////		TIM_SetCompare2(TIM5, capture + curve_gap_time);
////	}
////	else if(nID == 14)
////	{
////		TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
////		capture = TIM_GetCapture3(TIM5);
////		TIM_SetCompare3(TIM5, capture + curve_gap_time);
////	}
////	else if(nID == 15)
////	{
////		TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
////		capture = TIM_GetCapture4(TIM5);
////		TIM_SetCompare4(TIM5, capture + curve_gap_time);
////	}
//}

