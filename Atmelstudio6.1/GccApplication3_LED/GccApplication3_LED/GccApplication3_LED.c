/*
 * GccApplication3_LED.c
 *
 * Created: 2014/11/26 11:21:10
 *  Author: PE_NPI
 */ 


#include <avr/io.h>
#define F_CPU 7372800
#include <util/delay.h>
int main(void)
{
	unsigned char duty_cyc_a,duty_cyc_b;  // Initial PORT Used
	DDRD = 0b11111111;     // Set PORTD: Output
	PORTD = 0x00;  // Initial TIMER0 Fast PWM
	// Fast PWM Frequency = fclk / (N * 256), Where N is the Prescaler
	// f_PWM = 11059200 / (64 * 256) = 675 Hz
	TCCR0A = 0b10100001; // Fast PWM 8 Bit, Clear OCA0/OCB0 on Compare Match, Set on TOP
	TCCR0B = 0b00000011; // Used 64 Prescaler
	TCNT0 = 0;           // Reset TCNT0
	OCR0A = 0;           // Initial the Output Compare register A & B
	OCR0B = 0;  duty_cyc_a=0;	// Initial Duty Cycle for Channel A
	duty_cyc_b=255;	// Initial Duty Cycle for Channel B

	for(;;) {            // Loop Forever
		while(duty_cyc_a < 255) {
			OCR0A=duty_cyc_a++;
			OCR0B=duty_cyc_b--;
			_delay_ms(10);
		}

		while(duty_cyc_b < 255) {
			OCR0A=duty_cyc_a--;
			OCR0B=duty_cyc_b++;
			_delay_ms(10);
		}
	}

	return 0;
	}	        // Standard Return Code

/*int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}*/
