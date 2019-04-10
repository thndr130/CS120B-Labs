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
	DDRC = 0xFF; PORTC = 0x00; //C to output
	unsigned char fuelLevel = 0x00;
	
    while (1) 
    {
		fuelLevel = PINA;
		switch(fuelLevel){
			case 1:
				PORTC = 0x60;
			break;
			case 2:
				PORTC = 0x60;
			break;
			case 3:
				PORTC = 0x70;
			break;
			case 4:
				PORTC = 0x70;
			break;
			case 5:
				PORTC = 0x38;
			break;
			case 6:
				PORTC = 0x38;
			break;
			case 7:
				PORTC = 0x3C;
			break;
			case 8:
				PORTC =	0x3C;
			break;
			case 9:
				PORTC =	0x3C;
			break;
			case 10:
				PORTC = 0x3E;
			break;
			case 11:
				PORTC = 0x3E;
			break;
			case 12:
				PORTC = 0x3E;
			break;
			case 13:
				PORTC = 0x3F;
			break;
			case 14:
				PORTC = 0x3F;
			break;
			case 15:
				PORTC = 0x3F;
			break;
		}
    }
}

