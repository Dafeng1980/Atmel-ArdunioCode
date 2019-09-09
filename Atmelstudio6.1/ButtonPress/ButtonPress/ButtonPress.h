/*
 * ButtonPress.h
 *
 * Created: 2016/1/12 13:20:39
 *  Author: PE_NPI
 */ 


#ifndef BUTTONPRESS_H_
#define BUTTONPRESS_H_
#include <avr/io.h>

char ButtonPressed(int buttonNumber, unsigned char pinOfButton, unsigned char portBit, int confidenceLevel);

char Pressed[numberOfButtons];
int Pressed_Confidence_Level[numberOfButtons]; //Measure button press confidence
int Released_Confidence_Level[numberOfButtons]; //Measure button release confidence

char ButtonPressed(int buttonNumber, unsigned char pinOfButton, unsigned char portBit, int confidenceLevel)
{
	if (bit_is_clear(pinOfButton, portBit))
	{
		Pressed_Confidence_Level[buttonNumber] ++; //Increase Pressed Confidence
		Released_Confidence_Level[buttonNumber] = 0; //Reset released button confidence since there is a button press
		sif (Pressed_Confidence_Level[buttonNumber] > confidenceLevel) //Indicator of good button press
		{
			if (Pressed[buttonNumber] == 0)
			{
				Pressed[buttonNumber] = 1;
				return 1;
			}
			//Zero it so a new pressed condition can be evaluated
			Pressed_Confidence_Level[buttonNumber] = 0;
		}
	}
	else
	{
		Released_Confidence_Level[buttonNumber] ++; //This works just like the pressed
		Pressed_Confidence_Level[buttonNumber] = 0; //Reset pressed button confidence since the button is released
		if (Released_Confidence_Level[buttonNumber] > confidenceLevel)
		{
			Pressed[buttonNumber] = 0;
			Released_Confidence_Level[buttonNumber] = 0;
		}
	}
	return 0;
}




#endif /* BUTTONPRESS_H_ */