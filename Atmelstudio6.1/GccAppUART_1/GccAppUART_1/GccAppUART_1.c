/*
 * GccAppUART_1.c
 *
 * Created: 2012/6/24 20:44:51
 *  Author: fe
 */ 


#include <avr/io.h>
//#include <stdlib.h>
#include <avr/sfr_defs.h>
#include <stdio.h>
#include <avr/pgmspace.h>

char g_aString[81];
//uart 发送一字节
int 
usart_putchar(char c)
{
	if(c=='\n')
	usart_putchar('\r');
	loop_until_bit_is_set(UCSRA,UDRE);
	UDR=c;
	return 0;
}
//uart 接收一字节
int 
usart_getchar(void)
{
	loop_until_bit_is_set(UCSRA,RXC);
	return UDR;
}
void IoInit(void)
{
	//uart 初始化
	UCSRB=_BV(RXEN)|_BV(TXEN);/*(1<<RXCIE)|(1<<TXCIE)|*/
	UBRRL=11; //38400 baud 1.86MHz
	//流与设备连接
	FILE *fdevopen(usart_putchar,usart_getchar,0);
}

int main(void)
{
	int tmp;
	IoInit();
	while(1)
	{
	     
		//测试2
		printf("测试2[输入一字符串]:\n");
		scanf("%s",g_aString);
		printf("您的输入为:%s\n",g_aString);
		//测试3
		printf_P(PSTR("测试3[输入数字]:\n"));
		scanf_P(PSTR("%d"),&tmp);
		printf_P(PSTR("您的输入为:%d\n"),tmp);
	}
}
   
