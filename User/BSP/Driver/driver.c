#include <string.h>
#include "stm32f10x.h"
#include "bsp.h"
#include "stm32f10x_iwdg.h"

#define  MAX_CNT        200

char * BSP_version = "BSP:393R2+227R4 V2.17.1027.1";
unsigned long m_Input_All_now = 0;			//����1~32
unsigned long m_Input_All_now2 = 0;			//����33~40
//unsigned long m_Input_All_now_last;	
unsigned long m_Output_All = 0;				//���
unsigned long m_DM_Output_All = 0;			//ֱ�����
unsigned char m_DM_Status[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};		//ֱ�����״̬
unsigned char dm_run_ctl[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile unsigned long systemTime = 0;		//ϵͳʱ��



/*****************************************************************************
  * �������ƣ� check_time_delay
  * ���������� ��ʱ��ʱ��麯��
  * ����˵���� ��1��delayTimeΪ��ǰҪ��ʱ��ʱ�䣬��λΪ����
*****************************************************************************/
bool check_time_delay(unsigned long base_time, unsigned long wait_time)
{
	unsigned long long tmp_time;
	//��ֹ�ж�
	tmp_time = systemTime;
	//ʱ�����
	if(base_time > tmp_time)
	{
		tmp_time |= 0x100000000;
	}
	tmp_time = tmp_time - base_time;
	//�ָ��ж�
	//if((base_time + wait_time) >= tmp_time)
	if((unsigned long)tmp_time < wait_time)
	{
		return 0;	
	}
	else
	{
		return 1;
	}
}


/***************************************************************************************************************
  * �������ƣ� PreCheckSensor
  * ���������� Ԥ�촫����
  * ����˵������1������������ֻ����һ��Ŀ��λ�ô���������ⲻ����һ��λ�ô����������ν��٣���ͨ����
									�ֳɶ��STEPʵ��
****************************************************************************************************************/
void PreCheckSensor(stOpCtrl *Operation,unsigned char uTargetSensor,bool bRightResult,unsigned char uNextStep, unsigned int nErrorCode)
{
	if (input_get_one(uTargetSensor) == bRightResult)
	{
		// ��������������ת��ָ��STEP
		Operation->nStep = uNextStep;			
	}
	else
	{
		// λ�ü�⴫��������쳣�����Դ���
		SetOpErrorInfo(Operation,nErrorCode,ERROR_RETRY);			
	}
}

void init_output_ctl(void)
{
	//m_Output_All = 0;
	//output_ctl_all();
}

void init_dm_ctl(void)
{
	m_DM_Output_All = 0;
	dm_ctl_all();
	memset(m_DM_Status,DM_FREE,16);
}

//��ȡ���е�����״̬
void input_get_all(void)
{
	//���λλIN32,���λΪIN1
	__disable_irq();
	m_Input_All_now2 = 0xFFFF;
	m_Input_All_now2 = fpga_read(0x18); 	//�����Ƿ����0x5AA5
	m_Input_All_now2 = (fpga_read(0x14)) & 0xFF;

	m_Input_All_now = fpga_read(0x12);
	m_Input_All_now = m_Input_All_now << 16;
	m_Input_All_now = m_Input_All_now + fpga_read(0x10);
	__enable_irq();
}

/***************************************************************************************************************
  * �������ƣ� input_get_one
  * ���������� ��ȡָ��������״̬
  * ����˵����
****************************************************************************************************************/
bool input_get_one(unsigned char port_num)
{
	input_get_all();
	if(port_num < 1)
		port_num = 1;
	if(port_num > 32)
		port_num = 32;
	if(m_Input_All_now & (0x01L << (port_num-1)))
		return 1;
	else
		return 0;
}

GPIO_TypeDef * output_gpio_base[24] = {
	GROUP_OUT1,
	GROUP_OUT2,
	GROUP_OUT3,
	GROUP_OUT4,
	GROUP_OUT5,
	GROUP_OUT6,
	GROUP_OUT7,
	GROUP_OUT8,
	GROUP_OUT9,
	GROUP_OUT10,
	GROUP_OUT11,
	GROUP_OUT12,
	GROUP_OUT13,
	GROUP_OUT14,
	GROUP_OUT15,
	GROUP_OUT16,
	GROUP_OUT17,
	GROUP_OUT18,
	GROUP_OUT19,
	GROUP_OUT20,
	GROUP_OUT21,
	GROUP_OUT22,
	GROUP_OUT23,
	GROUP_OUT24
};

unsigned short int  output_gpio_pin[24] = {
	IO_OUT1,
	IO_OUT2,
	IO_OUT3,
	IO_OUT4,
	IO_OUT5,
	IO_OUT6,
	IO_OUT7,
	IO_OUT8,
	IO_OUT9,
	IO_OUT10,
	IO_OUT11,
	IO_OUT12,
	IO_OUT13,
	IO_OUT14,
	IO_OUT15,
	IO_OUT16,
	IO_OUT17,
	IO_OUT18,
	IO_OUT19,
	IO_OUT20,
	IO_OUT21,
	IO_OUT22,
	IO_OUT23,
	IO_OUT24
};

void dm_ctl_all(void)
{
	unsigned char i;
	for(i = 0; i < 24; i++)
	{
		if(m_DM_Output_All & (0x01L << i))
			output_gpio_base[i]->BSRR = output_gpio_pin[i];
		else
			output_gpio_base[i]->BRR = output_gpio_pin[i];
	}
}

//����һ��ֱ�����
void dm_ctrl_one(unsigned char dm_num, unsigned char dm_status)
{
//	if(1 == dm_status)
//		output_gpio_base[dm_num]->BSRR = output_gpio_pin[dm_num];
//	else
//		output_gpio_base[dm_num]->BRR = output_gpio_pin[dm_num];

	unsigned long tmp_dm_status;
	unsigned char i = 0;
	if(dm_num > 12)
		return;
	if(dm_num == 0)
		return;
  
	tmp_dm_status = dm_status;
	tmp_dm_status = tmp_dm_status << ((dm_num - 1)<<1);
  
	m_DM_Status[dm_num - 1] = dm_status;
	m_DM_Output_All &= ~(0x03UL << ((dm_num - 1)<<1));
	m_DM_Output_All |= tmp_dm_status;
  	//dm_ctl_all();

	i = (dm_num - 1) << 1;
	switch(dm_status)
	{
	case DM_BRAKE:
		output_gpio_base[i + 1]->BRR = output_gpio_pin[i + 1];
		output_gpio_base[i]->BRR = output_gpio_pin[i];
		break;
	case DM_CW:
		output_gpio_base[i + 1]->BSRR = output_gpio_pin[i + 1];
		output_gpio_base[i]->BRR = output_gpio_pin[i];
		break;
	case DM_CCW:
		output_gpio_base[i + 1]->BRR = output_gpio_pin[i + 1];
		output_gpio_base[i]->BSRR = output_gpio_pin[i];
		break;
	case DM_FREE:
		output_gpio_base[i + 1]->BSRR = output_gpio_pin[i + 1];
		output_gpio_base[i]->BSRR = output_gpio_pin[i];
		break;
	default:
		output_gpio_base[i + 1]->BRR = output_gpio_pin[i + 1];
		output_gpio_base[i]->BRR = output_gpio_pin[i];
		break;
	}
}


//ͨ�����巽ʽ������ֱ��������Դﵽ���Ƶ���ٶȵ�Ŀ��
void dm_ctl_one_slow(unsigned char dm_num, unsigned char dm_status)
{
	if(dm_num > 7)
		return;
	if(dm_num == 0)
		return;

	dm_run_ctl[dm_num - 1] = dm_status;
	
	switch(dm_status)
	{
	case DM_CW:		 						//�򿪶�ʱ�����ر����ڶ�ʱ���ж����Լ��رյ�
	case DM_CCW:
 		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		TIM_Cmd(TIM6, ENABLE);		
		break;

	default:								//������ͣס���������ڶ�ʱ�����棬����ͣ�ø��Ӽ�ʱ
		dm_ctrl_one(dm_num, dm_status);
		break;
	}
}

unsigned char get_button_status(unsigned char button_num)
{
    unsigned char value;
    switch(button_num)
    {
        case 1:
            //value = GPIO_ReadInputDataBit(GROUP_BUTTON1 , IO_BUTTON1)?1:0;
			value = 1;
            break;
        case 2:
            //��FPGA����ȡBUTTON2����Ϣ
            break;
        default:
            value = 0;
            break;
    }
    return value;
}

//��ɫLED��
__inline void led_red_on(void)
{
    GROUP_LED_RED->BRR = IO_LED_RED;    
}

//��ɫLED��
__inline void led_red_off(void)
{
    GROUP_LED_RED->BSRR = IO_LED_RED;       
}

//��ɫLED��
void led_blue_on(void)
{
    GROUP_LED_BLUE->BRR = IO_LED_BLUE;       
}

//��ɫLED��
void led_blue_off(void)
{
    GROUP_LED_BLUE->BSRR = IO_LED_BLUE;       
}

/*
void init_dc_motor(void)
{
	TIM_TimeBaseInitTypeDef	TIM6_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;							

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	DBGMCU_Config(DBGMCU_CR_DBG_TIM6_STOP,ENABLE);
	TIM_DeInit(TIM6);

	TIM6_TimeBaseStructure.TIM_Period = 49*5;						//49*1--10kHz,49*5--2kHz;
	TIM6_TimeBaseStructure.TIM_Prescaler = 72;						//ʱ��Ԥ��Ƶ����ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1)
	TIM6_TimeBaseStructure.TIM_ClockDivision = 0;					//������Ƶ
	TIM6_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	
	TIM_TimeBaseInit(TIM6, &TIM6_TimeBaseStructure); 	
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
		
	TIM_ARRPreloadConfig(TIM6, ENABLE);		

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM6_IRQHandler(void)
{
	static unsigned char flag = 0;
	unsigned char f_disable = 1 , ii = 0;
	TIM_TimeBaseInitTypeDef	TIM6_TimeBaseStructure;

	flag = ~flag;
	if (TIM_GetITStatus(TIM6 , TIM_IT_Update) == SET)
	{
		TIM6_TimeBaseStructure.TIM_Prescaler = 71;						//ʱ��Ԥ��Ƶ����ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1)
		TIM6_TimeBaseStructure.TIM_ClockDivision = 0;					//������Ƶ
		TIM6_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
		if(flag)
		{
			TIM6_TimeBaseStructure.TIM_Period = 49*4;					//49*1--10kHz,49*5--2kHz;	//������ʱ�䣬ͨ����������ռ�ձ�	
		}
		else
		{
			TIM6_TimeBaseStructure.TIM_Period = 49*1;					//49*1--10kHz,49*5--2kHz;	//������ʱ�䣬ͨ����������ռ�ձ�
		}
		TIM_TimeBaseInit(TIM6, &TIM6_TimeBaseStructure);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}

	for(ii=0;ii<sizeof(dm_run_ctl);ii++)
	{
		switch(dm_run_ctl[ii])
		{
			case DM_CW:
			case DM_CCW:
				f_disable = 0;
		  		if(flag) dm_ctrl_one(ii+1,dm_run_ctl[ii]);	
				else   dm_ctrl_one(ii+1,DM_BRAKE);
				break;
		}		
	}

	if(f_disable == 1)
	{
		TIM_Cmd(TIM6, DISABLE);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);	
	}
}

*/

//���ⲿ�洢����16λʱ��Ӳ���ܽ�A0-A24��ʾ���ǵ�ַ��A1-A25��ֵ����������Ҫλ��һ��
//Ϊ�˸��û��ṩ���� ���ѡ�����16λ��ȣ�FSMC�������͵�ַ��ʱ���Զ�Ϊ��������һλ�Ĳ��� 
void fpga_write(unsigned int addr, unsigned short int value)
{
	(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (addr << 1)))) = value;
}

