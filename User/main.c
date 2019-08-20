 /**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    01-September-2017
  * @brief   Main program body.
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
#include <string.h>
//#include "hw_config.h"
//#include "usb_lib.h"
//#include "usb_desc.h"
//#include "usb_pwr.h"
#include "stm32f10x.h"
#include "./Driver/platform.h"
#include "./Process/process.h"
#include "./Parameter/parameter.h"
#include "bsp.h"

//extern __IO  uint32_t Receive_length ;
//extern __IO  uint32_t length ;
uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

char * prj_designer = "ChenWenjian";;
char * prj_version = "V2.17.1116.1";
char * FPGA_version = "V1.YY.MMDD.C";

unsigned char board_addr = 1;  //�ڴ��е��豸ID,RS485ͨѶʱ���õ���
unsigned char sender_addr = 1;
unsigned char board_mac[6] = {0xAA,0xAA,0xAA,0xAA,0xAA,51};
unsigned char board_ip[4] = {192,168,200,51};
unsigned char device_status = 0;	//�豸״̬


extern unsigned char g_printer_now_status;
 	
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main.
* Descriptioan    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

int main(void)
{
	/* System clocks configuration ---------------------------------------------*/

	__disable_irq();

	InitBspConfig();		//��ʼ���ײ�����

  	InitHardware();			//��ʼ��cpu�Ĵ��� 	

	init_uart(1,38400);		//CPU�����ϵĴ���
	init_uart(2,38400);		//IO�װ��ϵĴ���

	debug_info("=============Program Begin!=============\n"); 

	board_addr = get_board_id(); //�̶�IP��ַ��"0"����192.168.200.10,�Դ�����
	board_mac[5] = 10 + board_addr;
	board_ip[3] = 10 + board_addr;

	__enable_irq();
	init_ethernet();
	//init_usb();				//��ʼ��USB��
	
	init_step_motor();
	init_parameter();

	//iwdg_feed();					//ι��

	delay_ms(2000);

	InitializeMachine();		    //��ʼ���豸
	print_message();				//��ӡ��·�������Ϣ

	//iwdg_feed();					//ι��
	while(1)
	{
	//ram_test();
		//MutipleCommunicate();

			ProcessCmd();

		input_get_all();
		process_serial();
		process_ethernet();
		//process_usb();			//USB��
		comm_cmd_execution();		//����ͨѶ������/���ڣ�
       StepCtrlMachine();
		//iwdg_feed();				//ι��	 
		//board_test();
		//fpga_test();
		process_all_cmd();
	}
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: debug_info("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
