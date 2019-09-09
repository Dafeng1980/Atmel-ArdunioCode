/*
 * Gcc_M41T00.c
 *
 * Created: 2014/6/21 21:41:55
 *  Author: Dafeng
 */ 
/*****************************************************************************
//  File Name    : Gcc_M41T00.c
//  Version      : 1.0
//  Description  : M41T00 RTC AVR Microcontroller Clock
//  Author       : Dafeng
//  Target       : Atmel Mega168PA Board
//  Compiler     : AVR_8_bit_GNU_Toolchain_3.4.4_1229 AVR-GCC 4.8.1
//  IDE          : Atmel AVR Studio 6.2
//  Programmer   : Atmel AVRISP mkII
//               : Atmel AVR Studio 6.2
//  Last Updated : 21 June 2014
*****************************************************************************/


#include <avr/io.h>
#define F_CPU 7372800UL
#include <util/delay.h>
#include <compat/twi.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BAUD_RATE 38400
#define MAX_TRIES 50
#define MT41T100_ID    0xD0        // I2C M41T00 Device Identifier
#define MT41T100_ADDR  0x00        // I2C M41T00 Device Address
#define I2C_START 0
#define I2C_DATA 1
#define I2C_DATA_ACK 2
#define I2C_STOP 3
#define ACK 1
#define NACK 0

#define TMP112_ID    0x90
#define TMP112_ADDR  0x02

#define SET_SHC PORTD|=1<<PD3      //PD3 is 595 RCLK (Storage register clock)
#define CLR_SHC PORTD&=~(1<<PD3)
#define SPI_595_T(x)   {SPDR=x;while(!(SPSR&(1<<SPIF)));}
	


// MT41T100 Register Address
// Second: M41T00_addr[0]
// Minute: M41T00_addr[1]
// Hour  : M41T00_addr[2]
// Day   : M41T00_addr[3]
// Date  : M41T00_addr[4]
// Month : M41T00_addr[5]
// Year  : M41T00_addr[6]
 unsigned char M41T00_addr[9];
 //char sdigit[3]={'0','0','\0'};
 unsigned char g_aDisplayBuf[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	 char h;

void uart_init(void)
{
	UBRR0H = (((F_CPU/BAUD_RATE)/16)-1)>>8;	// set baud rate
	UBRR0L = (((F_CPU/BAUD_RATE)/16)-1);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); 		 // enable Rx & Tx
	UCSR0C=  (1<<UCSZ01)|(1<<UCSZ00);  	        // config USART; 8N1
}
int uart_putch(char ch,FILE *stream)
{
	if (ch == '\n')
	uart_putch('\r', stream);
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0=ch;
	return 0;
}
int uart_getch(FILE *stream)
{
	unsigned char ch;
	while (!(UCSR0A & (1<<RXC0)));
	ch=UDR0;

	// Echo the Output Back to terminal //
	uart_putch(ch,stream);

	return ch;
}
void ansi_cl(void)
{
	// ANSI clear screen: cl=\E[H\E[J
	putchar(27);
	putchar('[');
	putchar('H');
	putchar(27);
	putchar('[');
	putchar('J');
}
void ansi_me(void)
{
	// ANSI turn off all attribute: me=\E[0m
	putchar(27);
	putchar('[');
	putchar('0');
	putchar('m');
}
void ansi_cm(unsigned char row,unsigned char col)
{
	// ANSI cursor movement: cl=\E%row;%colH
	putchar(27);
	putchar('[');
	printf("%d",row);
	putchar(';');
	printf("%d",col);
	putchar('H');
}
/* START I2C Routine */
unsigned char i2c_transmit(unsigned char type) {
	switch(type) {
		case I2C_START:    // Send Start Condition
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
		break;
		case I2C_DATA:     // Send Data with No-Acknowledge
		TWCR = (1 << TWINT) | (1 << TWEN);
		break;
		case I2C_DATA_ACK: // Send Data with Acknowledge
		TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
		break;
		case I2C_STOP:     // Send Stop Condition
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return 0;
	}
	// Wait for TWINT flag set on Register TWCR
	while (!(TWCR & (1 << TWINT)));
	// Return TWI Status Register, mask the prescaller bits (TWPS1,TWPS0)
	return (TWSR & 0xF8);
}
char i2c_start(unsigned int dev_id, unsigned int dev_addr, unsigned char rw_type)
{
	unsigned char n = 0;
	unsigned char twi_status;
	char r_val = -1;
	i2c_retry:
	if (n++ >= MAX_TRIES) return r_val;
	// Transmit Start Condition
	twi_status=i2c_transmit(I2C_START);

	// Check the TWI Status
	if (twi_status == TW_MT_ARB_LOST) goto i2c_retry;
	if ((twi_status != TW_START) && (twi_status != TW_REP_START)) goto i2c_quit;
	// Send slave address (SLA_W)
	TWDR = (dev_id & 0xF0) | (dev_addr & 0x07) | rw_type;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if ((twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MT_ARB_LOST)) goto i2c_retry;
	if (twi_status != TW_MT_SLA_ACK) goto i2c_quit;
	r_val=0;
	i2c_quit:
	return r_val;
}
void i2c_stop(void)
{
	 unsigned char twi_status;
	// Transmit I2C Data
	twi_status = i2c_transmit(I2C_STOP);
	//return twi_status;
}
char i2c_write(char data)
{
	unsigned char twi_status;
	char r_val = -1;
	// Send the Data to I2C Bus
	TWDR = data;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if (twi_status != TW_MT_DATA_ACK) goto i2c_quit;
	r_val=0;
	i2c_quit:
	return r_val;
}
char i2c_read(char *data,char ack_type)
{
	unsigned char twi_status;
	char r_val = -1;

	if (ack_type) {
		// Read I2C Data and Send Acknowledge
		twi_status=i2c_transmit(I2C_DATA_ACK);
		if (twi_status != TW_MR_DATA_ACK) goto i2c_quit;
		} else {
		// Read I2C Data and Send No Acknowledge
		twi_status=i2c_transmit(I2C_DATA);
		if (twi_status != TW_MR_DATA_NACK) goto i2c_quit;
	}
	// Get the Data
	*data=TWDR;
	r_val=0;
	i2c_quit:
	return r_val;
}
// Convert Decimal to Binary Coded Decimal (BCD)
char dec2bcd(char num)
{
	return ((num/10 * 16) + (num % 10));
}
// Convert Binary Coded Decimal (BCD) to Decimal

