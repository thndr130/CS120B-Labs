/*
 * clai_lab1_part1.c
 *
 * Created: 4/3/2019 6:36:13 PM
 * Author : Catherine Lai
 * SID: 862044772
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpC = 0x00; // Temporary variable to hold the value of B
while(1)
	{
		tmpA = PINA & 0x01;
		tmpB = PINA & 0x02;
		
		if (tmpA == 0x01 && tmpB == 0x00)
		{
			tmpC = 0x01;
		}
		else 
		{
			tmpC = 0x00;
		}
		PORTB = tmpC;
	}
	return 0;
}
