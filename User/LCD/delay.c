#include "delay.h"

//������ʱ����
void delay_ms(u32 i)
{
u32 j;
	for(;i>0;i--)
		for(j=30000;j>0;j--);
}

//΢����ʱ����
void delay_us(u32 i)
{
u32 j;
	for(;i>0;i--)
		for(j=50;j>0;j--);
}


