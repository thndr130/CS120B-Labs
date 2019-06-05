/* Partner 1 Name & Email: Catherine Lai clai022@ucr.edu
 * Lab Section: 28
 * Assignment: Final Project
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include <stdio.h>
#include "timer.h"
//#include "usart.h"

enum Motion_Sensor {M_start, /*M_low,*/ M_high, M_reset, M_wait} motion_state;
unsigned short M_count = 0;
//unsigned short motion = 0;
unsigned char lights = 0;
unsigned char reset = 0x00;

/*
void Motion_Tick(){ 
		reset = ~PINA & 0x02;
		switch(motion_state){ // transitions
			case M_start:
				motion_state = M_wait;
				break;
			case M_wait:
				if ( ((PINA&0x01) == 1) && M_count >= 500 && !reset){ //if sound detector, detects sound go to S_high
					motion_state = M_high;
				}
				else if ( (M_count >= 500) && reset){ // go to S_low
					motion_state = M_reset;
				}
				else {
					M_count++;
					motion_state = M_wait;
				}
				break;
			case M_high:
				M_count = 0;
				motion_state = M_wait;
				break;
			//	
			case M_low:
				M_count = 0;
				motion_state = M_wait;
				break;
			//
			case M_reset:
				M_count = 0;
				motion_state = M_wait;
				break;
			default:
				break;
	}
	
	switch(motion_state){ // actions
		case M_start:
			break;
		case M_wait:
			break;
		case M_high:
			lights = lights | 0x02;
			PORTB = lights;
			break;
		//
		case M_low:
			lights = 0;//lights & 0x05;
			PORTB = lights;
			break;
		//
		case M_reset:
			lights = 0;
			PORTB = lights;
			break;
		default:
			break;
	}
}
*/

void Motion_Tick(){
	reset = ~PINA & 0x02;
	switch(motion_state){ // transitions
		case M_start:
			motion_state = M_wait;
			break;
		case M_wait:
			if (((PINA&0x01) == 1) && M_count >= 500 && !reset){ //if sound detector, detects sound go to S_high
				motion_state = M_high;
			}
			else if ( reset && (M_count >= 500)){ // go to S_low
				motion_state = M_reset;
			}
			else {
				M_count++;
				motion_state = M_wait;
			}
			break;
		case M_high:
			M_count = 0;
			motion_state = M_wait;
			break;
		case M_reset:
			M_count = 0;
			motion_state = M_wait;
			break;
		default:
			break;
	}
	
	switch(motion_state){ // actions
		case M_start:
			break;
		case M_wait:
			break;
		case M_high:
			lights = lights | 0x02;
			PORTB = lights;
			break;
		case M_reset:
			lights = 0;
			PORTB = lights;
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // taking input
	DDRB = 0xFF; PORTB = 0x00; // output 
	
	motion_state = M_start;
	
	TimerSet(1);
	TimerOn();
	//initUSART(0);
	//USART_Flush(0);
	
	while(1)
	{
		Motion_Tick();
		/*
		while(!USART_IsSendReady(0));
		USART_Send((sound & 0x00FF, 0);
		while(!USART_HasTransmitted(0));
		*/
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}


