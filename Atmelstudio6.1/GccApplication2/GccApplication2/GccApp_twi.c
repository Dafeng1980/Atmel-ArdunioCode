/*
 * GccApplication2.c
 *
	I2C总线/TWI操作模块
	1、读取和写入函数
	2、地址有效范围255
	3、数据长度为单字节
	4、函数采用查询方式
	5、可直接嵌入到项目中使用
	6、晶振频率：1M
	7、总线速度：50K

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
/*I2C 单字节写入*/
unsigned char twi_
