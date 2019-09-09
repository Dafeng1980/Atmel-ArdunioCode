/*
 * GccAppUART.c
 *Mega168PA @ 7.3728
 *
 * Created: 2012/6/24 0:47:47
 *  Author: fe
 */ 
#include <avr/io.h>
#include <avr/sfr_defs.h>
#define uchar unsigned char
#define uint unsigned int
//uart 发送一字节数据
void putc(uchar c)
{
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0=c;
}
//uart 等待并接收一字节数据
uchar getc(void)
{
	loop_until_bit_is_set(UCSR0A,RXC0);
	return UDR0;
}
int main(void)
{
	//uart 初始化
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	//UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRR0=47;
	; //baud=9600 UBRR=CK/(baud*16) -1
	while(1)
	{
		putc(getc());
	}
}			