/*
 * GccApplication4_URAT.c
 *Author  Dafeng
 * Created: 2014/12/5 10:44:28
 *  Author: PE_NPI
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include "i2c.h"
#include <util/delay.h>
//#include <compat/twi.h>
#define BAUD 9600
#define MYUBPR F_CPU/16/BAUD-1

/*#define MAX_TRIES 50
#define I2C_START 0
#define I2C_DATA 1
#define I2C_DATA_ACK 2
#define I2C_STOP 3
#define ACK 1
#define NACK 0
*/
#define MT41T62_ID   0xD0
#define MT41T62_ADDR  0x00
#define TMP100_ID 0x90
#define TMP100_ADDR 0x0E             //TMP100 is 0x0E;         //TMP112 is 0x02;
#define TMP112_ADDR 0x02


volatile int temperatureSum, temperatureSum1;
unsigned char flag, flag1;
char decimal;

// MT41T62 Register Address
// Second: M41T62_addr[0]
// Minute: M41T62_addr[1]
// Hour  : M41T62_addr[2]
// Day   : M41T62_addr[3]
// Date  : M41T62_addr[4]
// Month : M41T62_addr[5]
// Year  : M41T62_addr[6]
unsigned char M41T62_addr[7];
char sdigit[3]={'0','0','\0'};
	
	/* START I2C Routine */
	/*
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
		TWDR = (dev_id & 0xF0) | (dev_addr & 0x0F) | rw_type;
		// Transmit I2C Data
		twi_status=i2c_transmit(I2C_DATA);
		// Check the TWSR status
		if ((twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MT_ARB_LOST)) goto i2c_retry;
		if (twi_status != TW_MT_SLA_ACK) goto i2c_quit;
		r_val=0;
		i2c_quit:
		return r_val;
	}
	char i2c_stop(void)
	{
		unsigned char twi_status;
		// Transmit I2C Data
		twi_status = i2c_transmit(I2C_STOP);
		return twi_status;
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
	
	*///
	
	
	void Read_TMP100(void)
	{
	char data;
	char msb;
	char lsb;
	i2c_start(TMP100_ID,TMP100_ADDR,TW_WRITE);
	i2c_write(0x00);
	i2c_stop();
	i2c_start(TMP100_ID,TMP100_ADDR,TW_READ);
	i2c_read(&data,ACK);
	msb=data;
	i2c_read(&data,ACK);
	lsb=data;
	i2c_stop();	
	temperatureSum = ((msb<<8) | lsb ) >> 4; //convert the result to binary code whit a 12 bit left-justified format.

	}
	void Read_TMP112(void)
	{
		char data;
		char msb;
		char lsb;
		i2c_start(TMP100_ID,TMP112_ADDR,TW_WRITE);
		i2c_write(0x00);
		i2c_stop();
		i2c_start(TMP100_ID,TMP112_ADDR,TW_READ);
		i2c_read(&data,ACK);
		msb=data;
		i2c_read(&data,ACK);
		lsb=data;
		i2c_stop();
		temperatureSum1 = ((msb<<8) | lsb ) >> 4; //
	}
	void Read_MT41T62(void)
	{
		char data;
		// First we initial the pointer register to address 0x01
		// Start the I2C Write Transmission
		i2c_start(MT41T62_ID,MT41T62_ADDR,TW_WRITE);
		// Start from Address 0x01
		i2c_write(0x01);
		// Stop I2C Transmission
		i2c_stop();

		// Start the I2C Read Transmission
		i2c_start(MT41T62_ID,MT41T62_ADDR,TW_READ);
		// Read the Second Register, Send Master Acknowledge
		i2c_read(&data,ACK);
		M41T62_addr[0]=data & 0x7F;
		// Read the Minute Register, Send Master Acknowledge
		i2c_read(&data,ACK);
		M41T62_addr[1]=data & 0x7F;
		// Read the Hour Register, Send Master Acknowledge
		i2c_read(&data,ACK);
		/* if ((data & 0x40) == 0x40) {
			hour_mode = HOUR_12;
			ampm_mode=(data & 0x20) >> 5;   // ampm_mode: 0-AM, 1-PM
			ds1307_addr[2]=bcd2dec(data & 0x1F);
			} else {
			hour_mode = HOUR_24;
			ampm_mode=0; */
			M41T62_addr[2]=data & 0x3F;
			
			// Read the Day of Week Register, Send Master Acknowledge
			i2c_read(&data,ACK);
			M41T62_addr[3]=data & 0x07;
			// Read the Day of Month Register, Send Master Acknowledge
			i2c_read(&data,ACK);
			M41T62_addr[4]=data & 0x3F;
			// Read the Month Register, Send Master Acknowledge
			i2c_read(&data,ACK);
			M41T62_addr[5]=data & 0x1F;
			// Read the Year Register, Send Master No Acknowledge
			i2c_read(&data,NACK);
			M41T62_addr[6]=data;
			// Stop I2C Transmission
			i2c_stop();
		
	}
	
	// Convert Decimal to Binary Coded Decimal (BCD)
	char dec2bcd(char num)
	{
		return ((num/10 * 16) + (num % 10));
	}
	
	char bcd2dec(char num)
{
	return ((num/16 * 10) + (num % 16));
}

	
	void Write_M41T62(void) 
	{
		unsigned char i;
		// Make sure we enable the Oscillator control bit CH=0 on Register 0x00
		M41T62_addr[0]=M41T62_addr[0] & 0x7F;
		// Start the I2C Write Transmission
		i2c_start(MT41T62_ID,MT41T62_ADDR,TW_WRITE);
		// Start from Address 0x01
		i2c_write(0x01);
		// Write the data to the MT41T62 address start at 0x01
		// MT41T62 automatically will increase the Address.
		for (i=0; i<7; i++) {
			i2c_write(dec2bcd(M41T62_addr[i]));
		}
		
		// Stop I2C Transmission
		i2c_stop();
	}
	
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
	void Init_M41T62_Time(void)
	{
		M41T62_addr[0]=00;
		M41T62_addr[1]=27;
		M41T62_addr[2]=11;
		M41T62_addr[3]=3;
		M41T62_addr[4]=17;
		M41T62_addr[5]=06;
		M41T62_addr[6]=15;
		_delay_ms(10);
		Write_M41T62();
	}
	




