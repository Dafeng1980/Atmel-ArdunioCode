/*
 * GccLibrary_I2C.c
 *
 * Created: 2012/7/10 19:51:08
 *  Author: fe
 */ 

#include <avr/io.h>
#include <macros.h>
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

#define SLA_W		0x94	   		 //TPM100д�ӵ�ַ
#define SLA_R		0x95			 //TPM100���ӵ�ַ

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

//�򻯺궨��
#define uchar unsigned char
#define uint unsigned int


//TWI��ʼ��
void Twi_Init(void)
   {
   TWBR=14;
   TWSR=0;
   }

//д��ʱ�亯��
uchar write_tim(uchar slv_addW,uchar sub_suba,uchar Wdata)
    {   
    sendbyte(a);				  //����
	waitack();						  //�ȴ�д���
	if(chkack()!=MT_SLA_ACK)return I2C_ERR;  //����Ƿ���ȷд��
	else return I2C_CRR;
	}
//����ʱ�亯��
uchar read_tim(void)
	{
	rcvackbyte();					  //����һ�ֽڲ�����Ӧ��
	waitack();						  //�ȴ��������
	if(chkack()!=MR_DATA_ACK)return I2C_ERR; 		  //����Ƿ���ճɹ�
//	    return I2C_ERR;
	else
		return(TWDR);				  //����ʱ������
	}

int function(void)
{
    //TODO:: Please write your application code

    return 0;
}