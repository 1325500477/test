#include "my_fun.h"

//0x00~0x0Fת��ΪASCII��
unsigned char hextoa(unsigned char dat)
{
	if (dat <= 9)
		return (dat+'0');
	else
		return (dat+('A'-10));
}

//0~9,A~F��ASCII�뻹ԭ��0~15
unsigned char atohex(unsigned char dat)
{
	if (dat <= '9')
		return (dat-'0');
	else
		return (dat-('A'-10));
}

//�̶�����ʱ������CPU��Դ
void delay_short(void)
{
	unsigned int i = 100;
	while(i--)
		;
}

//�Զ������ʱ������CPU��Դ
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
