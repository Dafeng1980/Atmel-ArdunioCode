/*
 * GccApplication_BUZZ.c
 *
 * Created: 2012/6/9 14:21:56
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/sfr_defs.h>
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
int main(void)
{
	unsigned int i;
	unsigned char j;
	unsigned char k;
	io_init();
	
    while(1)
    {
		if (PIND!=0xFF)   //*判断是否有按键按下
		{
			DelayMs(30); //*防抖动延时
			k=~PIND;
			k=k+50;
		if(PIND!=0xFF)  //*再次判断（两次判断可有效滤除抖动）
		{
			
		for (i=0; i<700; i++)
		{
			j=0;
			while(j<k)
			{
				j++;
				}
				PORTB^=_BV(PB1);
		}			
			PORTB|=_BV(PB1);
			while(PIND!=0xFF);
			
		}
		
        //TODO:: Please write your application code 
    }
		
	}	
}