unsigned short int fpga_read(unsigned int addr)
{ 
	return (*((volatile unsigned short int *)(FPGA_BASE_ADDR + (addr << 1))));
	//8bit �ķ�ʽ������ return (*((volatile unsigned short int *)(FPGA_BASE_ADDR + (addr))));
}

unsigned char get_board_id(void)
{
	unsigned char switch_status=0;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);		//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_Disable SWJ ��ȫ���ã�JTAG+SW-DP��
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = IO_SW1 | IO_SW2 | IO_SW3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IO_SW4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	if(GPIO_ReadInputDataBit(GROUP_SW1,IO_SW1)==0)switch_status |= 0x01;
	if(GPIO_ReadInputDataBit(GROUP_SW2,IO_SW2)==0)switch_status |= 0x02;
	if(GPIO_ReadInputDataBit(GROUP_SW3,IO_SW3)==0)switch_status |= 0x04;
	if(GPIO_ReadInputDataBit(GROUP_SW4,IO_SW4)==0)switch_status |= 0x08;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, DISABLE); 		//��������JTAG
	return switch_status;
}

void print_message(void)
{
  char board_message[50];
  debug_info("\n/********************************************************************\n");
  debug_info("Hello, Program is start to run!\n");
  memset(board_message, 0, sizeof(board_message));
  sprintf(board_message,"MAC:%X-%X-%X-%X-%X-%X\nIP:%d.%d.%d.%d\n",
          board_mac[0],
          board_mac[1],
          board_mac[2],
          board_mac[3],
          board_mac[4],
          board_mac[5],
          board_ip[0],
          board_ip[1],
          board_ip[2],
          board_ip[3]);
  debug_info(board_message);

  debug_info("Prj Designer:\t");
  debug_info(prj_designer);
  debug_info("\n");

  debug_info("BSP Version:\t");
  debug_info(BSP_version);
  debug_info("\n");

  debug_info("Prj Version:\t");
  debug_info(prj_version);
  debug_info("\n");

  //if(get_fpga_version())
  //{
    debug_info("FPGA Version:\t");
    debug_info(FPGA_version);
    debug_info("\n");  
  //}

  debug_info("********************************************************************/\n");
}

