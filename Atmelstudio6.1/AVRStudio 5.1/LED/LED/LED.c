/*
	I/O口输出实验-1位数码管

	晶振频率：
		默认内部1M晶振（注：无需改动熔丝位，请勿轻易改动熔丝位）。
		
	实验说明：
		1、I/O口数码管实验，JP6短路块取出其它插上。
		2、数码管的8路信号由PB口与PC口引出，在输出时需要注意信号的传输到位。
		3、PB和PC口对应引脚的方向设置，在赋值时还应注意不要影响未使用的引脚。
		
	知识点：
		1、本实验和发光二级管实验十分相似，学习点基本相同。
		2、了解数码管的显示原理。
		3、其它一些程序上面的运算语法同请留意体会，不明白的可以对照相关书本一并学习。
		
	编写：
		阿发
		2:22 2008-10-27
		AVR mega8学习板
		www.iccavr.com	www.iccavr.cn
		www.avrbbs.com	www.avrbbs.cn
*/

#include <avr/io.h>
#include <avr/delay.h>
//                   

/*
	延时函数

	函数说明：软件延时函数，此函数延时单位约为1mS。
	晶振频率：默认内部1M晶振。
	带入参数：i无符号整型，变量范围：0-65535。
	返回参数：无
*/
void DelayMs(unsigned int ms)
{
unsigned int i;
for(i=0;i<ms;i++)
_delay_loop_2(250);
}

/*
	LED数码管输出函数

	函数说明：
			LED数码管输出函数，由于数码管的8个位信号对应在两组端口上，
			所以特以函数型式单独处理。		数码管与引脚之间没有直接对应关系，
			所以大家在分析码表时需要对照着图纸。
		
	晶振频率：
			默认内部1M晶振。
		
	带入参数：
			led_b无符号字符型，一个字节长度。
			bit4对应小数点位,为高则亮
			bit7控制低4位是否有效
			字节低4位对应数码管显示值。
		
	返回参数：无
	特殊说明：此函数后面常用到，请留意。
*/
void led_out_b(unsigned char led_b) {

	unsigned char i;
	unsigned char disp[] = {0xB7,0x14,0xAD,0x9D,0x1E,0x9B,0xBB,0x15,0xBF,0x9F,0x3F,0xBA,0xA3,0xBC,0xAB,0x2B};

	/*提取处理带入参数*/
	i = disp[led_b&0x0F];								/*取出低4位字节数值，并带出码值*/
	if (led_b > 0x7F) {									/*判断其bit7值，确定是否显示数值*/
		i = 0x00;
		}
	if (led_b & 0x10) {									/*判断其bit4值，确定小数点状态*/
		i = i | 0x40;
		}

	/*输出到IO口*/
	PORTB=i&0x3F;									/*将输入值的低6位赋值给PB口*/
	if (i & 0x80) {										/*取输入值最高位bit7并对应输出*/
		PORTC|=(1 << PC3);							/*置高其中一位*/
		}
	else {
		PORTC &=~(1 << PC3);							/*置低其中一位*/
		}
	if (i & 0x40) {										/*取输入值最高位bit6并对应输出*/
		PORTC|=(1 << PC2);
		}
	else {
		PORTC&=~(1 << PC2);
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

	DDRB=0xFF;										/*方向输出*/
	PORTB=0x00;										/*低电平*/
	DDRC=0x0C;										/*对应LED引脚为输出*/
	PORTC=0xF3;										/*对应LED引脚输出低电平*/
	DDRD=0x00;										/*方向输入*/
	PORTD=0xFF;										/*打开内部上拉*/
}

/*主函数*/
void main(void) {

	unsigned char j = 0;

	io_init();

	while (1) {											/*程序中主体部分放在一个死循环中*/
		
		/*显示递增*/
		for (j = 0; j < 10; j ++) {						/*标准for循环用法*/
			led_out_b(j);								/*输出变量值到数码管*/
			DelayMs(500);
			}
		
		led_out_b(0x80);								/*熄灭，函数特殊用法*/
		DelayMs(500);
		
		/*显示递减*/
		//for (j = 9; j > 0; j --) {					/*请大家分析两种语句的区别*/
		for (j = 9; j < 10; j --) {						/*可以对比两种方式的结果*/
			led_out_b(j);
			DelayMs(500);
			}

		/*显示小数点闪烁*/
		for (j = 0; j < 6; j ++) {
			led_out_b(0x90);							/*点亮*/
			DelayMs(500);
			led_out_b(0x80);							/*熄灭*/
			DelayMs(500);
			}
		}
}
