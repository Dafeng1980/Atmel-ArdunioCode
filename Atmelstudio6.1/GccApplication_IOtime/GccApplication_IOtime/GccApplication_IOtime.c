/*
 * GccApplication_IOtime.c
 *
 * Created: 2016/12/26 12:55:17
 *  Author: PE_NPI
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define Pi 3.14
#define HIGH 1

int main(void)

{
	unsigned char i;
	i=0;
	float val;
	
	
	PORTG=0x00;
	DDRG=0x0F;
    while(1)
    {
		i++;
		if(i == 0xff)
		i=0;
		PORTG=HIGH;
		//PORTG=0b00000001;
		//PORTG |=_BV(PG0);
		 val = i/Pi;
		// PORTB = val;
		 PORTB = val;
		//asm("nop");
		//_delay_us(2);
	    PORTG=0b00000000;
		//PORTG=0b00000001;
		//asm("nop");
		//PORTG=0x01;
		//asm("nop");
				_delay_us(5);
		//PORTG=0x00;
		//PORTG=0x01;

		//PORTB|=1<<PD3;
		//_delay_us(5);
        //TODO:: Please write your application code 
    }
}