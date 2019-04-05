/*
 * clai_Lab1_Part2.c
 *
 * Created: 4/4/2019 6:26:00 PM
 * Author : Catherine Lai
 * SID: 862044772
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0x0F; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char first = 0x00; // Temporary variable to hold the value of A
	unsigned char second = 0x00; // Temporary variable to hold the value of A
	unsigned char third = 0x00; // Temporary variable to hold the value of A
	unsigned char fourth = 0x00; // Temporary variable to hold the value of A
	unsigned char cntavail = 0x00; // Temporary variable to hold the value of B
	while(1)
	{
		//count number of spots available, then shift numbers down to simplify number
		//then subtract number from 4
		first = PINA & 0x01;
		second = PINA & 0x02;
		third = PINA & 0x04;
		fourth = PINA & 0x08; 
		
		second = second >> 1;
		third = third >> 2;
		fourth = fourth >> 3;

		cntavail = 0x04 - (first + second + third + fourth);
		/*
		if (cntavail == 0x00)
		{
			
		}
		*/
		
		PORTC = cntavail;
		
	}
	return 0;
}