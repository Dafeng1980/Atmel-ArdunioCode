/*
 * AVRGCC1.c
 *
 * Created: 2012/3/2 23:32:29
#include <avr/io.h>
#include <iom8.h>

 *  Author: fe
 */ 

#include <avr/io.h>


void delay_ms(void) {
	unsigned char b;
	unsigned int a;
	unsigned char c;
	for (a=1;a<5550;a++) 
	{
		for (b=1;b<255;b++)
		
		c=c++;
		
		
	}
	
}
void led_out_a(unsigned char led_a) {
	PORTB = led_a & 0x3F;
	if (led_a & 0x80)
	{
        PORTC |= (1 << PC3);
		
	} 
	else
	{ 
		PORTC &= ~(1 << PC3);
	}
	if (led_a & 0x40)
	{
        PORTC |= (1 << PC2);
	} 
	else
	{
		PORTC &= (1 << PC2);
	}
}
void io_init(void) {
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0x0C;
	PORTC = 0xF3;
	DDRD = 0x00;
	PORTD = 0xFF;
}	
	
	

void main(void)
{
	unsigned char j = 0;
	io_init();
	
    while (1)
    {
		j=0x01;
		while (j!=0)
		 {
			led_out_a(j);
			delay_ms();
			j=j<<1;
		}
		j=0x80;
		while (j!=0)
		{
			led_out_a(j);
			delay_ms();
			j=j>>1;
		}
		
}
}	
	