void USART_Init (unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	UCSR0C=(1<<UCSZ00)|(1<<UCSZ01);
	
}

int main(void)

{		
		USART_Init(MYUBPR);
			TWSR = 0x00;
			TWBR = 0x24;
			//Init_M41T62_Time();
			

    while(1) //loop forever
    {
		Read_MT41T62();
		_delay_ms(10);
		Read_TMP100();
		_delay_ms(10);
		Read_TMP112();
		_delay_ms(50);
		
		if (temperatureSum & 0x0800) // TMP100 negative temperature convert 
		{
			flag=1;
			temperatureSum = temperatureSum | 0xf000;
			temperatureSum=~temperatureSum+1;
		}
		else 
		flag=0;
		
		decimal=(temperatureSum&0x000F)*0.625;	//convert decimal of data to temperature
		num2str(temperatureSum*0.0625);  //convert digital data to temperature
		
	           loop_until_bit_is_set(UCSR0A,UDRE0);
							UDR0='T';
							//_delay_ms(10);
			   loop_until_bit_is_set(UCSR0A,UDRE0);
							UDR0='M';
							//_delay_ms(10);
			   loop_until_bit_is_set(UCSR0A,UDRE0);
							UDR0='P';
							//_delay_ms(10);
			   loop_until_bit_is_set(UCSR0A,UDRE0);
							UDR0='1';
							//_delay_ms(10);
			   loop_until_bit_is_set(UCSR0A,UDRE0);
							UDR0='0';
							//_delay_ms(10);
			   loop_until_bit_is_set(UCSR0A,UDRE0);
							UDR0='0';
							//_delay_ms(10);
			   loop_until_bit_is_set(UCSR0A,UDRE0);
							UDR0=':';
							//_delay_ms(10);
							
		if(flag)  //negative temperature console output
		{
			loop_until_bit_is_set(UCSR0A,UDRE0);
			UDR0='-';
			//_delay_ms(10);
			loop_until_bit_is_set(UCSR0A,UDRE0);
			UDR0=sdigit[0];
			//_delay_ms(10);
			while (!(UCSR0A&(1<<UDRE0)));
			UDR0=sdigit[1];
			loop_until_bit_is_set(UCSR0A,UDRE0);
			UDR0='.';
			//_delay_ms(10);
			loop_until_bit_is_set(UCSR0A,UDRE0);
			UDR0=decimal+'0';  
			}
			
		else
			{
	   loop_until_bit_is_set(UCSR0A,UDRE0);
	   UDR0=sdigit[0];
	    //_delay_ms(10);
		while (!(UCSR0A&(1<<UDRE0)));
		UDR0=sdigit[1];
			loop_until_bit_is_set(UCSR0A,UDRE0);
				UDR0='.';
				//_delay_ms(10);
			loop_until_bit_is_set(UCSR0A,UDRE0);
				UDR0=decimal+'0';
			}
			
			
			//Read_TMP112();
			//_delay_ms(50);
				if (temperatureSum1 & 0x0800)
				{
					flag1=1;
					temperatureSum1 = temperatureSum1 | 0xf000;
					temperatureSum1 = ~temperatureSum1+1;	
				}
				else
				flag1=0;
				
				num2str(temperatureSum1*0.0625);
				decimal = (temperatureSum1&0x000F)*0.625;
				
						loop_until_bit_is_set(UCSR0A,UDRE0);
									UDR0=' ';
									_delay_ms(50);
						loop_until_bit_is_set(UCSR0A,UDRE0);
									UDR0=' ';
									_delay_ms(50);
						loop_until_bit_is_set(UCSR0A,UDRE0);
											UDR0='T';
						loop_until_bit_is_set(UCSR0A,UDRE0);
											UDR0='M';
						loop_until_bit_is_set(UCSR0A,UDRE0);
											UDR0='P';
						loop_until_bit_is_set(UCSR0A,UDRE0);
											UDR0='1';
						loop_until_bit_is_set(UCSR0A,UDRE0);
											UDR0='1';
						loop_until_bit_is_set(UCSR0A,UDRE0);
											UDR0='2';
				        loop_until_bit_is_set(UCSR0A,UDRE0);
				                          UDR0=':';
				if(flag1)
				{
					loop_until_bit_is_set(UCSR0A,UDRE0);
					UDR0='-';
					loop_until_bit_is_set(UCSR0A,UDRE0);
					UDR0=sdigit[0];
					while (!(UCSR0A&(1<<UDRE0)));					
					UDR0=sdigit[1];
					loop_until_bit_is_set(UCSR0A,UDRE0);
					UDR0='.';
					loop_until_bit_is_set(UCSR0A,UDRE0);
					UDR0=decimal+'0';	
				}
				
				else
				{
					loop_until_bit_is_set(UCSR0A,UDRE0);
					UDR0=sdigit[0];
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=sdigit[1];
					loop_until_bit_is_set(UCSR0A,UDRE0);
					UDR0='.';
					loop_until_bit_is_set(UCSR0A,UDRE0);
					UDR0=decimal+'0';
					}
					// Time display
								while (!(UCSR0A&(1<<UDRE0)));
								UDR0=' ';
								while (!(UCSR0A&(1<<UDRE0)));
								UDR0=' ';
											while (!(UCSR0A&(1<<UDRE0)));
											UDR0='T';
											while (!(UCSR0A&(1<<UDRE0)));
											UDR0='I';
					
						while (!(UCSR0A&(1<<UDRE0)));
						UDR0='M';
						while (!(UCSR0A&(1<<UDRE0)));
						UDR0='E';		
							while (!(UCSR0A&(1<<UDRE0)));
							UDR0=':';
							num2str(bcd2dec(M41T62_addr[2]));
							while (!(UCSR0A&(1<<UDRE0)));
							UDR0=sdigit[0];	
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=sdigit[1];
					num2str(bcd2dec(M41T62_addr[1]));
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=':';
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=sdigit[0];
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=sdigit[1];
					num2str(bcd2dec(M41T62_addr[0]));
						while (!(UCSR0A&(1<<UDRE0)));
						UDR0=':';
								while (!(UCSR0A&(1<<UDRE0)));
								UDR0=sdigit[0];
								while (!(UCSR0A&(1<<UDRE0)));
								UDR0=sdigit[1];
							
											
					
	    _delay_ms(500);
	   loop_until_bit_is_set(UCSR0A,UDRE0);
	   UDR0=0x0A;
	    _delay_ms(5);
	   loop_until_bit_is_set(UCSR0A,UDRE0);
	   UDR0=0x0D;
	  }
	return 0;
}