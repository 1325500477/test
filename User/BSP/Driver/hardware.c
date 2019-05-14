#include "hardware.h"
#include "stm32f10x.h"
#include "platform.h"
//#include "ethernet.h"
#include "./Driver/driver.h"
#include <stdio.h>


void Configure_AD(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* Configure PC.00(ADC Channel0) as analog inputs */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
	/* Enable ADC1 and GPIOC clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADC1 regular channels configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5); 
	/* Enable ADC1 EOC interrupt */
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);	   
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1)); 

	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void Configure_EXTI(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource8);	//ERROR_TPH
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);	//IRQ_NET PC5
	EXTI_ClearITPendingBit(EXTI_Line8);
	EXTI_ClearITPendingBit(EXTI_Line9); 
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿
	EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line5;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿
//	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}

void Configure_NVIC(void)
{
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x10000) ;

//NVIC_PriorityGroup  NVIC_IRQChannel的先占优先级 NVIC_IRQChannel的从优先级		描述 
//NVIC_PriorityGroup_0  		0  						0-15  					先占优先级 0 位 从优先级 4位 
//NVIC_PriorityGroup_1 			0-1						0-7						先占优先级 1 位 从优先级 3位 
//NVIC_PriorityGroup_2			0-3						0-3 					先占优先级 2 位 从优先级 2位 
//NVIC_PriorityGroup_3			0-7						0-1						先占优先级 3 位 从优先级 1位 
//NVIC_PriorityGroup_4			0-15					0						先占优先级 4 位 从优先级 0位

//	NVIC_InitTypeDef NVIC_InitStructure;

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;   
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	      
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;	      
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 											
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;	      
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	

//	/* Configure and enable ADC interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}



void Configure_Serial(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

	//初始化串口1为调试串口，直接用printf功能
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	/*
	*  USART1_TX -> PA9 , USART1_RX -> PA10
	*/				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);		 

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure); 
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//接收区非空
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	//发送区空，建议启动串口发送命令的时候，开启此中断
	//USART_ITConfig(USART2, USART_IT_TC, ENABLE); 	//发送完成中断
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_Cmd(USART1, ENABLE);

//	//本打印应用是否要开启中断？		
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	 	
//	/* Enable the USART1 Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);



//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);		   
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	/* USART2 */
//	USART_InitStructure.USART_BaudRate = 115200;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	   
//	/* Configure USART2 */
//	USART_Init(USART2, &USART_InitStructure);
//	//采用传统的中断收发方式	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);	//启动串口发送命令的时候，开启此中断
//	//USART_ITConfig(USART2, USART_IT_TC, ENABLE); 	//发送完成中断
//	USART_ClearFlag(USART2,USART_FLAG_TC);
//	USART_Cmd(USART2, ENABLE);
//	
//	/* Enable the USART2 Interrupt */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}

void Configure_DMA(void)
{
//	/* DMA clock enable */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//
//	/* USART2 TX DMA1 Channel (triggered by USART2 Tx event) Config */
//	DMA_DeInit(USART2_Tx_DMA_Channel);  
//	DMA_InitStructure_Uart2_TX.DMA_PeripheralBaseAddr = USART2_DR_Base;
//	DMA_InitStructure_Uart2_TX.DMA_MemoryBaseAddr = (uint32_t)Tx2Buffer;
//	DMA_InitStructure_Uart2_TX.DMA_DIR = DMA_DIR_PeripheralDST;
//	DMA_InitStructure_Uart2_TX.DMA_BufferSize = sizeof(Tx2Buffer);
//	DMA_InitStructure_Uart2_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure_Uart2_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure_Uart2_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure_Uart2_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure_Uart2_TX.DMA_Mode = DMA_Mode_Normal;
//	DMA_InitStructure_Uart2_TX.DMA_Priority = DMA_Priority_VeryHigh;
//	DMA_InitStructure_Uart2_TX.DMA_M2M = DMA_M2M_Disable;
//	DMA_Init(USART2_Tx_DMA_Channel, &DMA_InitStructure_Uart2_TX);
//	
//	/* Enable DMA1_Channel7 Transfer Half & Complete interrupt */
//	DMA_ITConfig(USART2_Tx_DMA_Channel, DMA_IT_HT | DMA_IT_TC, ENABLE);
//	/* Enable USART2 TX DMA1 Channel */
//	DMA_Cmd(USART2_Tx_DMA_Channel, ENABLE);
//	
//	/* USART2 RX DMA1 Channel (triggered by USART2 Rx event) Config */
//	DMA_DeInit(USART2_Rx_DMA_Channel);  
//	DMA_InitStructure_Uart2_RX.DMA_PeripheralBaseAddr = USART2_DR_Base;
//	DMA_InitStructure_Uart2_RX.DMA_MemoryBaseAddr = (uint32_t)Rx2Buffer;
//	DMA_InitStructure_Uart2_RX.DMA_DIR = DMA_DIR_PeripheralSRC;
//	DMA_InitStructure_Uart2_RX.DMA_BufferSize = ONE_PACKAGE_LENGTH << 1;  		//接收DMA的中断
//	DMA_InitStructure_Uart2_RX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure_Uart2_RX.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure_Uart2_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure_Uart2_RX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure_Uart2_RX.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;
//	DMA_InitStructure_Uart2_RX.DMA_Priority = DMA_Priority_VeryHigh;
//	DMA_InitStructure_Uart2_RX.DMA_M2M = DMA_M2M_Disable;
//	DMA_Init(USART2_Rx_DMA_Channel, &DMA_InitStructure_Uart2_RX);
//	
//	/* Enable DMA1_Channel6 Transfer Half & Complete interrupt */
//	DMA_ITConfig(USART2_Rx_DMA_Channel, DMA_IT_HT | DMA_IT_TC, ENABLE);
//	/* Enable USART2 TX DMA1 Channel */
//	DMA_Cmd(USART2_Rx_DMA_Channel, ENABLE);
}

