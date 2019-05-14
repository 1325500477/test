#ifndef __HARDWARE_H_
#define __HARDWARE_H_

#include "./Driver/driver.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef  void (*pFunction)(void);
#define	BootloaderAddress	(0x8000000+0x00000)  //Bootloader开始地址
#define	UserAppAddress		(0x8000000+0x10000)  //APP开始地址

//IS61LV25616  = 256K * 16 / 8 =  0x80000
#define SRAM_BLANK1_START_ADDR  ((uint32_t)0x68000000)
#define SRAM_BLANK1_END_ADDR	((uint32_t)0x6807FFFF)


//定义输入输出端口
//A21和A22为高
//FSMC_NE4	A18 A19 A20
//#define OUTPUT_GROUP1		(*((volatile unsigned short int *)0x6C000080))
//#define OUTPUT_GROUP2		(*((volatile unsigned short int *)0x6C000200))
//#define INPUT_GROUP1		(*((volatile unsigned short int *)0x6C000280))
//#define INPUT_GROUP2		(*((volatile unsigned short int *)0x6C000400))
//#define CS_DM9000			(*((volatile unsigned short int *)0x6C000480))
//#define CS_LCD			(*((volatile unsigned short int *)0x6C000600))
//#define CS_FPGA_READ		(*((volatile unsigned short int *)0x6C000600))
//#define CS_FPGA_WRITE		(*((volatile unsigned short int *)0x6C000600))

#define FPGA_BASE_ADDR	0x6C000000
#define FPGA_ADDR0			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0000 << 1))))
#define FPGA_ADDR1			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0002 << 1))))
#define FPGA_ADDR2			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0004 << 1))))
#define FPGA_ADDR3			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0008 << 1))))
#define FPGA_ADDR4			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0010 << 1))))
#define FPGA_ADDR5			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0020 << 1))))
#define FPGA_ADDR6			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0040 << 1))))
#define FPGA_ADDR7			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0080 << 1))))
#define FPGA_ADDR8			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0100 << 1))))
#define FPGA_ADDR9			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0200 << 1))))
#define FPGA_ADDR10			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0400 << 1))))
#define FPGA_ADDR11			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x0800 << 1))))
#define FPGA_ADDR12			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x1000 << 1))))
#define FPGA_ADDR13			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x2000 << 1))))
#define FPGA_ADDR14			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x4000 << 1))))
#define FPGA_ADDR15			(*((volatile unsigned short int *)(FPGA_BASE_ADDR + (0x8000 << 1))))


//电机脉冲
#define GROUP_SM_PUL1		GPIOE
#define IO_SM_PUL1			GPIO_Pin_5	//PE5
#define GROUP_SM_PUL2		GPIOE
#define IO_SM_PUL2			GPIO_Pin_4	//PE4
#define GROUP_SM_PUL3		GPIOB
#define IO_SM_PUL3			GPIO_Pin_7	//PB7
#define GROUP_SM_PUL4		GPIOB
#define IO_SM_PUL4			GPIO_Pin_6	//PB6
#define GROUP_SM_PUL5		GPIOG
#define IO_SM_PUL5			GPIO_Pin_9	//PG9
#define GROUP_SM_PUL6		GPIOA
#define IO_SM_PUL6			GPIO_Pin_12	//PA12	//BAK1
#define GROUP_SM_PUL7		GPIOB
#define IO_SM_PUL7			GPIO_Pin_11	//PB11	//BAK4
#define GROUP_SM_PUL8		GPIOG
#define IO_SM_PUL8			GPIO_Pin_6	//PG6	//BAK7

//电机使能
#define GROUP_SM_ENA1		GPIOE
#define IO_SM_ENA1			GPIO_Pin_3	//PE3
#define GROUP_SM_ENA2		GPIOE
#define IO_SM_ENA2			GPIO_Pin_2	//PE2
#define GROUP_SM_ENA3		GPIOG
#define IO_SM_ENA3			GPIO_Pin_15	//PG15
#define GROUP_SM_ENA4		GPIOG
#define IO_SM_ENA4			GPIO_Pin_14	//PG14
#define GROUP_SM_ENA5		GPIOD
#define IO_SM_ENA5			GPIO_Pin_6	//PD6
#define GROUP_SM_ENA6		GPIOB
#define IO_SM_ENA6			GPIO_Pin_12	//PB12	//BAK3
#define GROUP_SM_ENA7		GPIOC
#define IO_SM_ENA7			GPIO_Pin_6	//PC6	//BAK6
//没有第8路ENA
#define GROUP_SM_ENA8		GPIOC
#define IO_SM_ENA8			GPIO_Pin_6	//PC6	//BAK6

