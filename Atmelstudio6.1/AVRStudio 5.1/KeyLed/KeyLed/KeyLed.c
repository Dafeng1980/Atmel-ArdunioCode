/*
 * KeyLed.c
 *
 * Created: 2012/3/10 14:24:40
 *  Author: fe
 */ 

#include <avr/io.h>

void io_init(void) {
	DDRB=0xff;  //方向输出
	PORTB=0x00;   //低电平
	DDRC=0x0c;    //
	PORTC=0xf3;
	DDRD=0x00;
	PORTD=0xff;
	
}

int main(void)
{
	io_init();
	
    while(1)
    {
		if ((PIND & (1<<PD2)) ==0)
		{
			PORTB = 1 << PB0;
		}
		if ((PIND&(1<<PD3))==0)
		{
			PORTB=1<<PB1;
		}
		if ((PIND&(1<<PD4))==0)
		{
			PORTB=1<<PB2;
		}
		if ((PIND&(1<<PD5))==0)
		{
			PORTB=1<<PB3;
		}
		if ((PIND&(1<<PD6))==0)
		{
			PORTB=1<<PB4;
		}
		if ((PIND&(1<<PD7))==0)
		{
			PORTB=1<<PB5;
		}
        //TODO:: Please write your application code 
    }
}