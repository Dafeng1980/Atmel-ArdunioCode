/*
 * Atmel168_URAT.c
 *
 * Created: 2015/6/18 9:19:46
 *  Author: PE_NPI
 */ 

#define F_CPU 8000000
#include <avr/io.h>
//#include "uart.h"
#include <avr/interrupt.h>
#include "i2c_168.h"
#include <util/delay.h>
#define BAUD 9600
#define MYUBPR F_CPU/16/BAUD-1
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
volatile unsigned char M41T62_addr[7];
char sdigit[3]={'0','0','\0'};
	
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
		//temperatureSum1 = ((msb<<8) | lsb ) >> 4; //
		decimal = (lsb>>4)*10/16;
		temperatureSum1 = msb;
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
			i2c_write(M41T62_addr[i]);
			//i2c_write(bcd2dec(M41T62_addr[i]));
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
		M41T62_addr[1]=00;
		M41T62_addr[2]=00;
		M41T62_addr[3]=3;
		M41T62_addr[4]=24;
		M41T62_addr[5]=06;
		M41T62_addr[6]=15;
		_delay_ms(10);
		Write_M41T62();
	}
	
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
	ISR(TIMER0_OVF_vect)
	{
		cli();
		Read_MT41T62();
		_delay_ms(10);
		Read_TMP112();
		sei();
	}
	
FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);
  int main(void)

{		
	char ichoice;
	stdout = stdin = &uart_str;
		uart_init(MYUBPR);
			TWSR = 0x00;
			TWBR = 0x24;
			//Init_M41T62_Time();
			TCCR0A=00;	//TCCR2A=0x00;
			TCCR0B=(1<<CS02)|(1<<CS00);
			//TCCR2B=(1<<CS21);
			TIMSK0=(1<<TOIE0);
			//TIMSK2=(1<<TOIE0);
			sei();
					
    while(1) //loop forever
    {
		//Read_MT41T62();
		//_delay_ms(10);
		//Read_TMP100();
		//_delay_ms(10);
		//Read_TMP112();
		//_delay_ms(50);
		/*
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
		*/
	ansi_me();
	ansi_cl();                            // Clear Screen
	ansi_cm(1,1);
	printf("AVR Mega168PA M41T62 RTC Setup");
	ansi_cm(3,1);
	printf("1. Time: %02d:%02d:%02d\n",M41T62_addr[2],M41T62_addr[1],M41T62_addr[0]);
	//printf("2. Mode 24/12: %d, AM/PM: %d\n",hour_mode,ampm_mode);
	printf("2. Date: %02d-%02d-20%02d, Week Day: %d\n",M41T62_addr[4],M41T62_addr[5],M41T62_addr[6],M41T62_addr[3]);
	printf("3. Temperature: %02d.%01d\n",temperatureSum1,decimal);
	printf("4. Save and Exit\n");
	printf("5. Exit\n");
	printf("\nEnter Choice: ");	
	if ((ichoice=getnumber(1,6)) < 0) continue;
	
			switch (ichoice) 
			{
			case 1:  // DS1307 Time Setup
			printf("\n\nHour [0-24]: ");
			if ((M41T62_addr[2]=getnumber(0,24)) < 0) continue;
			printf("\nMinute [0-59]: ");
			if ((M41T62_addr[1]=getnumber(0,59)) < 0) continue;
			//M41T62_addr[1]=M41T62_addr[1]&0x7F;
			printf("\nSecond [0-59]: ");
			if ((M41T62_addr[0]=getnumber(0,59)) < 0) continue;
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
			if ((M41T62_addr[3]=getnumber(1,7)) < 0) continue;
			printf("\nDate [1-31]: ");
			if ((M41T62_addr[4]=getnumber(1,31)) < 0) continue;			  ;
			printf("\nMonth [1-12]: ");
			if ((M41T62_addr[5]=getnumber(1,12)) < 0) continue;
			printf("\nYear [0-99]: ");
			if ((M41T62_addr[6]=getnumber(0,99)) < 0) continue;
			break;
			case 4:  // Save to DS1307 Register and Exit Setup
			Write_M41T62();
			case 5:  // Exit Setup
			//mode = 0;
			ansi_cl();
			break;
		
	 } 
	 }        /* while (!(UCSR0A&(1<<UDRE0)));
							UDR0='T';
							//_delay_ms(10);
			   while (!(UCSR0A&(1<<UDRE0)));
							UDR0='M';
							//_delay_ms(10);
			   while (!(UCSR0A&(1<<UDRE0)));
							UDR0='P';
							//_delay_ms(10);
			   while (!(UCSR0A&(1<<UDRE0)));
							UDR0='1';
							//_delay_ms(10);
			   while (!(UCSR0A&(1<<UDRE0)));
							UDR0='0';
							//_delay_ms(10);
			   while (!(UCSR0A&(1<<UDRE0)));
							UDR0='0';
							//_delay_ms(10);
			   while (!(UCSR0A&(1<<UDRE0)));
							UDR0=':';
							//_delay_ms(10);
							
		if(flag)  //negative temperature console output
		{
			while (!(UCSR0A&(1<<UDRE0)));
			UDR0='-';
			//_delay_ms(10);
			while (!(UCSR0A&(1<<UDRE0)));
			UDR0=sdigit[0];
			//_delay_ms(10);
			while (!(UCSR0A&(1<<UDRE0)));
			UDR0=sdigit[1];
			while (!(UCSR0A&(1<<UDRE0)));
			UDR0='.';
			//_delay_ms(10);
			while (!(UCSR0A&(1<<UDRE0)));
			UDR0=decimal+'0';  
			}
			
		else
			{
	   while (!(UCSR0A&(1<<UDRE0)));
	   UDR0=sdigit[0];
	    //_delay_ms(10);
		while (!(UCSR0A&(1<<UDRE0)));
		UDR0=sdigit[1];
			while (!(UCSR0A&(1<<UDRE0)));
				UDR0='.';
				//_delay_ms(10);
			while (!(UCSR0A&(1<<UDRE0)));
				UDR0=decimal+'0';
			}
			
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
				
						while (!(UCSR0A&(1<<UDRE0)));
									UDR0=' ';
									_delay_ms(50);
						while (!(UCSR0A&(1<<UDRE0)));
									UDR0=' ';
									_delay_ms(50);
						while (!(UCSR0A&(1<<UDRE0)));
											UDR0='T';
						while (!(UCSR0A&(1<<UDRE0)));
											UDR0='M';
						while (!(UCSR0A&(1<<UDRE0)));
											UDR0='P';
						while (!(UCSR0A&(1<<UDRE0)));
											UDR0='1';
						while (!(UCSR0A&(1<<UDRE0)));
											UDR0='1';
						while (!(UCSR0A&(1<<UDRE0)));
											UDR0='2';
				        while (!(UCSR0A&(1<<UDRE0)));
				                          UDR0=':';
				if(flag1)
				{
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0='-';
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=sdigit[0];
					while (!(UCSR0A&(1<<UDRE0)));					
					UDR0=sdigit[1];
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0='.';
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=decimal+'0';	
				}
				
				else
				{
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=sdigit[0];
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0=sdigit[1];
					while (!(UCSR0A&(1<<UDRE0)));
					UDR0='.';
					while (!(UCSR0A&(1<<UDRE0)));
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
	   while (!(UCSR0A&(1<<UDRE0)));
	   UDR0=0x0A;
	    _delay_ms(5);
	   while (!(UCSR0A&(1<<UDRE0)));
	   UDR0=0x0D;
	   while (!(UCSR0A&(1<<UDRE0)));
	   UDR0='\n';
	  }
	  */
	return 0;
}