#define numberOfButtons 2

include <avr/io.h>
#include"ButtonPress.h"

int main(void)
{
DDRB = 0b00001100;
PORTB = (1 << PINB0)|(1 << PINB1);

while (1)
{
if (ButtonPressed(0, PINB, 0, 100)) PORTB ^= (1 << PINB2); 

if (ButtonPressed(1, PINB, 1, 100)) PORTB ^= (1 << PINB3);
}
}