/*
char bcd2dec(char num)
{
	return ((num/16 * 10) + (num % 16));
}
***/
void Read_M41T00(void)
{
	char data;
	// First we initial the pointer register to address 0x00
	// Start the I2C Write Transmission
	i2c_start(MT41T100_ID,MT41T100_ADDR,TW_WRITE);
	// Start from Address 0x00
	i2c_write(0x00);
	// Stop I2C Transmission
	i2c_stop();

	// Start the I2C Read Transmission
	i2c_start(MT41T100_ID,MT41T100_ADDR,TW_READ);
	// Read the Second Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	M41T00_addr[0]=data & 0x7F;
	// Read the Minute Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	M41T00_addr[1]=data & 0x7F;
	// Read the Hour Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	/**if ((data & 0x40) == 0x40) {
		hour_mode = HOUR_12;
		ampm_mode=(data & 0x20) >> 5;   // ampm_mode: 0-AM, 1-PM
		ds1307_addr[2]=bcd2dec(data & 0x1F);
		} else {
		hour_mode = HOUR_24;
		ampm_mode=0;**/
		M41T00_addr[2]=data & 0x3F;
	
	// Read the Day of Week Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	M41T00_addr[3]=data & 0x07;
	// Read the Day of Month Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	M41T00_addr[4]=data & 0x3F;
	// Read the Month Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	M41T00_addr[5]=data & 0x1F;
	// Read the Year Register, Send Master No Acknowledge
	i2c_read(&data,NACK);
	M41T00_addr[6]=data;
	// Stop I2C Transmission
	i2c_stop();
}
void Read_TMP112(void)
{
	char data;
	i2c_start(TMP112_ID,TMP112_ADDR,TW_WRITE);
	i2c_write(0x00);
	i2c_stop();
	i2c_start(TMP112_ID,TMP112_ADDR,TW_READ);
	i2c_read(&data,ACK);
	M41T00_addr[7]=dec2bcd(data);
	i2c_read(&data,ACK);
	M41T00_addr[8]=(data>>4);
	i2c_stop();
	
	
	
}
void Write_M41T00(void)
{
	unsigned char i;
	// Make sure we enable the Oscillator control bit CH=0 on Register 0x00
	M41T00_addr[0]=M41T00_addr[0] & 0x7F;
	// Start the I2C Write Transmission
	i2c_start(MT41T100_ID,MT41T100_ADDR,TW_WRITE);
	// Start from Address 0x00
	i2c_write(0x00);
	// Write the data to the MT41T100 address start at 0x00
	// MT41T100 automatically will increase the Address.
	for (i=0; i<7; i++) {
			i2c_write(dec2bcd(M41T00_addr[i]));
		}
	
	// Stop I2C Transmission
	i2c_stop();
}
// Implementing integer value from 00 to 99

