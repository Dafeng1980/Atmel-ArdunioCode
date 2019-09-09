/*
 * M41T00_168PA.c
 *
 * Created: 2012/10/26 20:23:56
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
//#include <avr/sfr_defs.h>
#define F_CPU	7372800UL
#define delayMs(x)   _delay_ms(x)

//宏定义
//状态寄存器TWSR各种状态定义
//主模式启动状态
#define START			0x08		//启动总线
#define RESTART			0x10		//重新启动总线

//主发送模式各种状态
#define MT_SLA_ACK		0x18		//SLA_W写地址已发送，收到应答位
#define MT_SLA_NACK		0x20		//SLA_W写地址已发送，收到非应答位
#define MT_DATA_ACK		0x28		//写入数据已发送，收到应答位
#define MT_DATA_NACK	0x30		//写入数据已发送，收到应答位
#define MT_ARB_LOST		0x38		//SLA_W或数据仲裁失败

//主接收模式各种状态
#define MR_ARB_LOST		0x38		//
#define MR_SLA_ACK		0x40		//SLA_R已发送，收到应答位
#define MR_SLA_NACK		0x48		//SLA_R已发送，收到非应答位
#define MR_DATA_ACK		0x50		//接收到数据，应答位已返回
#define MR_DATA_NACK	0x58		//接收到数据，非应答位已返回

#define SLA_W		0xD0	   		 //M41T00写从地址//I2C-bus slave address:write D0H
#define SLA_R		0xD1			 //M41T00读从地址//address:read D1H

#define Start()    	{TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN); Wait();}
#define Stop()     	(TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN))
#define Wait()	     while(!(TWCR&(1<<TWINT)))   
#define TestAck() 		(TWSR&0xf8)					//check ack
#define sendAck()	(TWCR|=(1<<TWEA))
#define sendNack()	(TWCR&=~(1<<TWEA))
#define SendByte(x)		{TWDR=(x); TWCR=(1<<TWINT)|(1<<TWEN); Wait();} 
#define rcvNackByte()	{TWCR=(1<<TWINT)|(1<<TWEN); Wait();}
#define rcvAckByte()	{TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA); Wait();}

/* I2C Config */
#define I2C_ERR			0
#define I2C_CRR			1
#define SET_SHC PORTD|=1<<PD3      //PD3 is 595 RCLK (Storage register clock)
#define CLR_SHC PORTD&=~(1<<PD3)
#define SPI_595_T(x)   {SPDR=x;while(!(SPSR&(1<<SPIF)));}
//简化宏定义
#define uchar unsigned char
#define uint unsigned int
uchar buff[3];
uchar g_aDisplayBuf[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	uchar i=0, c=0, h=0;
//TWI初始化
void Twi_Init(void)
   {
   TWBR=12;
   TWSR=0;
   }
uchar M41T00_Write(uchar slv_addw,uchar sub_suba,uchar Wdata)
{
	Start();
	if(TestAck()!=START)return I2C_CRR;
	SendByte(slv_addw);
	if(TestAck()!=MT_SLA_ACK)return I2C_CRR;
	SendByte(sub_suba);
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;//Seconds and bit7 is STOP bit BCD format
	SendByte(Wdata);
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;//Minutes
	SendByte(0x13);
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x18);                            //Hours and Century 
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x05);                               //Day 01-7     
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x02);                               //Date 01-31
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x11);                               //Month 01-12
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x12);                               //Year 00-99
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x00);                               //Control
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	Stop();
	return I2C_ERR;
}
uchar M41T00_Read(uchar slv_addr,uchar suba,uchar *s,uchar num)
{
	uchar n;
	Start();
	if(TestAck()!=START)return I2C_CRR;
	SendByte(slv_addr-1);
	if(TestAck()!=MT_SLA_ACK)return I2C_CRR;
	SendByte(suba);
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	Stop();
	Start();
	if(TestAck()!=RESTART)return I2C_CRR;
	SendByte(slv_addr);
	if(TestAck()!=MR_SLA_ACK)return I2C_CRR;
	for (n=0;n<num-1;n++)
	{
		rcvAckByte();
			if(TestAck()!=MR_DATA_ACK)return I2C_CRR;
				*s=TWDR;
					s++;
	}
	rcvNackByte();
	if(TestAck()!=MR_DATA_NACK)return I2C_CRR;
		*s=TWDR;
	Stop();
	return 0;	
}
void Timer1_init(void)
{
	TCCR1B=0x00;
	TCNT1H=0x8F;
	TCNT1L=0x80;
	TCCR1A=0x00;
	TCCR1B=0x03;
}
SIGNAL(TIMER1_OVF_vect)

{
	TCNT1H=0xAF;
	TCNT1L=0x80;
	uchar key=0x00;
	if (bit_is_clear(PIND,PD2))
	{
		delayMs(10);
		c++;
		//delayMs(10);
		if(c>2)	c=0;
		
		//delayMs(25);
		
	}
	
	if((c==1) && (PIND != key))
	{
		key = PIND;
	M41T00_Read(0xD1,0x03,buff,3);
		buff[0]&=0x07;   //weeks 
			buff[1]&=0x3F;   //date
				buff[2]&=0x1f;   //months
					h=0;
						}
	else if((c==2) && (PIND != key))
	{
		key = PIND;
		M41T00_Read(0x93,0x00,buff,2);    //I2C address 0x92, temperature sensor TMP112
			buff[1]=buff[1]>>4;
				buff[2]=buff[1]*10/16; //
					buff[1]=buff[0]%10;
						buff[0]=buff[0]/10;
							h=1;
									}
	
	
	else	
	{
	M41T00_Read(0xD1,0x00,buff,3);
		buff[0]&=0x7F;
			buff[1]&=0x7F;
				buff[2]&=0x3F;
					h=0;
						}	
}

void displayNumber(uchar num[],uchar i)

{
	if (i==1)
	{
			CLR_SHC;
			SPI_595_T(0x10);
			SPI_595_T(g_aDisplayBuf[num[0]]);
			SET_SHC;
			CLR_SHC;
			SPI_595_T(0x20);
			SPI_595_T(g_aDisplayBuf[num[1]]);
			SET_SHC;
			CLR_SHC;
			SPI_595_T(0x80);
			SPI_595_T(g_aDisplayBuf[num[2]]);
			SET_SHC;
	}
	else
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
void Port_init(void)
{
	PORTC=0xff;		DDRC=0x00;
	PORTD=0x04;		DDRD=0x0C;
}
void Spi_init(void)
{
	DDRB|=_BV(PB5) | _BV(PB3) | _BV(PB2);
		SPCR=_BV(SPE) | _BV(MSTR) | _BV(SPR0);
			SPSR=0x00;
}
int main(void)

{
	Port_init();
		Spi_init();
			Twi_Init();
				//M41T00_Write(0xD0,0x00,0x00);
		cli();
			Timer1_init();
	MCUCR=0x00;
	//GICR=0x00;
	TIMSK1=0x01;
	sei();
	//UCSR0B=(1<<RXEN0)|(1<<TXEN0);//
	//UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	//UBRR0L=47;//baud=9600 UBRR=CK/(baud*16) -
	
    while(1)
    {
	
	  displayNumber(buff,h);
	
    }
}