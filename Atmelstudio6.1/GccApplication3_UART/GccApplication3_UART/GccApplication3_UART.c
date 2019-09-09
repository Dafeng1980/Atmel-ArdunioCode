/*
 * GccApplication3_UART.c
 *
 * Created: 2014/11/25 14:05:10
 *  Author: PE_NPI
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>
#include <avr/interrupt.h>
#define MAX_TRIES   50
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

//#define SET_SHC PORTD|=1<<PD3      //PD3 is 595 RCLK (Storage register clock)
//#define CLR_SHC PORTD&=~(1<<PD3)
//#define SPI_595_T(x)   {SPDR=x;while(!(SPSR&(1<<SPIF)));}
	


// MT41T100 Register Address
// Second: M41T00_addr[0]
// Minute: M41T00_addr[1]
// Hour  : M41T00_addr[2]
// Day   : M41T00_addr[3]
// Date  : M41T00_addr[4]
// Month : M41T00_addr[5]
// Year  : M41T00_addr[6]
 unsigned char M41T00_addr[9];
 char sdigit[3]={'0','0','\0'};
 //unsigned char g_aDisplayBuf[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	 char h;

/* void uart_init(void)
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
} */

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


/*char bcd2dec(char num)
{
	return ((num/16 * 10) + (num % 16));
}
*/
/* void Read_M41T00(void)
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
	if ((data & 0x40) == 0x40) {
		hour_mode = HOUR_12;
		ampm_mode=(data & 0x20) >> 5;   // ampm_mode: 0-AM, 1-PM
		ds1307_addr[2]=bcd2dec(data & 0x1F);
		} else {
		hour_mode = HOUR_24;
		ampm_mode=0;
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
*/
void Read_TMP112(void)
{
	char data;
	i2c_start(TMP112_ID,TMP112_ADDR,TW_WRITE);
	i2c_write(0x00);
	i2c_stop();
	i2c_start(TMP112_ID,TMP112_ADDR,TW_READ);
	i2c_read(&data,ACK);
	M41T00_addr[7]=data;
	i2c_read(&data,ACK);        
	M41T00_addr[8]=(data>>4);
	i2c_stop();
	
	
	
}
/*void Write_M41T00(void)
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
// Implementing integer value from 00 to 99  **/

char *num2str(char number)
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
}
/**
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
void DelayMs(unsigned int t)
{
	unsigned int i;
	for(i=0;i<t;i++)
	_delay_loop_2(250*7);
}*/


int main(void)
{
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	UCSR0C=(1<<UCSZ00)|(1<<UCSZ01);
	UBRR0L=51;
	TWSR = 0x00;
	TWBR = 0x24;
    while(1)
    {
		Read_TMP112();
		//unsigned char i;
		//for (i=10; i<128; i++)
		num2str(M41T00_addr[7]);
		
			loop_until_bit_is_set(UCSR0A,UDRE0);
		UDR0=0x30; 
		UDR0=0x31;
		//
		
		_delay_ms(250);
		     loop_until_bit_is_set(UCSR0A,UDRE0);
		UDR0=0x0A;
		    // loop_until_bit_is_set(UCSR0A,UDRE0);
		UDR0=0x0D;
		_delay_ms(250);
		//if(i==128) i=0;
		
        //TODO:: Please write your application code 
    }
}