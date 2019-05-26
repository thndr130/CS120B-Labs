/*
 * lab_button.c
 *
 * Created: 4/15/2019 11:17:59 PM
 * Author : akiya
 */ 

#include <avr/io.h>

int main(void)
{
	
		DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
		DDRB = 0xFF; PORTB = 0x00; // Configure PORTB as outputs, initialize to 0s
		unsigned char led = 0x00;
		unsigned char button = 0x00;
		while(1)
		{
			// if PA0 is 1, set PB1PB0=01, else =10
			// 1) Read inputs
			button = ~PINA & 0x01; // button is connected to A0
			// 2) Perform Computation
			if (button) { // True if button is pressed
				led = (led & 0xFC) | 0x01; // Sets B to bbbbbb01
				// (clear rightmost 2 bits, then set to 01)
			}
			else {
				led = (led & 0xFC) | 0x02; // Sets B to bbbbbb10
				// (clear rightmost 2 bits, then set to 10)
			}
			// 3) Write output
			PORTB = led;
		}
	}


	
	/*
	DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
	DDRC = 0xFF; PORTC = 0x00; // Configure PORTB as outputs, initialize to 0s
	//unsigned char led = 0x00;
	unsigned char button = 0x00;
	while(1)
	{
		// if PA0 is 1, set PB1PB0=01, else =10
		// 1) Read inputs
		button = ~PINA & 0x01; // button is connected to A0
		// 2) Perform Computation
		if (button) { // True if button is pressed
			PORTC = 0xFF; // Sets B to bbbbbb01
			// (clear rightmost 2 bits, then set to 01)
		}
		else {
			PORTC = 0x00; // Sets B to bbbbbb10
			// (clear rightmost 2 bits, then set to 10)
		}
		// 3) Write output
		//PORTB = led;
	}
	*/

