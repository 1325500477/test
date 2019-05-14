#ifndef __MY_FUN_H__
#define __MY_FUN_H__

#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define DEBUG
#define DEBUG_LEVEL	7
	
//#ifdef DEBUG
//	#define debug(level,fmt,args...)	{if(level > DEBUG_LEVEL) printf(fmt ,##args);}
//#else
//	#define debug(level,fmt,args...)	
//#endif
#ifdef DEBUG
	#define debug_info(fmt,args...)	{printf(fmt ,##args);}
#else
	#define debug_info(fmt,args...)	
#endif

unsigned char hextoa(unsigned char dat);

unsigned char atohex(unsigned char dat);

void delay_short(void);

void delay(unsigned long t);

void delay_ms(unsigned int t);

void delay_us(unsigned int t);

#ifdef __cplusplus
}
#endif

#endif
