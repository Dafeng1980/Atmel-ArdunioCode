/*
 * AVRGCC3.c
 *
 * Created: 2012/3/8 18:39:50
 *  Author: fe
 */ 

#include <avr/io.h>
#include <avr/delay.h>


void DelayMs(unsigned int ms)
{
unsigned int i;
for(i=0;i<ms;i++)
_delay_loop_2(250);
}
/*
	LED发光二级管输出函数

	函数说明：LED发光二极管输出函数，由于8个发光管对应在两组端口上，
					所以特以函数型式单独处理。
	晶振频率：默认内部1M晶振（此函数与频与关）。
	带入参数：led_a无符号字符型，一个字节长度，其中8个bit位分别对应8个发光二级管。
	返回参数：无
	特殊说明：此函数后面会多次用到，请留意。
*/
void led_out_a(unsigned char led_a) {

	PORTB = led_a & 0x3F;								/*将输入值的低6位赋值给PB口*/
	if (led_a & 0x80) {									/*取输入值最高位bit7并对应输出*/
		PORTC |= (1 << PC3);							/*置高其中一位*/
		}
	else {
		PORTC &= ~(1 << PC3);							/*置低其中一位*/
		}
	if (led_a & 0x40) {									/*取输入值最高位bit6并对应输出*/
		PORTC |= (1 << PC2);
		}
	else {
		PORTC &= ~(1 << PC2);
		}
}

/*
	IO口初始化函数

	函数说明：初始化CPU对应IO口的方向及电平状态。
	晶振频率：任意
	带入参数：无
	返回参数：无
	特殊说明：此函数后面会多次用到，请留意。
*/
void io_init(void) {

	DDRB = 0xFF;										/*方向输出*/
	PORTB = 0x00;										/*低电平*/
	DDRC = 0x0C;										/*对应LED引脚为输出*/
	PORTC = 0xF3;										/*对应LED引脚输出低电平*/
	DDRD = 0x00;										/*方向输入*/
	PORTD = 0xFF;										/*打开内部上拉*/
}



int main(void)
{
	unsigned char j =0;
	io_init();
	
    while (1)
    {
		j=0x01;
		while (j!=0)
		{
			led_out_a(j);
			DelayMs(100);
		
			j=j<<1;
		}			
			j=0x80;
					
			while (j!=0)
			{
				led_out_a(j);
				DelayMs(200);
			
				j=j>>1;
				
			
    }
		}	
		}		
	
