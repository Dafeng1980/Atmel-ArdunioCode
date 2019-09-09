/*
 * Gcc_ButtonPress.c
 *
 * Created: 2016/1/12 13:00:32
 *  Author: PE_NPI
 */ 

#define numberOfButtons 1
#include <avr/io.h>
#include "ButtonPress.h"

int main(void)
{
	DDRD = 0b00001000;
	PORTD = (1 << PIND2);
	
    while(1)
    {
       if(ButtonPressed(0, PIND, 0, 200))
	   PORTD ^= (1 << PIND2);
	    //TODO:: Please write your application code 
    }
}