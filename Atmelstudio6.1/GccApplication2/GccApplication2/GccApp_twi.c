/*
 * GccApplication2.c
 *
	I2C����/TWI����ģ��
	1����ȡ��д�뺯��
	2����ַ��Ч��Χ255
	3�����ݳ���Ϊ���ֽ�
	4���������ò�ѯ��ʽ
	5����ֱ��Ƕ�뵽��Ŀ��ʹ��
	6������Ƶ�ʣ�1M
	7�������ٶȣ�50K

 * Created: 2012/6/23 10:24:06
 *  Author: fe
 */ 


#include "iomx8.h"
void error(unsigned char type){
	switch (type & 0xF8)
	{
	case 0x20:
			TWCR=(1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
		break;
	case 0x30:
			TWCR=(1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
		break;
	case 0x38:
		break;
	case 0x48:TWCR=(1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
		break;
	}
}
/*I2C ���ֽ�д��*/
unsigned char twi_
