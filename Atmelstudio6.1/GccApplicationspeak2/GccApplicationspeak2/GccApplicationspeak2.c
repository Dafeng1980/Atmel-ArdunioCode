/*
 * GccApplicationspeak2.c
 *
 * Created: 2014/12/24 18:40:07
 *  Author: PE_NPI
 */ 


//#define F_CPU 7234000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#define INT8U unsigned char
#define INT16U unsigned int
#define SPK() (PORTB^=_BV(PB1))
//INT8U t;

void Alarm(INT8U t)
{
	INT8U i;
	for(i=0;i<200;i++)
	{
		SPK();
		_delay_us(t);
	}
}

int main()
{    
	DDRB=0xFF;
	PORTB=0xFF;
    while(1)
    {
		Alarm(30);
		Alarm(200);
        //TODO:: Please write your application code 
// 			INT8U i,j;
// 			for(i=0;i<200;i++)
// 			{
// 				SPK();
// 				_delay_us(50);
// 			}
// 			  
// 				for(j=0;j<200;j++)
// 				{
// 					SPK();
// 					_delay_us(200);
// 				}
		
    }
}