//unsigned char cFpgaVersion[12];
//unsigned char get_fpga_version(void)
//{
//	unsigned short int nValue = 0;
//	unsigned char i = 0;
//	memset(cFpgaVersion, 0, sizeof(cFpgaVersion));
//	nValue = fpga_read(0xF000);
//	if((nValue >> 8) != 'V')
//		return false;
//	else
//	{
//		for(i = 0; i < 6; i++)
//		{
//			nValue = fpga_read(0xF000 + (i<<1));
//			cFpgaVersion[i<<1] = nValue >> 8;
//			cFpgaVersion[(i<<1)+1] = nValue & 0xFF;		
//		}
//		return true;
//	}
//}

unsigned char get_fpga_version(void)
{
	unsigned short int nValue = 0;
	unsigned char i = 0;
	memset(FPGA_version, 0, sizeof(FPGA_version));
	nValue = fpga_read(0xF000);
	if((nValue >> 8) != 'V')
		return false;
	else
	{
		for(i = 0; i < 6; i++)
		{
			nValue = fpga_read(0xF000 + (i<<1));
			FPGA_version[i<<1] = nValue >> 8;
			FPGA_version[(i<<1)+1] = nValue & 0xFF;		
		}
		return true;
	}
}

//
//void JumpToAddress(uint32 ApplicationAddress)
//{	
//	pFunction Jump_To_Application; 
//	uint32 JumpAddress;
//	uint32 nValue; 
//	char cBuf[64];
//	memset(cBuf, 0 ,sizeof(cBuf)); 
//    
//	nValue = (*(vu32*)ApplicationAddress) & 0x2FFF0000; 
//    //if (((*(vu32*)ApplicationAddress) & 0x2FFF0000 ) == 0x20000000)
//	if (nValue == 0x20000000)
//    {		
//		sprintf(cBuf, "OK, StackTop=0x%x!\n",nValue);
//		debug_info(cBuf);
//		//NVIC_DeInit();
//	   __disable_irq();
//       JumpAddress = *(vu32*) (ApplicationAddress+4);
//       Jump_To_Application = (pFunction) JumpAddress;
//       /* Initialize user application's Stack Pointer */
//       __set_MSP(*(vu32*) ApplicationAddress); 
//       Jump_To_Application();
//    }
//	else
//	{
//		sprintf(cBuf, "Fail, StackTop=0x%x!\n",nValue);
//		debug_info(cBuf);
//	}
//}
//
//uint8 Programming_CheckData(uint32 Address, uint8 *buf, uint16 len)
//{
//	uint16 RamSource;
// 	uint16 i;
//	char cBuf[32];
//	memset(cBuf,0,sizeof(cBuf));
//	//uint8 status;
//	if((Address%PAGE_SIZE)==0)
//	  FLASH_ErasePage(Address);
//	
//	len=(len+1)/2;
//	
//	for(i=0; i<len; i++)
//	{
//	  //cpyU16fU8(RamSource, buf);
//	  //RamSource=(uint16)buf;
//	  //sprintf(cBuf, "[0x%x:0x%x]", Address, RamSource);
//	  //debug_info(cBuf);	 
//	  memcpy((uint8*)&RamSource, buf, 2);
//	  FLASH_ProgramHalfWord( Address, RamSource); 
//	  if ( *(uint16*)Address != RamSource )
//	     return(1);
//	  
//	  buf+=2;
//	  Address+=2;
//	}
//	return(0);
//}