//电机方向
#define GROUP_SM_DIR1		GPIOE
#define IO_SM_DIR1			GPIO_Pin_6	//PE6
#define GROUP_SM_DIR2		GPIOB
#define IO_SM_DIR2			GPIO_Pin_9	//PB9
#define GROUP_SM_DIR3		GPIOB
#define IO_SM_DIR3			GPIO_Pin_8	//PB8
#define GROUP_SM_DIR4		GPIOG
#define IO_SM_DIR4			GPIO_Pin_13	//PG13
#define GROUP_SM_DIR5		GPIOG
#define IO_SM_DIR5			GPIO_Pin_11	//PG11
#define GROUP_SM_DIR6		GPIOA
#define IO_SM_DIR6			GPIO_Pin_11	//PA11	//BAK2
#define GROUP_SM_DIR7		GPIOG
#define IO_SM_DIR7			GPIO_Pin_8	//PG8	//BAK5
#define GROUP_SM_DIR8		GPIOG
#define IO_SM_DIR8			GPIO_Pin_7	//PG7	//BAK8


//输出OUT1~OUT24
#define GROUP_OUT1		GPIOB
#define IO_OUT1			GPIO_Pin_13	//PB13
#define GROUP_OUT2		GPIOB
#define IO_OUT2			GPIO_Pin_14	//PB14
#define GROUP_OUT3		GPIOB
#define IO_OUT3			GPIO_Pin_15	//PB15
#define GROUP_OUT4		GPIOB
#define IO_OUT4			GPIO_Pin_5	//PB5
#define GROUP_OUT5		GPIOC
#define IO_OUT5			GPIO_Pin_13	//PC13
#define GROUP_OUT6		GPIOC
#define IO_OUT6			GPIO_Pin_14	//PC14
#define GROUP_OUT7		GPIOC
#define IO_OUT7			GPIO_Pin_15	//PC15
#define GROUP_OUT8		GPIOF
#define IO_OUT8			GPIO_Pin_6	//PF6
#define GROUP_OUT9		GPIOF
#define IO_OUT9			GPIO_Pin_7	//PF7
#define GROUP_OUT10		GPIOF
#define IO_OUT10		GPIO_Pin_8	//PF8
#define GROUP_OUT11		GPIOF
#define IO_OUT11		GPIO_Pin_9	//PF9
#define GROUP_OUT12		GPIOF
#define IO_OUT12		GPIO_Pin_10	//PF10
#define GROUP_OUT13		GPIOB
#define IO_OUT13		GPIO_Pin_0	//PB0
#define GROUP_OUT14		GPIOB
#define IO_OUT14		GPIO_Pin_1	//PB1
#define GROUP_OUT15		GPIOF
#define IO_OUT15		GPIO_Pin_11	//PF11
#define GROUP_OUT16		GPIOC
#define IO_OUT16		GPIO_Pin_7	//PC7
#define GROUP_OUT17		GPIOC
#define IO_OUT17		GPIO_Pin_8	//PC8
#define GROUP_OUT18		GPIOC
#define IO_OUT18		GPIO_Pin_9	//PC9
#define GROUP_OUT19		GPIOA
#define IO_OUT19		GPIO_Pin_8	//PA8
#define GROUP_OUT20		GPIOA
#define IO_OUT20		GPIO_Pin_9	//PA9
#define GROUP_OUT21		GPIOA
#define IO_OUT21		GPIO_Pin_10	//PA10
#define GROUP_OUT22		GPIOC
#define IO_OUT22		GPIO_Pin_12	//PC12
#define GROUP_OUT23		GPIOD
#define IO_OUT23		GPIO_Pin_2	//PD2
#define GROUP_OUT24		GPIOD
#define IO_OUT24		GPIO_Pin_3	//PD3
////BAK1~8
//#define GROUP_BAK1		GPIOA
//#define IO_BAK1			GPIO_Pin_12	//PA12
//#define GROUP_BAK2		GPIOA
//#define IO_BAK2			GPIO_Pin_11	//PA11
//#define GROUP_BAK3		GPIOB
//#define IO_BAK3			GPIO_Pin_12	//PB12
//#define GROUP_BAK4		GPIOB
//#define IO_BAK4			GPIO_Pin_11	//PB11
//#define GROUP_BAK5		GPIOG
//#define IO_BAK5			GPIO_Pin_8	//PG8
//#define GROUP_BAK6		GPIOC
//#define IO_BAK6			GPIO_Pin_6	//PC6
//#define GROUP_BAK7		GPIOG
//#define IO_BAK7			GPIO_Pin_6	//PG6
//#define GROUP_BAK8		GPIOG
//#define IO_BAK8			GPIO_Pin_7	//PG7

