/*
 * GccApplicationTPM100.c
 *
 * Created: 2012/7/10 20:15:01
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/delay.h>

//#include <macros.h>
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

#define SLA_W		0xA2	   		 //PCF8563写从地址
#define SLA_R		0xA3			 //PCF8563读从地址

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

//简化宏定义
#define uchar unsigned char
#define uint unsigned int
uchar buff[2];


//TWI初始化
void Twi_Init(void)
   {
   TWBR=2;
   TWSR=0;
   }
uchar TMP100_Write(uchar slv_addw,uchar sub_suba,uchar Wdata)
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
	stop();
	return I2C_ERR;
}
uchar TMP100_Read(uchar slv_addr,uchar suba,uchar *s,uchar no)
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
	for (n=0;n<no-1;n++)
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
	TCNT1H=0xC2;
	TCNT1L=0xF7;
	TCCR1A=0x00;
	TCCR1B=0x03;
}
SIGNAL(TIMER1_OVF_vect)
{
	TCNT1H=0xC2;
	TCNT1L=0xF7;
	//TMP100_Write(0x94,0x01,0x60);
	//TWBR=2;
	//TMP100_Write(0x94,0x00,0x00);
	//TWBR=2;
	TMP100_Read(0x95,0x00,buff,1);
	//buff[0]=0x30;
	TWBR=4;
}
void DelayMs(uint t)
{
	uint i;
	for(i=0;i<t;i++)
	_delay_loop_2(250*2);
}
int main(void)
{
	
	DDRC=0x00;
	PORTC=0xff;
	DDRD=0x00;
	PORTD=0x00;
	Twi_Init();
	DelayMs(100);
	cli();
	Timer1_init();
	MCUCR=0x00;
	GICR=0x00;
	TIMSK=0x05;
	sei();
	UCSRB=(1<<RXEN)|(1<<TXEN);//
	//UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRRL=11;//baud=9600 UBRR=CK/(baud*16) -
	
	
    while(1)
    {
      
	  loop_until_bit_is_set(UCSRA,UDRE);
	  UDR=buff[0];
	  DelayMs(500);
	  
	  
	    //TODO:: Please write your application code 
    }
}