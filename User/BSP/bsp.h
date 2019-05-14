#ifndef __BSP_H__
#define __BSP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <stdio.h>
#include <stdbool.h>	

#include "stdint.h"	
#include "./Driver/standard.h"
#include "./Driver/my_fun.h"
#include "./Driver/hardware.h"
#include "./Driver/driver.h"
#include "./Serial/Serial.h"
#include "./Ethernet/Ethernet.h"
//#include "usb.h"
#include "./Communicate/Communicate.h"
#include "./Command/bsp_command.h"
#include "./EEPROM/eeprom.h"
#include "./EEPROM/eeprom_par.h"
#include "./StepMotor/StepMotor.h"
#include "./OpCtrl/OpCtrl.h"
#include "./Test/Test.h"
#include "BspConfig.h"

#define VERSION_LENGTH 25	//  V1.16.0728.1
#define TYPE_SPEED_TABLE    unsigned short			//加速表数据类型和AVR框架统一
#define TYPE_TIME_BASE		U32						//CheckTimeOut数据类型和AVR框架统一

#define MAX_NUM_DC_MOTOR	12
#define MAX_NUM_ST_MOTOR	8
#define MAX_NUM_INPUT		32
#define OP_MAX_NUM			50	   

#define APP_UDP_PORT    8080	//UDP通信的端口
#define APP_TCP_PORT    8081	//TCP通信的端口

#define BootStart			0x8000000U
#define	BootloaderAddress	(0x8000000+0x00000)  //Bootloader开始地址
#define	UserAppAddress		(0x8000000+0x10000)  //APP开始地址

extern char * prj_build_time;
extern char * prj_designer;
extern char * prj_version;
extern char * BSP_version;
extern char * FPGA_version;
extern unsigned char board_addr;
extern unsigned char sender_addr;
extern unsigned char board_mac[];
extern unsigned char board_ip[];
extern unsigned char device_status;


extern stOpCtrl *gMachineOperation[OP_MAX_NUM];
extern unsigned char nTotalOperation;


#ifdef __cplusplus
}
#endif

#endif
