/*
 * temputure_LED.c
 *
 * Created: 2012/10/18 23:23:26
 *  Author: fe
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
//#include <avr/delay.h>

//#include <macros.h>
//�궨��
//״̬�Ĵ���TWSR����״̬����
//��ģʽ����״̬
#define START			0x08		//��������
#define RESTART			0x10		//������������

//������ģʽ����״̬
#define MT_SLA_ACK		0x18		//SLA_Wд��ַ�ѷ��ͣ��յ�Ӧ��λ
#define MT_SLA_NACK		0x20		//SLA_Wд��ַ�ѷ��ͣ��յ���Ӧ��λ
#define MT_DATA_ACK		0x28		//д�������ѷ��ͣ��յ�Ӧ��λ
#define MT_DATA_NACK	0x30		//д�������ѷ��ͣ��յ�Ӧ��λ
#define MT_ARB_LOST		0x38		//SLA_W�������ٲ�ʧ��

//������ģʽ����״̬
#define MR_ARB_LOST		0x38		//
#define MR_SLA_ACK		0x40		//SLA_R�ѷ��ͣ��յ�Ӧ��λ
#define MR_SLA_NACK		0x48		//SLA_R�ѷ��ͣ��յ���Ӧ��λ
#define MR_DATA_ACK		0x50		//���յ����ݣ�Ӧ��λ�ѷ���
#define MR_DATA_NACK	0x58		//���յ����ݣ���Ӧ��λ�ѷ���

#define SLA_W		0xA2	   		 //TMP112д�ӵ�ַ
#define SLA_R		0xA3			 //TMP112���ӵ�ַ

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
//�򻯺궨��
#define uchar unsigned char
#define uint unsigned int
uchar buff[2];
uchar g_aDisplayBuf[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07, \
0x7F,0x6F,0x88,0x83,0xc6,0xa1,0x86,0x8e};
//TWI��ʼ��
void Twi_Init(void)
   {
   TWBR=10;
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
	TCNT1H=0x90;
	TCNT1L=0xFF;
	TCCR1A=0x00;
	TCCR1B=0x03;
}
SIGNAL(TIMER1_OVF_vect)
{
	TCNT1H=0x90;
	TCNT1L=0xFF;
	//TMP100_Write(0x92,0x01,0x60);
	//TWBR=2;
	//TMP100_Write(0x94,0x00,0x00);
	//TWBR=2;
	TMP100_Read(0x93,0x00,buff,2);
	//buff[0]=0x30;
	//TWBR=4;
}
/*void DelayMs(uint t)
{
	uint i;
		for(i=0;i<t;i++)
			_delay_loop_2(250*7);
}
*/

void displayNumber(uchar num,uchar hex)

{
	uchar buf[3];
	uchar temp;
	uchar tem;
	CLR_SHC;
	
	tem=hex>>4;//ȡ�� С��
	if(num>127)
	{	
		if(num==0xFF)num=0;
	else	num=(0xFF-num)+1;
		tem=((0x0F-tem)+1)*10/16;
		
		buf[1]=g_aDisplayBuf[num%10];
		temp=num%100;
		buf[0]=g_aDisplayBuf[temp/10];
		temp=num/100;
		buf[2]=g_aDisplayBuf[tem];
		
		SPI_595_T(0x20);
		SPI_595_T(buf[1]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x10);
		SPI_595_T(buf[0]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x80);
		SPI_595_T(buf[2]);
		SET_SHC;
		CLR_SHC;
		SPI_595_T(0x08);
		SPI_595_T(0x40);
		SET_SHC;
		
		
		
		
	}
		tem=tem*10/16;
		buf[1]=g_aDisplayBuf[num%10];
			temp=num%100;
		buf[0]=g_aDisplayBuf[temp/10];
			temp=num/100;
			buf[2]=g_aDisplayBuf[tem];
			
		/*if(num<10)
			buf[0]&=0x7F; //��һ�������С�����ʾ��λ1
		if(temp>1)
			buf[0]|=0x80;//���������С�����ʾ��λ2**/
	
	
	SPI_595_T(0x20);
	SPI_595_T(buf[1]);
	SET_SHC;
	CLR_SHC;
	SPI_595_T(0x10);
	SPI_595_T(buf[0]);
	SET_SHC;
	CLR_SHC;
	SPI_595_T(0x80);
	SPI_595_T(buf[2]);
	SET_SHC;
	
}
void Port_init(void)
{
	PORTC=0xff;		DDRC=0x00;
	PORTD=0x00;		DDRD=0x08;
}
void Spi_init(void)
{
	DDRB|=_BV(PB5)|_BV(PB3)|_BV(PB2);
		SPCR=_BV(SPE) | _BV(MSTR) | _BV(SPR0);
			SPSR=0x00;
}
int main(void)
{
	Port_init();
	Spi_init();
	Twi_Init();
	//DelayMs(100);
	cli();
	Timer1_init();
	MCUCR=0x00;
	//GICR=0x00;
	TIMSK1=0x01;
	sei();
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);//
	//UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRR0L=47;//baud=9600 UBRR=CK/(baud*16) -
	
	
    while(1)
    {
	  //loop_until_bit_is_set(UCSR0A,UDRE0);
	  //UDR0=buff[0];
	 // tem=((0x0f&(buff[1]>>4))*10/16);//ȡ�� С��
	  displayNumber(buff[0],buff[1]);
	
    }
}