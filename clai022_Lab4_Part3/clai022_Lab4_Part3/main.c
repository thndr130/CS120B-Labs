/*
 * clai022_Lab4_Part3.c
 *
 * Created: 4/11/2019 4:48:45 PM
 * Author : akiya
 */ 

#include <avr/io.h>
enum door_States {Start, Init, Wait, Pound, Y, Lock, Pause} doorState;

void TickFct_Latch(){ 
	switch(doorState){ //State transitions
		case Start:
			doorState = Init;
		break;
		
		case Init:
			if((PINA & 0x04) == 0x04){
				doorState = Pound; 
			}
			else if ((PINA & 0x80) == 0x80){
				doorState = Lock; 
			}
			else{
				doorState = Init; 
			}
		break;
		
		case Pound:
			doorState = Wait; 
		break;
		
		case Wait:
			if((PINA & 0x02) == 0x02){
				doorState = Y; 
			}
			else if((PINA & 0x80) == 0x80){
				doorState = Lock; 
			}
			else if((PINA & 0x01) == 0x01){
				doorState = Init;
			}
			else{
				doorState = Wait; 
			}
		break;
		
		case Y:
			doorState = Pause; 
		break;
		
		case Pause:
			if((PINA & 0x87) == 0x00){
				doorState = Init; 
			}
			else{
				doorState = Pause; 
			}
		break;
			
		case Lock:
			if((PINA & 0x00) == 0x00){
				doorState = Init; 
			}
			else{
				doorState = Lock; 
			}
		break;
		
		default:
		break;
	}
	
	switch(doorState){ //State actions
		case Start:
			PORTB = 0x02;
			PORTC = 0x00;
		break;
		
		case Init:
			PORTC = 0x01;
		break;
		
		case Pound:
			PORTC = 0x02;
		break;
		
		case Wait:
			PORTC = 0x03;
		break;
		
		case Y:
			PORTC = 0x04;
			PORTB = 0x01;
		break;
		
		case Pause:
			PORTC = 0x05;
		break;
		
		case Lock:
			PORTC = 0x06;
			PORTB = 0x02;
		break;
	}
	
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x02;
	DDRC = 0xFF; PORTC = 0x00;
	
	doorState = Start;
	
	while(1)
	{
		TickFct_Latch();
	}
}