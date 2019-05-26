/*
 * clai_Lab1_Challenge.c
 *
 * Created: 4/4/2019 7:28:41 PM
 * Author : Catherine Lai
 * SID: 862044772
 * 
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as outputs
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as outputs, initialize to 0s
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char total = 0x00;
	
	while(1)
	{
		//set port d bit 0 to 1 if cart's total passenger weight exceeds max 140kg
		//set port d bit 1 to 1 if first - third > 80kg

		total = PINA + PINB + PINC;
		if (total > 0x8C && (PINA - PINC > 0x50 || PINC - PINA > 0x50 ))
		{
			PORTD = 0x03;
		}
		else if (total > 0x8C)
		{
			PORTD = 0x01;
		}
		else if (PINA - PINC > 0x50 || PINC - PINA > 0x50)
		{
			PORTD = 0x02;
		}
		
	}
	return 0;
}