/*char *num2str(char number)
{
	unsigned char digit;

	digit = '0';                       // Start with ASCII '0'
	while(number >= 10)                // Keep Looping for larger than 10
	{
		digit++;                         // Increase ASCII character
		number -= 10;                    // Subtract number with 10
	}

	sdigit[0]='0';                     // Default first Digit to '0'
	if (digit != '0')
	sdigit[0]=digit;                 // Put the Second digit
	sdigit[1]='0' + number;
	return sdigit;
}*/
char getnumber(unsigned char min, unsigned char max)
{
	int inumber;
	scanf("%d",&inumber);
	if (inumber < min || inumber > max) {
		printf("\n\nInvalid [%d to %d]!",min,max);
		_delay_ms(500);
		return -1;
	}
	return inumber;
}


void displayNumber(unsigned char num[], unsigned char i)
{
	if (i==2)
	{
		CLR_SHC;
		SPI_595_T(0x10);
		SPI_595_T(g_aDisplayBuf[num[7]>>4]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x20);
		SPI_595_T(g_aDisplayBuf[(num[7]&0x0F)]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x80);
		SPI_595_T(g_aDisplayBuf[num[8]*10/16]);
		SET_SHC;
	}
	if(i==0)
	{
		CLR_SHC;
		SPI_595_T(0x80);
		SPI_595_T(g_aDisplayBuf[num[3]&0x0F]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x40);
		SPI_595_T(g_aDisplayBuf[num[3]>>4]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x10);
		SPI_595_T(g_aDisplayBuf[num[4]&0x0F]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x08);
		SPI_595_T(g_aDisplayBuf[num[4]>>4]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x02);
		SPI_595_T(g_aDisplayBuf[num[5]&0x0F]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x01);
		SPI_595_T(g_aDisplayBuf[num[5]>>4]);
		SET_SHC;
	}
	if(i==1)
	{
		CLR_SHC;
		SPI_595_T(0x80);
		SPI_595_T(g_aDisplayBuf[num[0]&0x0F]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x40);
		SPI_595_T(g_aDisplayBuf[num[0]>>4]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x10);
		SPI_595_T(g_aDisplayBuf[num[1]&0x0F]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x08);
		SPI_595_T(g_aDisplayBuf[num[1]>>4]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x02);
		SPI_595_T(g_aDisplayBuf[num[2]&0x0F]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x01);
		SPI_595_T(g_aDisplayBuf[num[2]>>4]);
		SET_SHC;
	}
}
ISR(TIMER0_OVF_vect)
{
	
	//static unsigned char tenms=1;
	//int iTemp;
	//displayNumber(M41T00_addr,0);
	//tenms++;                  // Read DS1307 every 100 x 10ms = 1 sec
	//if (tenms >= 2) {
		//cli();                                // Disable Interupt
		// Read DS1307
		if(bit_is_clear(PINC,PC3))
		{
			_delay_us(500);
			
			if(bit_is_clear(PINC,PC3))
			   h++;
			   if(h>2) h=0;
		}
		   
	
		Read_M41T00();
		Read_TMP112();
		//sei();
	//}
		
		
}
ISR(TIMER2_OVF_vect)
{
	/**char h=1;
		if(bit_is_clear(PINC,PC3))
		{
			_delay_ms(5);
			
			if(bit_is_clear(PINC,PC3))
			h=0;
			
			
			
		}**/
	displayNumber(M41T00_addr,h);
}
	

void Port_init(void)
{
	PORTC=0xff;		DDRC=0x08;
	PORTD=0x04;		DDRD=0x0C;
}
void Spi_init(void)
{
	DDRB|=_BV(PB5) | _BV(PB3) | _BV(PB2);
	SPCR=_BV(SPE) | _BV(MSTR) | _BV(SPR0);
	SPSR=0x00;
}
// Assign I/O stream to UART
FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);

