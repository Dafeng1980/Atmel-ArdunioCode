/*
 * GccApplication2_key_led.c
 *
 * Created: 2012/6/9 10:56:45
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/sfr_defs.h>

void io_init(void)
{
	DDRB=0xFF;
	PORTB=0x00;
	DDRC=0x0C;
	PORTC=0xF3;
	DDRD=0x00;
	PORTD=0xFF;
}

int main(void)
{
    io_init();
	while (1)
    {
		if(bit_is_clear(PIND,PD2))
		{
			PORTB=_BV(PB0);
			
        //TODO:: Please write your application code 
    }
	if (bit_is_clear(PIND,PD3))
	{
		PORTB=_BV(PB1);
	}
	if (bit_is_clear(PIND,PD4))
	{
		PORTB=_BV(PB2);
	}
	if (bit_is_clear(PIND,PD5))
	{
		PORTB=_BV(PB3);
	}
	if (bit_is_clear(PIND,PD6))
	{
		PORTB=_BV(PB4);
	}
	if (bit_is_clear(PIND,PD7))
	{
		PORTB=_BV(PB5);
	}
}
}