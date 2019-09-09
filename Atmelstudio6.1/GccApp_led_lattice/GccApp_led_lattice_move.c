/*
 * GccApp_led_lattice.c
 *
 * Created: 2012/6/10 13:39:56
 *  Author: fe
 */ 


#include <avr/io.h>

#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>
#include "Debug/hanzi.h"
#define SET_SHC PORTD|=_BV(PD3)  //74HC595 shift register set(PD3 Port)
#define CLR_SHC PORTD&=~_BV(PD3) //74HC595 shift register clear 
#define SET_AB PORTB|=_BV(PB0) //Row output 74HC164; (PB0 Port)
#define CLR_AB PORTB&=~_BV(PB0)
#define SET_SCL PORTB|=_BV(PB1) //Row clock 74HC164; (PB1 Port)
#define CLR_SCL PORTB&=~_BV(PB1)
#define WORDS 32 // Display Chinese word count.


  unsigned char c=0;
  unsigned char buffer_wd[32];
void Port_init(void)
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
void SPI_595_OUT(unsigned char i)
{
	CLR_SCL;
	CLR_SHC;
	SPI_T(~buffer_wd[i*2+1]);
	SPI_T(~buffer_wd[i*2]);
	SET_SCL;
	SET_SHC;
	SET_AB;
}
void Spi_init(void)
{
	DDRB|=_BV(PB5)|_BV(PB3);
	SPCR=0x71;
	SPSR=0x00;
}
void Timer0_init(void)
{
	TCCR0=0x00;
	TCNT0=0x83;
	TCCR0=0x02;
}
SIGNAL(TIMER0_OVF_vect)
{
	static unsigned m=0;
	TCNT0=0x80;
	SPI_595_OUT(m);
	m++;
	if(m>15)
	{
		m=0;
		CLR_AB;
	}
}
void Timer1_init(void)
{
	TCCR1B=0x00;
	TCNT1H=0xC2;
	TCNT1L=0xF7;
	TCCR1A=0x00;
	TCCR1B=0x03;
}
SIGNAL(TIMER1_OVF_vect)
{
	static unsigned n=0;
	static unsigned k=0;
	static unsigned char tmp[16];
	unsigned char ia=0;
	unsigned char i=0;
	unsigned char j=0;
	TCNT1H=0xF5;
	TCNT1L=0xF7;
	if(n==0){
			j=c%2;
			for(i=0;i<16;i++)
			{
			tmp[i]=pgm_read_byte(&hanzi[k][i*2+j]);
			}
			if(j)
			k++;
	}			
		
		for(i=0;i<16;i++)
		{
			buffer_wd[ia]<<=1;
			if(buffer_wd[ia+1]&0x80){
			buffer_wd[ia]++;
			}			
			ia++;			
			buffer_wd[ia]<<=1;
			if(tmp[i]&0x80){
			buffer_wd[ia]++;
			}			
			ia++;
			tmp[i]<<=1;
			}
			n++;
			if(n==8)
			{
				n=0;
				c--;
				if(c==0){
				c=WORDS*2;
				k=0;
				}
			}
		}
void Init_devices(void)
{
	unsigned char i;
	cli();
	Port_init();
	Timer0_init();
	Timer1_init();
	Spi_init();
	MCUCR=0x00;
	GICR=0x00;
	TIMSK=0x05;
	for(i=0;i<32;i++)
	buffer_wd[i]=pgm_read_byte(&hanzi[25][i]);
	c=WORDS*2;
	sei();
	
}

int main(void)
{
	Init_devices();
    while(1)
    {
       ; //TODO:: Please write your application code 
    }
}
