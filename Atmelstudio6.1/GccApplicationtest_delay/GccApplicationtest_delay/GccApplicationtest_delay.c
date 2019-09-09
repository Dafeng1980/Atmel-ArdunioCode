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

	PORTB = led_a & 0x3F;								/*������ֵ�ĵ�6λ��ֵ��PB��*/
	if (led_a & 0x80) {									/*ȡ����ֵ���λbit7����Ӧ���*/
		PORTC |= (1 << PC3);							/*�ø�����һλ*/
		}
	else {
		PORTC &= ~(1 << PC3);							/*�õ�����һλ*/
		}
	if (led_a & 0x40) {									/*ȡ����ֵ���λbit6����Ӧ���*/
		PORTC |= (1 << PC2);
		}
	else {
		PORTC &= ~(1 << PC2);
		}
}

/*
	IO�ڳ�ʼ������

	����˵������ʼ��CPU��ӦIO�ڵķ��򼰵�ƽ״̬��
	����Ƶ�ʣ�����
	�����������
	���ز�������
	����˵�����˺�����������õ��������⡣
*/
void io_init(void) {

	DDRB = 0xFF;										/*�������*/
	PORTB = 0x00;										/*�͵�ƽ*/
	DDRC = 0x0C;										/*��ӦLED����Ϊ���*/
	PORTC = 0xF3;										/*��ӦLED��������͵�ƽ*/
	DDRD = 0x00;										/*��������*/
	PORTD = 0xFF;										/*���ڲ�����*/
}

int main(void)
{
	uchar j=0;
	io_init();
    while(1)
    {
        /*���������岿�ַ���һ����ѭ����*/

        /*�������ƶ�Ч��*/
        j = 0x01;										/*����ʾ������ֵ���������λΪ�ߵ�ƽ*/
        while (j != 0) {								/*�ж���ʾ�����Ƿ�Ϊ0*/
        led_out_a(j);								/*������������������*/
        delayMs(200);
        j = j << 1;									/*���ƶ���ʾ������������ˮ��Ч��*/
			}
		
		/*�������ƶ�Ч��*/
		j = 0x80;
		while (j != 0) {
			led_out_a(j);
			delayMs(200);
			j = j >> 1;
			}
		}//TODO:: Please write your application code 
    }