int main(void)
{
	 char mode,ichoice;
	stdout = stdin = &uart_str;
	Port_init();
	Spi_init();
	uart_init();
	
	  // Initial ATMega168 TWI/I2C Peripheral
	  TWSR = 0x00;   // Select Prescaler of 1
	  // SCL frequency = 11059200 / (16 + 2 * 48 * 1) = 98.743 khz
	  TWBR = 0x20;   // 48 Decimal
	    // Initial ATMega168 Timer/Counter0 Peripheral
	  TCCR0A=0x00; TCCR2A=0x00;                 // Normal Timer0 Operation
	  TCCR0B=(1<<CS02)|(1<<CS00);
	  TCCR2B=(1<<CS21);                 // Use maximum prescaller: Clk/1024
	  TCNT0=0x20;
	  TCNT2=0x00;                   // Start counter from 0x94, overflow at 10 mSec
	  TIMSK0=(1<<TOIE0);
	  TIMSK2=(1<<TOIE0);           // Enable Counter Overflow Interrupt
	  sei();  
	  mode = 0; 
	  h = 0;                     // Enable Interrupt
	
    while(1)
    {
		
		//displayNumber(M41T00_addr,0);
		
		if (bit_is_clear(PIND,PD2))
		{
			_delay_ms(5);
			
			if(bit_is_clear(PIND,PD2))
			{
				cli();
				mode=1;
			}
		}
		if (mode)
	{
		ansi_me();
		ansi_cl();                            // Clear Screen
		ansi_cm(1,1);
		printf("AVR Mega168PA M41T00 RTC Setup");
		ansi_cm(3,1);
		printf("1. Time: %02d:%02d:%02d\n",M41T00_addr[2],M41T00_addr[1],M41T00_addr[0]);
		//printf("2. Mode 24/12: %d, AM/PM: %d\n",hour_mode,ampm_mode);
		printf("2. Date: %02d-%02d-20%02d, Week Day: %d\n",M41T00_addr[4],M41T00_addr[5],M41T00_addr[6],M41T00_addr[3]);
		printf("3. Save and Exit\n");
		printf("4. Exit\n");
		printf("\nEnter Choice: ");
		if ((ichoice=getnumber(1,5)) < 0) continue;
		switch (ichoice) {
			case 1:  // DS1307 Time Setup
			printf("\n\nHour [0-24]: ");
			if ((M41T00_addr[2]=getnumber(0,24)) < 0) continue;
			printf("\nMinute [0-59]: ");
			if ((M41T00_addr[1]=getnumber(0,59)) < 0) continue;
			printf("\nSecond [0-59]: ");
			if ((M41T00_addr[0]=getnumber(0,59)) < 0) continue;
			break;
			/*case 2:  // DS1307 Hour Mode Setup
			printf("\n\nMode 0> 24, 1> 12: ");
			if ((hour_mode=getnumber(0,1)) < 0) continue;
			printf("\nAM/PM 0> AM, 1> PM: ");
			if ((ampm_mode=getnumber(0,1)) < 0) continue;
			break;
			*/
			case 2:  // DS1307 Date Setup
			printf("\n\nWeekDay [1-7]: ");
			if ((M41T00_addr[3]=getnumber(1,7)) < 0) continue;
			printf("\nDate [1-31]: ");
			if ((M41T00_addr[4]=getnumber(1,31)) < 0) continue;			  ;
			printf("\nMonth [1-12]: ");
			if ((M41T00_addr[5]=getnumber(1,12)) < 0) continue;
			printf("\nYear [0-99]: ");
			if ((M41T00_addr[6]=getnumber(0,99)) < 0) continue;
			break;
			case 3:  // Save to DS1307 Register and Exit Setup
			Write_M41T00();
			case 4:  // Exit Setup
			mode = 0;
			ansi_cl();
			// Illuminating the LCD
			/*for (icount=0;icount < 255;icount++) {
				OCR2A=icount;
				_delay_ms(3);
				*/
			
			
			TCNT0=0x94;
			sei();   // Enable Interrupt
			break;
		
	}
	}
	//_delay_ms(1);
	}
        //TODO:: Please write your application cod
	return 0;
}