//Tab size: 4;	Indent size: 4
#include "stm32f10x.h"
#include "./Driver/platform.h"
#include "bsp.h"

bool serial_cmd_busy_flag = false;          //串口通信（接收-》解析-》应答 完成）
bool serial_cmd_recv_flag = false;			//串口已收到一包数据
bool serial_send_finished_flag = true;		//串口发送数据结束
bool serial_cmd_error_flag = false;			//串口收到错误命令(系统暂时未使用，保留）
unsigned int serial_cmd_send_length = 0;	//串口需要共发送的字节数
unsigned int serial_cmd_data_len = 0;

unsigned char serial_tx_out = 1;			//真实发送的字节数
unsigned int receive_time_last = 0;			//上次收到数据的时间
bool serial_start_to_receive_flag = 0;		//串口开始接收到数据
bool serial_rx_timeout_flag = 0;			//接收超时
//bool serial_tx_timeout_flag = 0;			//发送超时

//初始化串口相关寄存器，设置波特率，并开启串口的中断
void init_uart(unsigned char port_no, unsigned int baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//初始化串口2为调试串口，直接用printf功能输出
	if(port_no == 1)
	{
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE); 
		/*
		*  USART2_TX -> PA2 , USART2_RX -> PA3
		*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART2, &USART_InitStructure);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE); 
		USART_Cmd(USART2, ENABLE);

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		/* Enable the USART2 Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	//初始化串口4为通信串口，同外界交互指令
	else if(port_no == 2)
	{
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC ,ENABLE);
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE); 
		/*
		*  USART4_TX -> PC10 , USART4_RX -> PC11
		*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* USART4 */
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		/* Configure USART4 */
		USART_Init(UART4, &USART_InitStructure);
		//采用传统的中断收发方式
		USART_ClearFlag(UART4,USART_FLAG_TC);
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); 	//接收中断允许
		USART_ITConfig(UART4, USART_IT_TXE, DISABLE);	//发送中断暂时关闭，当启动串口发送命令的时候，开启此中断
		USART_Cmd(UART4, ENABLE);

		/* Enable the USART4 Interrupt */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}

//串口2中断
void USART2_IRQHandler(void)
{
	unsigned char tmp_data;
	static unsigned int length;
	static unsigned int index;
	static unsigned char operation = STEP1;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//接收中断
	{
		tmp_data = USART_ReceiveData(USART2);
		if(serial_rx_timeout_flag)
		{
			serial_rx_timeout_flag = 0;
			operation = STEP1;
		}
		switch(operation)
		{
			case STEP1:
				if(tmp_data == MSG_SYN1)
				{
					comm_recv_buf[POS_SYN1] = MSG_SYN1;				
					operation = STEP2;
					serial_start_to_receive_flag = 1;
					receive_time_last = systemTime;
				}
				break;
			case STEP2:
				if(tmp_data == MSG_SYN2)
				{
					comm_recv_buf[POS_SYN2] = MSG_SYN2;
					receive_time_last = systemTime;
					operation = STEP3;
				}
				else
					operation = STEP1;
				break;
			case STEP3:
				if(tmp_data == MSG_STX)
				{
					comm_recv_buf[POS_STX] = MSG_STX;
					receive_time_last = systemTime;
					operation = STEP4;
				}
				else
					operation = STEP1;
				break;
			case STEP4:
				if(tmp_data == MSG_VER)
				{
					comm_recv_buf[POS_VER] = MSG_VER;
					receive_time_last = systemTime;
					operation = STEP5;
				}
				else
					operation = STEP1;
				break;
			case STEP5:
				//if(tmp_data == board_addr)
				if(1)
				{
					comm_recv_buf[POS_ADDR_RECV] = tmp_data;
					receive_time_last = systemTime;
					operation = STEP6;
				}
				else
					operation = STEP1;
				break;
			case STEP6:
				comm_recv_buf[POS_ADDR_SEND] = tmp_data;
				receive_time_last = systemTime;
				operation = STEP7;
				break;	
			case STEP7:
	#ifdef CMD_NUM_ADD_AUTO		
				//if((tmp_data == (cmd_num_last_time + 1)) || (tmp_data == 0))
				if((tmp_data != cmd_num_last_time) || (tmp_data == 0))
				{
					comm_recv_buf[POS_CMD_NUM] = tmp_data;	
					receive_time_last = systemTime;
					index = 0;
					operation = STEP8;
				}				
	#else
				if(1)				
				{
					comm_recv_buf[POS_CMD_SEQ] = tmp_data;
					receive_time_last = systemTime;
					index = 0;
					operation = STEP8;
				}
	#endif			
				break;
			case STEP8:
				comm_recv_buf[POS_CMD_LEN_H + index] = tmp_data;
				receive_time_last = systemTime;
				index++;
				if(index == 2)
				{
				  	length = comm_recv_buf[POS_CMD_LEN_H];
					length = length << 8;
					length += comm_recv_buf[POS_CMD_LEN_L];
					
					if((length > 0) && (length <= MAX_CMD_CONTENT_LENGTH))
					{
					  	index = 0;
						operation = STEP9;
					}
					else
					  	operation = STEP1;
				}
				break;
			case STEP9:
				comm_recv_buf[POS_CMD_TYPE + index] = tmp_data;
				receive_time_last = systemTime;
				index ++;
				if(index == length)
					operation = STEP10;
				break;
			case STEP10:	//LRC
				comm_recv_buf[POS_CMD_TYPE + index] = tmp_data;
				receive_time_last = systemTime;
				index ++;
				operation = STEP11;
				break;
			case STEP11:
				if(tmp_data == MSG_ETX)
				{
					comm_recv_buf[POS_CMD_TYPE + index] = MSG_ETX;
					serial_cmd_recv_flag = TRUE;
					serial_start_to_receive_flag = 0;
					index = 0;
					operation = STEP1;
				}
				break;
		}
	}
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)	//发送缓冲区空
	{
		if (serial_tx_out < serial_cmd_send_length)
		{
			USART_SendData(USART2, comm_send_buf[serial_tx_out]);
			serial_tx_out++;
		}
		else
		{
			serial_send_finished_flag = TRUE;
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	}
}

