/*
 * GccApp_int.c
 *
 * Created: 2012/6/12 1:04:28
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/signal.h>


void DelayMs(unsigned int ms)
{
	unsigned int i;
	for(i=0;i<ms;i++)
	_delay_loop_2(250);
}
void io_init(void)
{
	DDRB=0xFF;
	PORTB=0x00;
	DDRC=0x0C;
	PORTC=0xF3;
	DDRD=0x00;
	PORTD=0xFF;
}
void int_init(void)
{
	cli();
	MCUCR=0x00;
	GICR=_BV(INT1)|_BV(INT0);
	sei();
}
//SIGNAL(INT0_vect)
{
	PORTB|=1<<PB0;
	while((PIND&_BV(PD2))==0);
	PORTB&=~_BV(PB0);
	
}
SIGNAL(INT1_vect)
{
	PORTB|=1<<PB1;
	while((PIND&_BV(PB3))==0);
	PORTB&=~_BV(PB1);
}

int main(void)
{
	io_init();
	int_init();
	
    while(1)
    {
		DelayMs(200);
		PORTB^=0x10;
        //TODO:: Please write your application code 
    }
}