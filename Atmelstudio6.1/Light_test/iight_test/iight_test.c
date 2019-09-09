/*
 * iight_test.c
 *
 * Created: 2012/7/20 23:30:13
 * Atmage168PA @ 7.3728Mhz
 *PB0 Output / PC3  KEY
 *
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#define KEY_IN  (!(PINC&=(1<<PORTC3)))

unsigned char key;

void delayMS(unsigned int ms)
{
	unsigned int i;
	for(i=0;i<ms;i++)
	_delay_loop_2(250*7);
}
int main(void)
{
	unsigned char i;
	
	
	PORTB=0x00;
	DDRB=0x01;
	DDRC=0x00;
	PORTC=0x08;
	
    while(1)
    {
        while(1)
		{

		if(KEY_IN)
		
			delayMS(5);
				if(KEY_IN)
					{
						key++;
							break;
		}
		}
						
		i=key%2;
		switch(i)
		{
			case 0:PORTB=0x00;break;
			case 1:PORTB=0x01;break;
	    }
	
}
	}