//Led
#define	GROUP_LED_RED	GPIOA	
#define	IO_LED_RED		GPIO_Pin_0	//PA0
#define	GROUP_LED_BLUE	GPIOA	
#define	IO_LED_BLUE		GPIO_Pin_1	//PA1

#define GROUP_USB_EN	GPIOC
#define IO_USB_EN		GPIO_Pin_1	//PC1


//START & FINISHED (MCU communicate with FPGA)
//#define GROUP_START		GPIOB
//#define IO_START		GPIO_Pin_12	//PB12(CS2)
//#define IO_START		GPIO_Pin_13	//PB13(SCK2)
//#define IO_START		GPIO_Pin_15	//PB15(MOSI)

//#define GROUP_START		GPIOD
//#define IO_START		GPIO_Pin_13	//PD13(FSMC_A18)


//#define GROUP_FINISHED	GPIOB
//#define IO_FINISHED		GPIO_Pin_14	//PB14(MISO2)



//#define NET_CS		GPIO_Pin_4		//PA4

//拨码开关定义-------------------
#define	GROUP_SW1	   	GPIOA
#define IO_SW1			GPIO_Pin_15	//PA15
#define	GROUP_SW2	   	GPIOA
#define IO_SW2			GPIO_Pin_13	//PA13
#define	GROUP_SW3	   	GPIOA
#define IO_SW3			GPIO_Pin_14	//PA14
#define	GROUP_SW4	   	GPIOB
#define IO_SW4			GPIO_Pin_3	//PB3


//PRINTER_STROBE1 PF11
//#define GROUP_PRINTER_STROBE1   GPIOF
//#define IO_PRINTER_STROBE1      GPIO_Pin_11

#define IN1		1
#define IN2		2
#define IN3		3
#define IN4		4
#define IN5		5
#define IN6		6
#define IN7		7
#define IN8		8
#define IN9		9
#define IN10	10
#define IN11	11
#define IN12	12
#define IN13	13
#define IN14	14
#define IN15	15
#define IN16	16
#define IN17	17
#define IN18	18
#define IN19	19
#define IN20	20
#define IN21	21
#define IN22	22
#define IN23	23
#define IN24	24
#define IN25	25
#define IN26	26
#define IN27	27
#define IN28	28
#define IN29	29
#define IN30	30
#define IN31	31
#define IN32	32
#define IN33	33
#define IN34	34
#define IN35	35
#define IN36	36
#define IN37	37
#define IN38	38
#define IN39	39
#define IN40	40

#define DM1		1
#define DM2		2
#define DM3		3
#define DM4		4
#define DM5		5
#define DM6		6
#define DM7		7
#define DM8		8
#define DM9		9
#define DM10	10
#define DM11	11
#define DM12	12
#define DM13	13
#define DM14	14
#define DM15	15
#define DM16	16	

#define SM_1	1
#define SM_2	2
#define SM_3	3
#define SM_4	4
#define SM_5	5
#define SM_6	6
#define SM_7	7
#define SM_8	8
#define SM_9	9
#define SM_10	10
#define SM_11	11
#define SM_12	12
#define SM_13	13
#define SM_14	14
#define SM_15	15
#define SM_16	16



void InitHardware(void);

#ifdef __cplusplus
}
#endif

#endif
