/*
 * GccAppT1.c
 *
 * Created: 2012/6/14 19:07:31
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
void io_init(void)
{
	DDRB=0xFF;
	PORTB=0x00;
	DDRC=0x0C;
	PORTC=0xF3;
	DDRD=0x00;
	PORTD=0xFF;
}
void t1_init(void)
{
	OCR1A=1250;
	TIMSK|=_BV(OCIE1A);
	TCCR1A=0x00;
	TCCR1B=_BV(WGM12);
	TCCR1B|=_BV(CS11);
}
SIGNAL(TIMER1_COMPA_vect)
{
	static unsigned char i;
	i++;
	if(i>100)
	{
		PORTB^=_BV(PB0);
		i=0;
	}
}



int main(void)
{
	io_init();
	t1_init();
	sei();
    while(1)
    {
        ;//TODO:: Please write your application code 
    }
}