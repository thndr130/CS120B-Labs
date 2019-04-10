/*
 * clai022_Lab2_Part3.c
 *
 * Created: 4/9/2019 4:41:46 PM
 * Author : Catherine Lai
 * SID: 862044772
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}



int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; //A to input
    DDRC = 0xFF; PORTC = 0x00; //C to output
    unsigned char fuelLevel = 0x00;
	//unsigned char key = 0x00;
	//unsigned char seated = 0x00;
	//unsigned char seatbelt = 0x00;
	unsigned char holder = 0x00;
    
    while (1)
    {
		holder = 0;
	    fuelLevel = PINA & 0x0F;
	    switch(fuelLevel){
		    case 1:
			holder = 0x60;
		    break;
		    case 2:
		    holder = 0x60;
		    break;
		    case 3:
		    holder = 0x70;
		    break;
		    case 4:
		    holder = 0x70;
		    break;
		    case 5:
		    holder = 0x38;
		    break;
		    case 6:
		    holder = 0x38;
		    break;
		    case 7:
		    holder = 0x3C;
		    break;
		    case 8:
		    holder = 0x3C;
		    break;
		    case 9:
		    holder = 0x3C;
		    break;
		    case 10:
		    holder = 0x3E;
		    break;
		    case 11:
		    holder = 0x3E;
		    break;
		    case 12:
		    holder = 0x3E;
		    break;
		    case 13:
		    holder = 0x3F;
		    break;
		    case 14:
		    holder = 0x3F;
		    break;
		    case 15:
		    holder = 0x3F;
		    break;
			default:
			holder = 0x00;
			break;
	    }
		/*
		key = PINA & 0x04;
		seated = PINA & 0x05;
		seatbelt = PINA & 0x06;
		if (key&&seated&&(~seatbelt)){
			holder = SetBit(holder, 7, 1);
		}
		*/
		
		if (GetBit(PINA, 4) && GetBit(PINA, 5) && ~GetBit(PINA, 6)){
			holder = (holder & 0x7F) | 0x80;
		}
		else {
			holder = (holder & 0x7F) | 0x00;
		}
		/*
		if (GetBit(PINA, 4) && GetBit(PINA, 5) && ~GetBit(PINA, 6)){
			holder = SetBit(holder, 7, 1);
		}
		else {
			holder = SetBit(holder, 7, 0);
		}*/
		PORTC = holder;
    }
	return 0;
}

