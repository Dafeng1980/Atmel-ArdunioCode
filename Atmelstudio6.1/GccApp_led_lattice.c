/*
 * GccApp_led_lattice.c
 *
 * Created: 2012/6/10 13:39:56
 *  Author: fe
 */ 


#include <avr/io.h>
//#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
//#include <avr/signal.h>
#include <avr/pgmspace.h>
#include "Debug/hanzi.h"
#define SET_SHC PORTD|=_BV(PD3)  //74HC595 shift register set(PD3 Port)
#define CLR_SHC PORTD&=~_BV(PD3) //74HC595 shift register clear 
#define SET_AB PORTB|=_BV(PB0) //Row output 74HC164; (PB0 Port)
#define CLR_AB PORTB&=~_BV(PB0)
#define SET_SCL PORTB|=_BV(PB1) //Row clock 74HC164; (PB1 Port)
#define CLR_SCL PORTB&=~_BV(PB1)
#define hz_s 31 // Display Chinese word count.

 static unsigned char i=0;
 static unsigned char m=0;

void port_init(void)
{
	PORTB=0x00;
	DDRB=0x3F;
	PORTC=0x00;
	DDRC=0x00;
	PORTD=0x00;
	DDRD=0x08;
}
void SPI_T(unsigned char a)
{	
	SPDR=a;
while(!(SPSR&_BV(SPIF)))
{
	;
}
}
void SPI_595_OUT(unsigned char a)
{
	CLR_SCL;
	CLR_SHC;
	SPI_T(0xff-pgm_read_byte(&hanzi[i][a*2+1]));
	SPI_T(0xff-pgm_read_byte(&hanzi[i][a*2]));
	SET_SCL;
	SET_SHC;
	SET_AB;
}
void spi_init(void)
{
	DDRB|=_BV(PB5)|_BV(PB3);
	SPCR=0x71;
	SPSR=0x00;
}
void timer0_init(void)
{
	TCCR0=0x00;
	TCNT0=0x83;
	TCCR0=0x02;
}
SIGNAL(TIMER0_OVF_vect)
{
	TCNT0=0x83;
	SPI_595_OUT(m);
	m++;
	if(m>15)
	{
		m=0;
		CLR_AB;
	}
}
void timer1_init(void)
{
	TCCR1B=0x00;
	TCNT1H=0xC2;
	TCNT1L=0xF7;
	TCCR1A=0x00;
	TCCR1B=0x03;
}
SIGNAL(TIMER1_OVF_vect)
{
	TCNT1H=0xC2;
	TCNT1L=0xF7;
	i++;
	if(i>hz_s)
	{
		i=0;
	}
}
void init_devices(void)
{
	cli();
	port_init();
	timer0_init();
	timer1_init();
	spi_init();
	MCUCR=0x00;
	GICR=0x00;
	TIMSK=0x05;
	sei();
	
}

int main(void)
{
	init_devices();
    while(1)
    {
       ; //TODO:: Please write your application code 
    }
}