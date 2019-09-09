/*
 * GccLibrary_I2C.c
 *
 * Created: 2012/7/10 19:51:08
 *  Author: fe
 */ 

#include <avr/io.h>
#include <macros.h>
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

#define SLA_W		0x94	   		 //TPM100写从地址
#define SLA_R		0x95			 //TPM100读从地址

#define start()    	(TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN))
#define stop()     	(TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN))
#define waitAck()	{while(!(TWCR&(1<<TWINT)));}
#define chkAck() 		(TWSR&0xf8)					//check ack
#define sendAck()	(TWCR|=(1<<TWEA))
#define sendNack()	(TWCR&=~(1<<TWEA))
#define sendByte(x)	{TWDR=(x);TWCR=(1<<TWINT)|(1<<TWEN);} 
#define rcvNackByte()	(TWCR=(1<<TWINT)|(1<<TWEN))
#define rcvAckByte()	(TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA))

/* I2C Config */
#define I2C_ERR			0
#define I2C_CRR			1

//简化宏定义
#define uchar unsigned char
#define uint unsigned int


//TWI初始化
void Twi_Init(void)
   {
   TWBR=14;
   TWSR=0;
   }

//写入时间函数
uchar write_tim(uchar slv_addW,uchar sub_suba,uchar Wdata)
    {   
    sendbyte(a);				  //发送
	waitack();						  //等待写完成
	if(chkack()!=MT_SLA_ACK)return I2C_ERR;  //检查是否正确写入
	else return I2C_CRR;
	}
//读出时间函数
uchar read_tim(void)
	{
	rcvackbyte();					  //接收一字节并返回应答
	waitack();						  //等待接收完成
	if(chkack()!=MR_DATA_ACK)return I2C_ERR; 		  //检查是否接收成功
//	    return I2C_ERR;
	else
		return(TWDR);				  //读出时钟数据
	}

int function(void)
{
    //TODO:: Please write your application code

    return 0;
}