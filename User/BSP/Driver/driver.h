//Tab size: 4;	Indent size: 4
#include "stdbool.h"
#include "standard.h"


#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "./OpCtrl/OpCtrl.h" 

#define bool _Bool

#define FOREVER		0xFFFF

#define    RUN        1
#define    STOP       0
//#define    ENABLE     1
//#define    DISABLE    0
//#define    CW         1
//#define    CCW        0


#define SM_ENABLE	1	//高电平，使能
#define SM_DISABLE	0	//低电平，去使能

#define SM_CW		0	//方向为低电平，正转
#define SM_CCW		1	//方向为高电平，反转

#define SM_RUN		1
#define SM_STOP		0

#define DM_ON		1
#define DM_OFF		0

#define DM_CW	    1
#define DM_CCW		2
#define DM_BRAKE	3
#define DM_FREE		0

#define LED_ON		1
#define LED_OFF		0
#define LED_FLASH	2

//typedef union BYTE_BIT
//{
//	unsigned char byte;
//	struct
//	{
//		unsigned char Bit0:1;
//		unsigned char Bit1:1;
//		unsigned char Bit2:1;
//		unsigned char Bit3:1;
//		unsigned char Bit4:1;
//		unsigned char Bit5:1;
//		unsigned char Bit6:1;
//		unsigned char Bit7:1;
//	};
//	
//}BYTEBIT;
///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

extern volatile unsigned long systemTime;	//系统时钟
extern unsigned long m_Input_All_now;		//输入
extern unsigned long m_Input_All_last;		//输入
extern unsigned long m_Output_All;			//输出
extern unsigned long m_DM_Output_All;		//直流马达
extern unsigned char m_DM_Status[];			//直流马达状态数组
//void sys_time_update(void); //-------------
bool check_time_delay(unsigned long base_time, unsigned long wait_time);
void init_board(void);//--------------------
void init_output_ctrl(void);
void init_dm_ctrl(void);

bool input_get_one(unsigned char port_num);
void dm_ctrl_one(unsigned char dm_num, unsigned char dm_status);
void input_get_all(void);
void dm_ctl_all(void);

unsigned char get_board_id(void);
unsigned char get_fpga_version(void);
void print_message(void);
void PreCheckSensor(stOpCtrl *operation,unsigned char targetSensor,bool rightresult,unsigned char nextStep, unsigned int nErrorCode);


void fpga_write(unsigned int addr, unsigned short int value);
unsigned short int fpga_read(unsigned int addr);



void led_blue_off(void);
void led_blue_on(void);
void led_red_off(void);
void led_red_on(void);
#endif
