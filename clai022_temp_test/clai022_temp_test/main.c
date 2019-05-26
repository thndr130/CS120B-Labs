/*
 * clai022_temp_test.c
 *
 * Created: 5/26/2019 1:09:18 AM
 * Author : akiya
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

enum states {start, wait, on} state;
unsigned char count = 0;
unsigned short temp = 0;
	
void tick (void){
	switch(state){ //transitions
		case start: 
			state = wait;
			break;
		case wait:
			if (count >= 1000){
				state = on;
			}
			else if (count < 1000) {
				count++;
				state = wait;
			}
			break;
		case on:
			count = 0;
			state = wait;
			break;
	
	}
	
	switch (state) { // actions
		case start:
			break;
		case wait:
			break;
		case on:
			temp = PINA & 0x01;
			PORTB = temp/2;
			break;
	
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1);
	TimerOn();
	state = start;
    while (1) 
    {
		tick();
		while(!TimerFlag) {}
		TimerFlag = 0;	
    }
}

