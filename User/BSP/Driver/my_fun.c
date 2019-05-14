#include "my_fun.h"

//0x00~0x0F转换为ASCII码
unsigned char hextoa(unsigned char dat)
{
	if (dat <= 9)
		return (dat+'0');
	else
		return (dat+('A'-10));
}

//0~9,A~F的ASCII码还原成0~15
unsigned char atohex(unsigned char dat)
{
	if (dat <= '9')
		return (dat-'0');
	else
		return (dat-('A'-10));
}

//固定短延时，消耗CPU资源
void delay_short(void)
{
	unsigned int i = 100;
	while(i--)
		;
}

//自定义短延时，消耗CPU资源
void delay(volatile unsigned long t)
{
	while(t--)
		;
}

void delay_ms(unsigned int ms)    
{ 
	unsigned int i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 


void delay_us(unsigned int us)
{
	volatile unsigned int i,j; 
	for( i = 0; i < us; i++ )
	{ 
		for( j = 0; j < 30; j++ );
	}
}
