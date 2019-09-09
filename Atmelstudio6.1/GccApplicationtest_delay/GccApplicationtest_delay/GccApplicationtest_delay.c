/*
 * GccApplicationtest_delay.c
 *
 * Created: 2012/7/19 0:16:18
 *  Author: fe
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#define uint unsigned int
#define uchar unsigned char

void delayMs(uint ms)
{
	uint i;
	 for(i=0;i<ms;i++)
		_delay_loop_2(250*1.8);
	
}
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
	uchar j=0;
	io_init();
    while(1)
    {
        /*程序中主体部分放在一个死循环中*/

        /*产生左移动效果*/
        j = 0x01;										/*给显示变量赋值，让其最低位为高电平*/
        while (j != 0) {								/*判断显示变量是否为0*/
        led_out_a(j);								/*输出变量到发光二极管*/
        delayMs(200);
        j = j << 1;									/*左移动显示变量，产生流水灯效果*/
			}
		
		/*产生右移动效果*/
		j = 0x80;
		while (j != 0) {
			led_out_a(j);
			delayMs(200);
			j = j >> 1;
			}
		}//TODO:: Please write your application code 
    }
