/*
 * Gcc_168PA_ADC.c
 *ADC_test ext AREF_2.5V  by ADC0 conversion
 * Created: 2012/10/23 21:45:34
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#define F_CPU  7372800
#define delayUs(x)  _delay_us(x)
#define delayMs(x)  _delay_ms(x)
#define SET_SHC PORTD|=1<<PD3      //PD3 is 595 RCLK (Storage register clock)
#define CLR_SHC PORTD&=~(1<<PD3)
#define SPI_595_T(x)   {SPDR=x;while(!(SPSR&(1<<SPIF)));}
#define uchar unsigned char
#define uint unsigned int
uchar buff[2];
uchar g_aDisplayBuf[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};


uchar getAdc(void){
	uchar i;
	ADMUX=_BV(ADLAR);  //Ref =Vref. left shift
	ADCSRA=_BV(ADEN) | _BV(ADSC) | _BV(ADPS2);
	while(!(ADCSRA & _BV(ADIF)));
	i = ADCH;
	ADCSRA&=~(_BV(ADIF));
	ADCSRA&=~(_BV(ADEN));
	return i;
	
}
void disply(uchar ad_i){
	uchar i;
	uint j;
	j=ad_i*250;
	ad_i=j/256;
	for (i=0;i<3;i++)
	{	
		buff[i]=ad_i%10;
		ad_i=ad_i/10;
		
	}
	
	
}void Spi_init(void)
{
	DDRB|=_BV(PB5) | _BV(PB3) | _BV(PB2);
		SPCR=_BV(SPE) | _BV(MSTR) | _BV(SPR0);
			SPSR=0x00;
}

void displayNumber(uchar num[])

{
	CLR_SHC;
	SPI_595_T(0x80);
	SPI_595_T(0x3E);
	SET_SHC;
	CLR_SHC;
	SPI_595_T(0x40);
	SPI_595_T(g_aDisplayBuf[num[0]]);
	SET_SHC;
	CLR_SHC;
	SPI_595_T(0x20);
	SPI_595_T(g_aDisplayBuf[num[1]]);
	SET_SHC;
	CLR_SHC;
	SPI_595_T(0x10);
	SPI_595_T(g_aDisplayBuf[num[2]]);
	SET_SHC;
	
}

int main(void)
{
	uchar i;
	PORTD=0x04;		DDRD=0x0C;
	Spi_init();
	i=getAdc();
	disply(i);
    while(1)
    {
       //delayMs(5);
	   i=getAdc();
	   disply(i);
	   displayNumber(buff);
	     
    }
}