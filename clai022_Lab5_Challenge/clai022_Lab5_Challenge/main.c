/*
 * clai022_Lab5_Challenge.c
 *
 * Created: 4/17/2019 4:17:02 PM
 * Author : clai022
 */ 

/*
 *Create your own festive lights display with 6 LEDs connected to port PB5..PB0, lighting in some attractive sequence. 
 *Pressing the button on PA0 changes the lights to the next configuration in the sequence.  
 *Use a state machine (not synchronous) captured in C.
 */

#include <avr/io.h>
enum States{init, wait, fir, sec, thi, fou, fif, six} state;
unsigned char button = 0x00;
unsigned char count = 0x00;

void tick(void){
	button = PINA & 0x01;
	switch(state){
		case init:
			if(button==0x01){
					state = wait;
			}
			else{
				state = init;
			}
			break;
		case wait:
			if (((PINA&0x01)==0x01)){
				state = wait;
			}
			else if(count==0x00){
				state = fir;
			}
			else if(count==0x01){
				state = sec;
			}
			else if(count==0x02){
				state = thi;
			}
			else if(count==0x03){
				state = fou;
			}
			else if(count==0x04){
				state = fif;
			}
			else if(count==0x05){
				state = six;
			}
			else if(count==0x06){
				state = init;
			}
			break;
		case fir:
			if(button==0x01){
					state = wait;
			}
			else{
				state = fir;
			}
			break;
		case sec:	
			if(button==0x01){
					state = wait;
			}
			else{
				state = sec;
			}
			break;
		case thi:
			if(button==0x01){
					state = wait;
			}
			else{
				state = thi;
			}
			break;
		case fou:
			if(button==0x01){
					state = wait;
			}
			else{
				state = fou;
			}
			break;
		case fif:
			if(button==0x01){
					state = wait;
			}
			else{
				state = fif;
			}
			break;
		case six:
			if(button==0x01){
					state = wait;
			}
			else{
				state = six;
			}
			break;
		default:
			state = init;
			break;
	}
	
	switch(state){
		case init:
			count = 0;
			PORTB = 0x00;
			break;
		case wait:
			break;
		case fir:
			count=0x01;
			PORTB = 0x01;
			break;
		case sec:
			count=0x02;
			PORTB = 0x02;
			break;
		case thi:
			count=0x03;
			PORTB = 0x04;
			break;
		case fou:
			count=0x04;
			PORTB = 0x08;
			break;
		case fif:
			count=0x05;
			PORTB = 0x10;
			break;
		case six:
			count=0x06;
			PORTB = 0x20;
			break;
		default:
			break;
	}
	
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = -1;
    while (1) 
    {
		tick();
    }
	return 1;
}

