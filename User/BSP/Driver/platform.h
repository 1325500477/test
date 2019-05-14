/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

  #define USARTy                   USART1
  #define USART1_GPIO              GPIOA
  #define USART1_CLK               RCC_APB2Periph_USART1
  #define USART1_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USART1_RxPin             GPIO_Pin_10
  #define USART1_TxPin             GPIO_Pin_9
  #define USART1_Tx_DMA_Channel    DMA1_Channel4
  #define USART1_Tx_DMA_FLAG       DMA1_FLAG_TC4
  #define USART1_Rx_DMA_Channel    DMA1_Channel5
  #define USART1_Rx_DMA_FLAG       DMA1_FLAG_TC5  
  #define USART1_DR_Base           0x40013804

  //#define USART2                   USART2
  #define USART2_GPIO              GPIOA
  #define USART2_CLK               RCC_APB1Periph_USART2
  #define USART2_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USART2_RxPin             GPIO_Pin_3
  #define USART2_TxPin             GPIO_Pin_2
  #define USART2_Tx_DMA_Channel    DMA1_Channel7
  #define USART2_Tx_DMA_FLAG       DMA1_FLAG_TC7
  #define USART2_Rx_DMA_Channel    DMA1_Channel6
  #define USART2_Rx_DMA_FLAG       DMA1_FLAG_TC6
  #define USART2_DR_Base           0x40004404



#define ONE_PACKAGE_LENGTH	100
#define TOTAL_LENGTH	1000


#endif /* __PLATFORM_CONFIG_H */
