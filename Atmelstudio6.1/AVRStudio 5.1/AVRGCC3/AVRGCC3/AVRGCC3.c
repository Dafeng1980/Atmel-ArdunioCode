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
	LED����������������

	����˵����LED����������������������8������ܶ�Ӧ������˿��ϣ�
					�������Ժ�����ʽ��������
	����Ƶ�ʣ�Ĭ���ڲ�1M���񣨴˺�����Ƶ��أ���
	���������led_a�޷����ַ��ͣ�һ���ֽڳ��ȣ�����8��bitλ�ֱ��Ӧ8����������ܡ�
	���ز�������
	����˵�����˺�����������õ��������⡣
*/
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
	
