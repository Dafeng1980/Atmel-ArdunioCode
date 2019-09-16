============================================================================= 
/* dmx.c 
 * 
 * DMX-to-8-PWM decoder 
 * 
 */ 

// Includes 
#include <inttypes.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/signal.h> 

// Defines 
#define LED_PORT PORTB 
#define LED_CH8  PB7 
#define LED_CH7  PB6 
#define LED_CH6  PB5 
#define LED_CH5  PB4 
#define LED_CH4  PB3 
#define LED_CH3  PB2 
#define LED_CH2  PB1 
#define LED_CH1  PB0 

#define SWITCH_PORT PIND 

#define FOSC 20000000L 
#define BAUD 250000 
#define MYUBRR (FOSC / 16 / BAUD - 1)   // 250kbps 

#define TIMER_CLOCK 100                 // PWM refresh 

// Consts 
enum {IDLE, BREAK, STARTB, STARTADR};   // DMX available states 

// Vars 
volatile uint8_t dmxRxField[8];         // Array of DMX vals (raw) 
volatile uint16_t dmxAddress;           // Start address 
volatile uint8_t gDmxState;             // Global DMX state 
volatile uint8_t tick;                  // Timer tick 


// IO init 
void initIO(void) 
{ 
    DDRB = 0xff;    // PORTB as output 
    PORTB = 0x00;   // default outputs to 0 

    DDRD = 0x00;    // PORTD as input 
    PORTD = 0xff;   // Enable pullup resistors 
} 


// Timer init 
void initTimer(void) 
{ 
    // Use CLK/1 prescale value, clear timer/counter on compareA match 
    TCCR1B = (1 << CS10) | (1 << WGM12); 

    // Preset timer1 high/low byte 
    OCR1A = ((FOSC / TIMER_CLOCK / 256) - 1); 

    // Enable Timer/Counter1, Output Compare A Match Interrupt 
    TIMSK  = (1 << OCIE1A); 

    tick = 0; 
} 


// USART init 
void initUSART(void) 
{ 
    // Configure USART 
    UCSRB = (1 << RXCIE) | (1 << RXEN); 
    UCSRC = (1 << USBS) | (3 << UCSZ0); 

    // Set baud rate 
    UBRRH = (unsigned char)(MYUBRR >> 8); 
    UBRRL = (unsigned char)(MYUBRR); 
} 


// Get DMX base address from dip switch 
void readDmxAddress(void) 
{ 
    dmxAddress = (((SWITCH_PORT >> 1) ^ 0x3f) << 3) + 1; 
} 


// Timer interrupt routine 
ISR(SIG_TIMER1_COMPA) 
{ 
    tick++; 
} 


// USART interrupt routine 
ISR(SIG_USART0_RECV) 
{ 
    static uint16_t dmxCount; 
           uint8_t USARTstate = UCSRA;      // Get state before data 
           uint8_t dmxByte = UDR;           // Get data 
           uint8_t dmxState = gDmxState;    // Load once from SRAM 

    if (USARTstate & (1 << FE)) {           // Check for break 
        UCSRA &= ~(1 << FE);                // Reset flag 
        dmxCount = dmxAddress;              // Reset channel counter 
        gDmxState = BREAK; 
    } 
    else if (dmxState == BREAK) { 
        if (dmxByte == 0) { 
            gDmxState = STARTB;             // Normal start code detected 
        } 
        else { 
            gDmxState= IDLE; 
        } 
    } 
    else if (dmxState == STARTB) { 
        if (--dmxCount == 0) {              // Start address reached? 
            dmxCount = 1;                   // Set up counter 
            dmxRxField[0] = dmxByte;        // Get 1st DMX channel of device 
            gDmxState = STARTADR; 
        } 
    } 
    else if (dmxState == STARTADR) { 
        dmxRxField[dmxCount++] = dmxByte;   // Get channel 
        if (dmxCount >= 8) {                // All channels received? 
            gDmxState = IDLE;               // Wait for next break 
        } 
    } 
} 

int main(void) 
{ 
    uint8_t LED_REG; 
    uint8_t channel; 
    uint8_t channelFlagTable[8] = {(1 << LED_CH1), 
                                   (1 << LED_CH2), 
                                   (1 << LED_CH3), 
                                   (1 << LED_CH4), 
                                   (1 << LED_CH5), 
                                   (1 << LED_CH6), 
                                   (1 << LED_CH7), 
                                   (1 << LED_CH8)}; 

    // Inits 
    initIO(); 
    initTimer(); 
    initUSART(); 

    // Read DMX start address, set by switches 
    readDmxAddress(); 

    // Init led state 
    LED_REG = 0x00; 

    // Enable interrupts 
    sei(); 

    // Main loop 
    while (1) { 
        for (channel = 0; channel < 8; channel++) { 
            if (tick == dmxRxField[channel]) 
                LED_REG &= ~channelFlagTable[channel]; 
        } 

        if (tick == 255) { 
            tick = 0;   // Needed? 
            for (channel = 0; channel < 8; channel++) { 
                if (dmxRxField[channel] > 0) 
                    LED_REG |= channelFlagTable[channel]; 
            } 
        } 

         LED_PORT = LED_REG;     // Refresh led output 
    } 

    return 1; 
} 
