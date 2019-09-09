/*
 * eighitA_7seg_digital_application3.c
 *
 * Created: 2012/10/18 19:41:59
 *  Author: fe
 */ 


#include <avr/io.h>

#define SET_SHC PORTD|=1<<PD3      //PD3 is 595 RCLK (Storage register clock)
#define CLR_SHC PORTD&=~(1<<PD3)
#define SPI_595_T(x)   {SPDR=x;while(!(SPSR&(1<<SPIF)));}
#define uchar unsigned char
#define uint unsigned int

uchar g_aDisplayBuf[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07, \
0x7F,0x6F,0x88,0x83,0xc6,0xa1,0x86,0x8e};



void Spi_init(void)
{
	DDRB|=_BV(PB5) | _BV(PB3) | _BV(PB2);
		SPCR=_BV(SPE) | _BV(MSTR) | _BV(SPR0);
			SPSR=0x00;
}


void displayNum(uchar *lp , uchar c)
{
		uchar i;uchar j;
			for(i=0;i<c;i++)
	{
		j=7-i;
		CLR_SHC;
			SPI_595_T(1<<j);
			SPI_595_T(*(lp+i));
					SET_SHC;
		
		
		
	}
	
}






int main(void)
{		
		uchar 
		PORTD=0x00;		DDRD=0x08;
		Spi_init();
    while(1)
    {
		
		displayNum(g_aDisplayBuf,5);
		
		     
    }
}