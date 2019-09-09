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
//uart ����һ�ֽ�
int 
usart_putchar(char c)
{
	if(c=='\n')
	usart_putchar('\r');
	loop_until_bit_is_set(UCSRA,UDRE);
	UDR=c;
	return 0;
}
//uart ����һ�ֽ�
int 
usart_getchar(void)
{
	loop_until_bit_is_set(UCSRA,RXC);
	return UDR;
}
void IoInit(void)
{
	//uart ��ʼ��
	UCSRB=_BV(RXEN)|_BV(TXEN);/*(1<<RXCIE)|(1<<TXCIE)|*/
	UBRRL=11; //38400 baud 1.86MHz
	//�����豸����
	FILE *fdevopen(usart_putchar,usart_getchar,0);
}

int main(void)
{
	int tmp;
	IoInit();
	while(1)
	{
	     
		//����2
		printf("����2[����һ�ַ���]:\n");
		scanf("%s",g_aString);
		printf("��������Ϊ:%s\n",g_aString);
		//����3
		printf_P(PSTR("����3[��������]:\n"));
		scanf_P(PSTR("%d"),&tmp);
		printf_P(PSTR("��������Ϊ:%d\n"),tmp);
	}
}
   
