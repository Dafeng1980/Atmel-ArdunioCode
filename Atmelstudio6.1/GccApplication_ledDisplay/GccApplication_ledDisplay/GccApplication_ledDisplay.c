/*
 * GccApplication_ledDisplay.c
 *
 * Created: 2012/7/12 18:35:48
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/delay.h>
//#include <avr/pgmspace.h>
#define SET_SHC PORTD|=1<<PD3
#define CLR_SHC PORTD&=~(1<<PD3)
#define uchar unsigned char
#define uint unsigned int

  uchar g_aDisplayBuf[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, \
0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void DelayMs(uint t)
{
	uint i;
	for(i=0;i<t;i++)
	_delay_loop_2(250*3);
}
void SPI_595T(uchar a)
{
	SPDR=a;
	while(!(SPSR&(1<<SPIF)))
	{
		;
	}
}
void displayNumber(uchar num,uchar hex)

{
	uchar buf[2];
	uchar temp;
	CLR_SHC;
	if(hex)
	{
		buf[0]=g_aDisplayBuf[num>>4];
		buf[1]=g_aDisplayBuf[num&0x0f];
	}
	else
	{
		buf[1]=g_aDisplayBuf[num%10];
		temp=num%100;
		buf[0]=g_aDisplayBuf[temp/10];
		temp=num/100;
		if(temp>0)
		buf[1]|=0x80; //第一个数码管小数点表示百位1
		if(temp>1)
		buf[0]|=0x80;//两个数码管小数点表示百位2
	}
	SPI_595T(buf[1]);
	SPI_595T(buf[0]);
	SET_SHC;
	
	}
	void Port_init(void)
	{
		PORTB=0x00;
		DDRB=0x3F;
		PORTC=0x00;
		DDRC=0x00;
		PORTD=0x00;
		DDRD=0x08;
	}
	void Spi_init(void)
	{
		DDRB|=_BV(PB5)|_BV(PB3);
		SPCR=0x51;
		SPSR=0x00;
	}
	

int main(void)

{
	uchar i=0;
   Port_init();
   Spi_init();
   
    while(1)
    {
       displayNumber(i++,1);
       DelayMs(1000);
	    //TODO:: Please write your application code 
    }
}