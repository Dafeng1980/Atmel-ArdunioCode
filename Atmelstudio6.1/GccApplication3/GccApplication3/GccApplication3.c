#include <avr/io.h>
#define F_CPU 7230000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
// Notes Frequency from http://www.phy.mtu.edu/~suits/notefreqs.html
// The Original frequency value (decimal) is converted to the integer value
#define C4  262
#define Cc4 277
#define D4  294
#define Dc4 311
#define E4  330
#define F4  349
#define Fc4 370
#define G4  392
#define Gc4 415
#define A4  440
#define Ac4 466
#define B4  494

#define C5  523
#define Cc5 554
#define D5  587
#define Dc5 622
#define E5  659
#define F5  698
#define Fc5 740
#define G5  783
#define Gc5 831
#define A5  880
#define Ac5 932
#define B5  988
#define C6  1047

volatile unsigned char duty_cyc_a,duty_cyc_b, duty_cyc_c,led_a,led_b,led_c;
volatile unsigned int tempo;

// TIMER1 Overflow Interrupt
ISR(TIMER1_OVF_vect)

{
	cli();                  // Disable Interrupt  // Reading User Trimpot on Analog Channel 0
	//ADMUX = 0;  // Start conversion by setting ADSC on ADCSRA Register
	//ADCSRA |= (1<<ADSC);  // wait until convertion complete ADSC=0 -> Complete
	//while (ADCSRA & (1<<ADSC));  // Get the ADC Result
	//tempo=ADCW;
	tempo=0x250;

	if (led_a) {
		if (duty_cyc_a < 255) {
			OCR0A=duty_cyc_a++;
			} else {
			led_a=0;
		}
		} else {
		if (duty_cyc_a > 0) {
			OCR0A=duty_cyc_a--;
			} else {
			led_a=1;
			duty_cyc_a=TCNT1L;
		}
	}

	if (led_b) {
		if (duty_cyc_b < 255) {
			OCR0B=duty_cyc_b++;
			} else {
			led_b=0;
		}
		} else {
		if (duty_cyc_b > 0) {
			OCR0B=duty_cyc_b--;
			} else {
			led_b=1;
			duty_cyc_b=(unsigned char) rand() % 255; ;
		}
	}

	if (led_c) {
		if (duty_cyc_c < 255) {
			OCR2B=duty_cyc_c++;
			} else {
			led_c=0;
		}
		} else {
		if (duty_cyc_c > 0) {
			OCR2B=duty_cyc_c--;
			} else {
			led_c=1;
			duty_cyc_c=TCNT1H;
		}
	}  sei();                      // Enable Interrupt
}// PlayNotes function
void PlayNotes(unsigned int note_frequency,unsigned int duration)
{
	unsigned int top_value,duty_cycle;   // Calculate the Top Value
	// TOP = Board Clock Frequency / (2 x N x Notes Frequency)
	// Where N is Prescler: 8
	top_value=(F_CPU / (16 * note_frequency));   // Reset the TIMER1 16 bit Counter
	TCNT1H = 0;
	TCNT1L = 0;

	// Set the TIMER1 Counter TOP value on ICR1H and ICR1L
	ICR1H = (top_value >> 8 ) & 0x00FF;
	ICR1L = top_value;

	// Set the TIMER1 PWM Duty Cycle on OCR1AH and OCR1AL
	// Always use half of the TOP value (PWM Ducty Cycle ~ 50%)
	duty_cycle=top_value / 2;   
	OCR1AH=(duty_cycle >> 8 ) & 0x00FF;
	OCR1AL=duty_cycle;   // Turn ON the TIMER1 Prescaler of 8
	TCCR1B |= (1<<CS11);

	// Notes Delay Duration
	_delay_ms(250);

	// Turn OFF the TIMER1 Prescaler of 8
	TCCR1B &= ~(1<<CS11);   // Delay Between Each Notes
	_delay_ms(50);
}int main(void)
{
	unsigned char song_index;  // Initial PORT Used
	DDRD = 0b11111111;   // Set PORTD as Output
	PORTD = 0b00000000;
	DDRB = 0b11111110;   // Set PB0 as Input and other as Output
	PORTB = 0b00000000;  // Initial the ADC Peripheral
	//ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);  // Use Free running Mode
	//ADCSRB = 0b00000000;  // Disable digital input on Channel 0
	DIDR0 = 0b00000001;  // Initial TIMER0 Fast PWM
	// Fast PWM Frequency = fclk / (N * 256), Where N is the prescaler
	// f_PWM = 11059200 / (64 * 256) = 675 Hz
	TCCR0A = 0b10100011; // Fast PWM 8 Bit, Clear OCA0/OCB0 on Compare Match, Set on TOP
	TCCR0B = 0b00000011; // Used 64 Prescaler
	TCNT0 = 0;           // Reset TCNT0
	OCR0A = 0;           // Initial the Output Compare register A & B
	OCR0B = 0;  // Initial TIMER1 Phase and Frequency Correct PWM
	// Set the Timer/Counter Control Register
	TCCR1A = 0b11000000; // Set OC1A when up counting, Clear when down counting
	TCCR1B = 0b00010000; // Phase/Freq-correct PWM, top value = ICR1, Prescaler: Off

	TIMSK1 = (1<<TOIE1); // Enable Overflow Interrupt  // Initial TIMER2 Phase Correct PWM Mode
	// Phase Correct PWM Frequency = fclk / (N * 512), Where N is the prescaler
	// f_PWM = 11059200 / (64 * 512) = 337.5 Hz
	TCCR2A = 0b00100001; // Fast PWM 8 Bit, Clear OC2B on Compare Match, Set on TOP
	TCCR2B = 0b00000011; // Used 64 Prescaler
	TCNT2 = 0;           // Reset TCNT2
	OCR2B = 0;           // Initial the Output Compare register A & B  duty_cyc_a=(unsigned char) rand() % 255;
	led_a=1;
	duty_cyc_b=(unsigned char) rand() % 255;
	led_b=1;
	duty_cyc_c=(unsigned char) rand() % 255;
	led_c=1;  
	sei();               // Enable Interrupt  song_index=0;
	//tempo=0;

	for(;;) {            // Loop Forever
		// Playing "What a Wonderfull World" Song Notes
		PlayNotes(G4,300 + tempo); PlayNotes(A4,100 + tempo);
		PlayNotes(C5,500 + tempo); PlayNotes(C5,450 + tempo);
		PlayNotes(G5,1150 + tempo); PlayNotes(A5,350 + tempo);
		PlayNotes(A5,350 + tempo); PlayNotes(A5,150 + tempo);
		PlayNotes(G5,1150 + tempo); PlayNotes(F5,450 + tempo);
		PlayNotes(F5,300 + tempo); PlayNotes(F5,250 + tempo);
		PlayNotes(E5,1150 + tempo); PlayNotes(D5,600 + tempo);
		PlayNotes(E5,175 + tempo); PlayNotes(D5,100 + tempo);
		PlayNotes(C5,1050 + tempo);	PlayNotes(C5,550 + tempo); 
		PlayNotes(C5,175 + tempo);
		PlayNotes(C5,100 + tempo); PlayNotes(C5,100 + tempo);
		PlayNotes(C5,150 + tempo); PlayNotes(C5,1300 + tempo);
		PlayNotes(C5,600 + tempo); PlayNotes(B4,200 + tempo);
		PlayNotes(C5,200); PlayNotes(D5,200 + tempo);	
		if (song_index >= 1) {
			PlayNotes(C5,1600 + tempo);	  
			if (song_index == 3) {
				_delay_ms(100 + 250);
				PlayNotes(C5,550 + tempo); PlayNotes(C5,175 + tempo);
				PlayNotes(C5,100 + tempo); PlayNotes(C5,100 + tempo);
				PlayNotes(C5,150 + tempo); PlayNotes(C5,1300 + tempo);
				PlayNotes(C5,800 + tempo); PlayNotes(B4,400 + tempo);
				PlayNotes(C5,300 + tempo); PlayNotes(D5,300 + tempo);
				PlayNotes(C5,2300 + tempo);	    song_index = 0;
				} 
				else {
				song_index=2;
			}
			} 
			else {
			PlayNotes(E5,1100 + tempo); PlayNotes(E5,800 + tempo);
			PlayNotes(D5,1600 + tempo);
			song_index=1;
			}    
			if (song_index == 2) {
			_delay_ms(100 + 250);      
			PlayNotes(C5,450 + tempo); PlayNotes(D5,150 + tempo);
			PlayNotes(D5,50 + tempo); PlayNotes(D5,50 + tempo);
			PlayNotes(D5,1 + tempo); PlayNotes(D5,1000 + tempo);
			PlayNotes(G4,450 + tempo); PlayNotes(E5,150 + tempo);
			PlayNotes(E5,50 + tempo); PlayNotes(E5,50 + tempo);
			PlayNotes(E5,1 + tempo); PlayNotes(E5,1000 + tempo);      
			PlayNotes(C5,350 + tempo); PlayNotes(D5,250 + tempo);
			PlayNotes(D5,100 + tempo); PlayNotes(D5,75 + tempo);
			PlayNotes(D5,350 + tempo); PlayNotes(C5,150 + tempo);
			PlayNotes(D5,250 + tempo); PlayNotes(E5,1000 + tempo);
			PlayNotes(E5,250 + tempo); PlayNotes(G5,175 + tempo);
			PlayNotes(A5,450 + tempo); PlayNotes(A5,100 + tempo);      
			PlayNotes(E5,150 + tempo); PlayNotes(G5,1000 + tempo);
			PlayNotes(A5,100 + tempo); PlayNotes(A5,50 + tempo);
			PlayNotes(E5,150 + tempo); PlayNotes(G5,1000 + tempo);

			PlayNotes(A5,100 + tempo); PlayNotes(A5,50 + tempo);
			PlayNotes(E5,150 + tempo); PlayNotes(G5,1000 + tempo);
			PlayNotes(F5,450 + tempo); PlayNotes(E5,650);
			PlayNotes(D5,1300 + tempo);      
			song_index =3;
		}
	}

	return 0;	           // Standard Return Code
}