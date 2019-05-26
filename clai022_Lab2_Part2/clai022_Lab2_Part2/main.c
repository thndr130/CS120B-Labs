/*
 * clai022_Lab2_Part2.c
 *
 * Created: 4/9/2019 3:49:29 PM
 * Author : Catherine Lai
 * SID: 862044772
 * 
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //A to input
	DDRB = 0xFF; PORTB = 0x00; //C to output
	unsigned char fuelLevel = 0x00;
	
    while (1) 
    {
		fuelLevel = ~PINA;
		switch(fuelLevel){
			case 0:
				PORTB =	0x40;
				break;
			case 1:
				PORTB = 0x60;
			break;
			case 2:
				PORTB = 0x60;
			break;
			case 3:
				PORTB = 0x70;
			break;
			case 4:
				PORTB = 0x70;
			break;
			case 5:
				PORTB = 0x38;
			break;
			case 6:
				PORTB = 0x38;
			break;
			case 7:
				PORTB = 0x3C;
			break;
			case 8:
				PORTB =	0x3C;
			break;
			case 9:
				PORTB =	0x3C;
			break;
			case 10:
				PORTB = 0x3E;
			break;
			case 11:
				PORTB = 0x3E;
			break;
			case 12:
				PORTB = 0x3E;
			break;
			case 13:
				PORTB = 0x3F;
			break;
			case 14:
				PORTB = 0x3F;
			break;
			case 15:
				PORTB = 0x3F;
			break;
		}
    }
}

