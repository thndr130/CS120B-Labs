/*
 * lab_chip.c
 *
 * Created: 4/15/2019 9:35:33 PM
 * Author : akiya
 */ 

#include <avr/io.h>

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	while(1)
	{
		PORTB = 0x0F; // Writes port B's 8 pins with 00001111
	}
}

