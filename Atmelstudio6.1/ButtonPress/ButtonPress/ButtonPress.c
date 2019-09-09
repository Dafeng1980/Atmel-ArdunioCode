/*
 * ButtonPress.c
 *
 * Created: 2016/1/12 13:20:16
 *  Author: PE_NPI
 */ 

#define numberOfButtons 1
#include <avr/io.h>
#include "ButtonPress.h"

int main(void)
{
	DDRD = 0b00101000;
	PORTD = (1 << PIND2) | (1 << PIND5);
    while(1)
    {
		if (ButtonPressed(0, PIND, 2, 500))
		{
			PORTD ^= ((1 << PIND3) | (1 << PIND5)); 
			//PORTD ^= (1 << PIND5);
		}
		
		
        //TODO:: Please write your application code 
    }
}