//uint16_t CCR1_Val = 100;
//uint16_t PrescalerValue = 0;
void Configure_TIM(void)
{
//	TIM_TimeBaseInitTypeDef	TIM2_TimeBaseStructure;
//	TIM_OCInitTypeDef		TIM2_OCInitStructure;
//
//	TIM_TimeBaseInitTypeDef	TIM3_TimeBaseStructure;
//	TIM_OCInitTypeDef		TIM3_OCInitStructure;
//
//	NVIC_InitTypeDef NVIC_InitStructure;	

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	DBGMCU_Config(DBGMCU_CR_DBG_TIM2_STOP,ENABLE);	//定时器2在debug时，断点处停止计数
	DBGMCU_Config(DBGMCU_CR_DBG_TIM3_STOP,ENABLE);	//定时器2在debug时，断点处停止计数

//	/* Frequency=(SYSCLK/(Prescaler+1)) /(Period+1) */
////	PrescalerValue = (uint16_t) (SystemCoreClock / 2000) - 1;
//	/* Time base configuration */
//	TIM_DeInit(TIM2);	//1K Hz	(1us)
//	TIM2_TimeBaseStructure.TIM_Period = 999;  	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
//	TIM2_TimeBaseStructure.TIM_Prescaler = 71;	/* 时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1) */
//	TIM2_TimeBaseStructure.TIM_ClockDivision = 0;	/* 采样分频 */
//	TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	/* 向上计数模式 */
//	
//	TIM_TimeBaseInit(TIM2, &TIM2_TimeBaseStructure); 	
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//		
//	/* Output Compare Active Mode configuration: Channel1 */
//	TIM2_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
//	TIM2_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM2_OCInitStructure.TIM_Pulse = 370;	//CCR1_Val;
//	TIM2_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//
//	TIM_OC1Init(TIM2, &TIM2_OCInitStructure);
//
//	TIM2_OCInitStructure.TIM_Pulse = TIM2_TimeBaseStructure.TIM_Period / 2;
//	TIM_OC2Init(TIM2, &TIM2_OCInitStructure);
//	
//	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
//	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
//	
//	//TIM_ARRPreloadConfig(TIM2, ENABLE);
//	TIM_ARRPreloadConfig(TIM2, DISABLE);
//	
//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
//	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
//	/* TIM IT enable */
//	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
//	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
//	/* TIM2 enable counter */
//	TIM_Cmd(TIM2, ENABLE);
//
//
//   	//直流马达PWM调速功能	----定时器1通道3刚好接在PA10上，想办法实现
//	TIM_DeInit(TIM3);	//10KHz	(0.1us)
//	TIM3_TimeBaseStructure.TIM_Period = 99;  	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
//	TIM3_TimeBaseStructure.TIM_Prescaler = 71;	/* 时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1) */
//	TIM3_TimeBaseStructure.TIM_ClockDivision = 0;	/* 采样分频 */
//	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	/* 向上计数模式 */
//	
//	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure); 	
//	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//		
//	/* Output Compare Active Mode configuration: Channel1 */
//	TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
//	TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM3_OCInitStructure.TIM_Pulse = 59;	//CCR1_Val;
//	TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//
//	TIM_OC1Init(TIM3, &TIM3_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	
//	TIM_ARRPreloadConfig(TIM3, ENABLE);		
//	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//	/* TIM IT enable */
//	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
//	/* TIM3 enable counter */
//	TIM_Cmd(TIM3, ENABLE);
//
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}

void Configure_USB(void)
{
//	Set_System();
//	Set_USBClock();
//	USB_Interrupts_Config();
//	USB_Init();
}
void InitHardware()
{
	//Configure_AD();
	//Configure_Serial();
	Configure_DMA();
	//Configure_TIM();

	Configure_EXTI();
	Configure_NVIC();
	//ethernet_init();
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External lines 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line1) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
	}				
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9-5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)		
{
	if ( EXTI_GetITStatus(EXTI_Line5) != RESET )   	//IRQ_NET中断
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if ( EXTI_GetITStatus(EXTI_Line6) != RESET )
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if ( EXTI_GetITStatus(EXTI_Line7) != RESET ) 
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if ( EXTI_GetITStatus(EXTI_Line8) != RESET ) 	//IRQ_TPH中断
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if ( EXTI_GetITStatus(EXTI_Line9) != RESET )
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	}				
}