void UART4_IRQHandler(void)
{
	unsigned char tmp_data;
	static unsigned int length;
	static unsigned int index = 0;
	static unsigned char operation = STEP1;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)	//接收中断
	{
		tmp_data = USART_ReceiveData(UART4);
		if(serial_rx_timeout_flag)
		{
			serial_rx_timeout_flag = 0;
			operation = STEP1;
		}
		switch(operation)
		{
			case STEP1:
				if(tmp_data == MSG_SYN1)
				{
					comm_recv_buf[POS_SYN1] = MSG_SYN1;				
					operation = STEP2;
					serial_start_to_receive_flag = 1;
					receive_time_last = systemTime;
				}
				break;
			case STEP2:
				if(tmp_data == MSG_SYN2)
				{
					comm_recv_buf[POS_SYN2] = MSG_SYN2;
					receive_time_last = systemTime;
					operation = STEP3;
				}
				else
					operation = STEP1;
				break;
			case STEP3:
				if(tmp_data == MSG_STX)
				{
					comm_recv_buf[POS_STX] = MSG_STX;
					receive_time_last = systemTime;
					operation = STEP4;
				}
				else
					operation = STEP1;
				break;
			case STEP4:
				if(tmp_data == MSG_VER)
				{
					comm_recv_buf[POS_VER] = MSG_VER;
					receive_time_last = systemTime;
					operation = STEP5;
				}
				else
					operation = STEP1;
				break;
			case STEP5:
				//if(tmp_data == board_addr)
				if(1)
				{
					comm_recv_buf[POS_ADDR_RECV] = tmp_data;
					receive_time_last = systemTime;
					operation = STEP6;
				}
				else
					operation = STEP1;
				break;
			case STEP6:
				comm_recv_buf[POS_ADDR_SEND] = tmp_data;
				receive_time_last = systemTime;
				operation = STEP7;
				break;	
			case STEP7:
	#ifdef CMD_NUM_ADD_AUTO		
				//if((tmp_data == (cmd_num_last_time + 1)) || (tmp_data == 0))
				if((tmp_data != cmd_num_last_time) || (tmp_data == 0))
				{
					comm_recv_buf[POS_CMD_NUM] = tmp_data;	
					receive_time_last = systemTime;
					index = 0;
					operation = STEP8;
				}				
	#else
				if(1)				
				{
					comm_recv_buf[POS_CMD_SEQ] = tmp_data;
					receive_time_last = systemTime;
					index = 0;
					operation = STEP8;
				}
	#endif			
				break;
			case STEP8:
				comm_recv_buf[POS_CMD_LEN_H + index] = tmp_data;
				receive_time_last = systemTime;
				index++;
				if(index == 2)
				{
				  	length = comm_recv_buf[POS_CMD_LEN_H];
					length = length << 8;
					length += comm_recv_buf[POS_CMD_LEN_L];
					
					if((length > 0) && (length <= MAX_CMD_CONTENT_LENGTH))
					{
					  	index = 0;
						operation = STEP9;
					}
					else
					  	operation = STEP1;
				}
				break;
			case STEP9:
				comm_recv_buf[POS_CMD_TYPE + index] = tmp_data;
				receive_time_last = systemTime;
				index ++;
				if(index == length)
					operation = STEP10;
				break;
			case STEP10:	//LRC
				comm_recv_buf[POS_CMD_TYPE + index] = tmp_data;
				receive_time_last = systemTime;
				index ++;
				operation = STEP11;
				break;
			case STEP11:
				if(tmp_data == MSG_ETX)
				{
					comm_recv_buf[POS_CMD_TYPE + index] = MSG_ETX;
					serial_cmd_recv_flag = TRUE;
					serial_start_to_receive_flag = 0;
					serial_cmd_data_len = PACKAGE_HEAD_LENGTH + PACKAGE_INFO_LENGTH + length + PACKAGE_TAIL_LENGTH;
					index = 0;
					operation = STEP1;
				}
				break;
		}
		//USART_ClearITPendingBit(UART4, USART_IT_RXNE); 	//可以不用清除，读取缓冲区时自动清除
	}
	if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)	//发送缓冲区空
	{
		if (serial_tx_out < serial_cmd_send_length)
		{
			USART_SendData(UART4, comm_send_buf[serial_tx_out]);
			serial_tx_out++;
		}
		else
		{
			serial_send_finished_flag = TRUE;
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
		}
	}
}

//串口发送数据，手动发送第一字节，并开启发送中断，后续数据自动由中断发送
void serial_send_msg(void)
{
	serial_send_finished_flag = FALSE;
	serial_tx_out = 0;
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
}

//处理接收超时，防止凌乱数据导致串口无法收到新的数据包（以前程序存在此问题）
void process_serial(void)
{
	//处理超时，拼包等事情
	if(serial_start_to_receive_flag)
	{
		if(check_time_delay(receive_time_last, 50))//50mS超时
		{
			serial_start_to_receive_flag = 0;
			serial_rx_timeout_flag = 1;
		}	
	}
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
//CPU板----USART2
//IO底板----UART4
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  //USATR2
  USART_SendData(USART2, (uint8_t) ch);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

/*  
  //UART4
  USART_SendData(UART4, (uint8_t) ch);
//while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
  while (!(UART4->SR & USART_FLAG_TXE));	//效率更高
*/
    
  return ch;
}
