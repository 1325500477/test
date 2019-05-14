/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "string.h"
#include "./Driver/platform.h"
#include "./Driver/hardware.h"
#include "./Driver/my_fun.h"
//#include "serial.h"
//#include "usb_lib.h"
//#include "usb_istr.h"

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  //debug_info("HardFault_Handler!\n");
  //uint32_t r_sp ;

  //r_sp = __get_PSP(); //获取SP的值
  if(CoreDebug->DHCSR & 1)
  {
    __breakpoint(0);
  }

  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  debug_info("MemManage_Handler!\n");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  debug_info("BusFault_Handler!\n");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  debug_info("UsageFault_Handler!\n");
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern volatile unsigned long systemTime;
void SysTick_Handler(void)
{
	static unsigned char flag = 0;
	static unsigned int ticket_counter = 0;
	ticket_counter ++;

	systemTime ++;

	if(ticket_counter >= 500)	//1ms * 500
	{
		ticket_counter = 0;	
		if(flag)
		{
			//led_red_on();
			GROUP_LED_RED->BRR = IO_LED_RED; 
		}
		else
		{
			//led_red_off();
			GROUP_LED_RED->BSRR = IO_LED_RED; 
		}
		flag = !flag;
	}
}

/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB Low Priority interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)|| defined (STM32F37X)
void USB_LP_IRQHandler(void)
#else
void USB_LP_CAN1_RX0_IRQHandler(void)
#endif
{
//  USB_Istr();
}

/*******************************************************************************
* Function Name  : USB_FS_WKUP_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
void USB_FS_WKUP_IRQHandler(void)
#else
void USBWakeUp_IRQHandler(void)
#endif
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{
//  if(DMA_GetITStatus(DMA1_IT_HT4))
//  {
//    /* Get Current Data Counter value after complete transfer */
//    //CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel4);
//    /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
//    DMA_ClearITPendingBit(DMA1_IT_HT4);
//	send_half_package_flag = 1;
//  }
//  else if(DMA_GetITStatus(DMA1_IT_TC4))
//  {
//  	DMA_ClearITPendingBit(DMA1_IT_TC4);
//	send_one_package_flag = 1;
//  }
//  else
//  {
//  	while(1)
//	;
//  }
}

//unsigned int package_num = 0;
void DMA1_Channel6_IRQHandler(void)
{
//  if(DMA_GetITStatus(DMA1_IT_HT5))
//  {
//    /* Get Current Data Counter value after complete transfer */
//    //CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel4);
//    /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
//    DMA_ClearITPendingBit(DMA1_IT_HT5);
//	receive_half_package_flag = 1;
//	package_num ++;
//  }
//  else if(DMA_GetITStatus(DMA1_IT_TC5))
//  {
//  	DMA_ClearITPendingBit(DMA1_IT_TC5);
//	receive_one_package_flag = 1;
//	package_num ++;
//  }
//  else
//  {
//  	while(1)
//	;
//  }
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
