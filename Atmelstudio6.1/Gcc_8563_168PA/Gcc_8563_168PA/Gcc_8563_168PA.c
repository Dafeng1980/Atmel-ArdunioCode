/*
 * Gcc_8563_168PA.c
 *
 * Created: 2012/10/20 13:26:36
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

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

#define SLA_W		0xA2	   		 //PFC8563写从地址//I2C-bus slave address:write A2H
#define SLA_R		0xA3			 //PFC8563读从地址//address:read A3H

#define Start()    	(TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN))
#define stop()     	(TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN))
#define Wait()	{while(!(TWCR&(1<<TWINT)));}
#define TestAck() 		(TWSR&0xf8)					//check ack
#define sendAck()	(TWCR|=(1<<TWEA))
#define sendNack()	(TWCR&=~(1<<TWEA))
#define SendByte(x)	{TWDR=(x);TWCR=(1<<TWINT)|(1<<TWEN);} 
#define rcvNackByte()	(TWCR=(1<<TWINT)|(1<<TWEN))
#define rcvAckByte()	(TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA))

/* I2C Config */
#define I2C_ERR			0
#define I2C_CRR			1

//#include <avr/pgmspace.h>
#define SET_SHC PORTD|=1<<PD3      //PD3 is 595 RCLK (Storage register clock)
#define CLR_SHC PORTD&=~(1<<PD3)
#define SPI_595_T(x)   {SPDR=x;while(!(SPSR&(1<<SPIF)));}
//简化宏定义
#define uchar unsigned char
#define uint unsigned int
uchar buff[3];
uchar g_aDisplayBuf[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//TWI初始化
void Twi_Init(void)
   {
   TWBR=12;
   TWSR=0;
   }
uchar PFC8563_Write(uchar slv_addw,uchar sub_suba,uchar Wdata)
{
	Start();
	Wait();
	if(TestAck()!=START)return I2C_CRR;
	SendByte(slv_addw);
	Wait();
	if(TestAck()!=MT_SLA_ACK)return I2C_CRR;
	SendByte(sub_suba);
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(Wdata);
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x00);
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x00);                            //Seconds
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x15);                               //Minutes     
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x02);                               //Hours
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x28);                               //Days
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x00);                               //Weekdays
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x10);                               //Century_months
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	SendByte(0x12);                               //Years
	Wait();
	if(TestAck()!=MT_DATA_ACK)return I2C_CRR;
	
	
	stop();
	return I2C_ERR;
}
uchar PFC8563_Read(uchar slv_addr,uchar suba,uchar *s,uchar num)
{
	uchar n;
	Start();
	Wait();
	if(TestAck()!=START)return 1;
	SendByte(slv_addr-1);
	Wait();
	if(TestAck()!=MT_SLA_ACK)return 1;
	SendByte(suba);
	Wait();
	if(TestAck()!=MT_DATA_ACK)return 1;
	stop();
	Start();
	Wait();
	if(TestAck()!=RESTART)return 1;
	SendByte(slv_addr);
	Wait();
	if(TestAck()!=MR_SLA_ACK)return 1;
	for (n=0;n<num-1;n++)
	{
		rcvAckByte();
		Wait();
		if(TestAck()!=MR_DATA_ACK)return 1;
		*s=TWDR;
		s++;
	}
	rcvNackByte();
	Wait();
	if(TestAck()!=MR_DATA_NACK)return 1;
	*s=TWDR;
	
	stop();
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
	uchar j;
	TCNT1H=0x8F;
	TCNT1L=0x80;
	if(bit_is_clear(PIND,PD2))
	{
		PFC8563_Read(0xA3,0x05,buff,3);
		j=buff[0];
		buff[0]=buff[1]&0x07;   //weeks 
		buff[1]=j&0x3F;         //date
		buff[2]&=0x1f;          //months
		
	}
	else
	{
		
	
	PFC8563_Read(0xA3,0x02,buff,3);
	buff[0]&=0x7F;
	buff[1]&=0x7F;
	buff[2]&=0x3F;
	}	
	}

void displayNumber(uchar num[])

{
	CLR_SHC;
	
	//tem=((hex>>4)*10/16);//取出 小数
	/*if(hex)
	{
		buf[0]=g_aDisplayBuf[num>>4];
		buf[1]=g_aDisplayBuf[num&0x0f];
	}
	else
	{**/
		//buf[1]=g_aDisplayBuf[num%10];
			//temp=num%100;
		//buf[0]=g_aDisplayBuf[temp/10];
			//temp=num/100;
			//buf[2]=g_aDisplayBuf[tem%10];
			
		/*if(num<10)
			buf[0]&=0x7F; //第一个数码管小数点表示百位1
		if(temp>1)
			buf[0]|=0x80;//两个数码管小数点表示百位2**/
	
	
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
	PFC8563_Write(0xA2,0x00,0x00);
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
	  displayNumber(buff);
	
    }
}