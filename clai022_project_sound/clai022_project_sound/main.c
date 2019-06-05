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

enum Sound_Sensor {S_start,/* S_low,*/ S_high, S_reset, S_wait} sound_state;
unsigned short S_count = 0;
unsigned short sound = 0;
unsigned char lights = 0;
unsigned char reset = 0x00;


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void Sound_Tick(){
	reset = ~PINA & 0x02;
	switch(sound_state){ // transitions
		case S_start:
			sound_state = S_wait;
			/*
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
			*/
			break;
		case S_wait:
			if ( ((PINA&0x01) == 1) && (S_count >= 500) && !reset ){ 
				sound_state = S_high;
			}
			else if ( reset && S_count >= 500 ){
				sound_state = S_reset;
			}
			/*
			else if (((PINA&0x01) == 0) && (S_count >= 500)){ // go to S_low
				sound_state = S_low;
			}
			*/
			else {
				S_count++;
				sound_state = S_wait;
			}
			break;
		case S_high:
			S_count = 0;
			sound_state = S_wait;
			break;
		/*
		case S_low: // might not need this state bc once warning is sent, won't return to low until reset
			S_count = 0;
			//
			if (S_count < 500){
				S_count++;
			}
			else if (S_count >= 500){
				sound_state = S_wait;
			}
			//
			sound_state = S_wait;
			break;
		*/
		case S_reset:
			S_count = 0;
			sound_state = S_wait;
			break;
		default:
			break;
	}
	
	switch(sound_state){ // actions
		case S_start:
			break;
		case S_wait:
			sound = ADC;
			break;
		case S_high:
			sound = 0;
			lights = lights | 0x04;
			PORTB = lights;
			break;
		/*
		case S_low:
			sound = 0;
			lights = 0;//lights & 0x03; 
			PORTB = lights;
			break;
		*/
		case S_reset:
			sound = 0;
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
	
	sound_state = S_start;
	
	ADC_init();
	TimerSet(1);
	TimerOn();
	//initUSART(0);
	//USART_Flush(0);
	
	while(1)
	{
		Sound_Tick(); 
		/*
		while(!USART_IsSendReady(0));
		USART_Send((sound & 0x00FF, 0);
		while(!USART_HasTransmitted(0));
		*/
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}


