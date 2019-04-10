/*
 * clai022_Lab3_Part1.c
 *
 * Created: 4/8/2019 5:09:32 PM
 * Author : Catherine Lai
 * SID: 862044772
 * 
 */ 

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	//unsigned char number = 0x00; // placeholder
	unsigned char amount; // amount from PINA of 1s
	
	
	while(1){
		amount = 0;
		for (unsigned char i = 0; i < 8; i++){
			amount = amount + GetBit(PINA, i);
			amount = amount + GetBit(PINB, i);
		}
	
		PORTC = amount;
	}
	return 0;
}
