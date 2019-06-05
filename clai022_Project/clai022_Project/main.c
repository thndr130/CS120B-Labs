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
#include "io.c"
#include <util/delay.h>

enum Temp_Sensor {T_start, T_high, T_reset, T_wait} temp_state;
enum Motion_Sensor {M_start, M_high, M_reset, M_wait} motion_state;
enum Sound_Sensor {S_start, S_high, S_reset, S_wait} sound_state;
unsigned short T_count = 0;
unsigned short M_count = 0;
unsigned short S_count = 0;
unsigned short sound = 0;
unsigned short motion = 0;
unsigned char lights = 0;
unsigned char reset = 0x00;
unsigned short adcResult;
unsigned char calc;

//String: Temp C: 70 Sound: N Motion: N ---- has 30 characters
//temp starts writing at index 8, sound starts writing at 19, motion starts writing at 29

unsigned char scrollIndex = 0;
unsigned char text[30] = "Temp C: --- Sound: - Motion: -";
unsigned char update[30] = "Temp C: --- Sound: - Motion: -";
unsigned char message[16] = "";

void messageToSend(unsigned char string[], unsigned char index) {
	for(char i = 0; i < 15; i++) {
		message[i] = ' ';
	}
	
	if(index < 15) {
		for(char i = 0; i < index; i++) {
			message[16-index+i] = string[i];
		}
	}
	else {
		for(char i = 0; i < 15; i++) {
			message[15-i] = string[index - i];
		}
	}
	
}

enum scrollLcdStates {scrollWait, scrollPrint} scrollLcdState;
	
int scrollLcdTick(int state) {
	//Local variables
	
	//State machine transitions
	switch (state) {
		case scrollWait:
			state = scrollPrint;
			break;
		case scrollPrint:
			if( (scrollIndex < sizeof(text)) && !reset) {
				scrollIndex++;
			}
			else {
				scrollIndex = 0;
			}
			state = scrollWait;
			break;
		default:
			state = scrollWait;
			break;
	}
	//State machine actions
	switch (state) {
		case scrollWait:
			break;
		case scrollPrint:
			messageToSend(update[30], scrollIndex);
			LCD_DisplayString(1, message);
			break;
		default:
			state = scrollWait;
			break;
	}
	
	return state;
}

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}


void Temp_Tick(){
	//reset = ~PINB & 0x02;
	
	switch(temp_state){
		case T_start:
			temp_state = T_wait;
			break;
		case T_wait:
			if ( ((PINA&0x01) == 1) && T_count >= 2000 && !reset){
				temp_state = T_high;
			}
			else if (reset && T_count >= 2000){
				temp_state = T_reset;
			}
			else {
				T_count++;
				temp_state = T_wait;
			}
			break;
		case T_high:
			T_count = 0;
			temp_state = T_wait;
			break;
		case T_reset:
			T_count = 0;
			temp_state = T_wait;
			break;
		default:
			break;
	}
	
	switch(temp_state){
		case T_start:
			LCD_ClearScreen();
			LCD_DisplayString(1, update[30]);
			break;
		case T_wait:
			break;
		case T_high:
			adcResult = ADC;
			calc = (char) ((adcResult - 500)/10);
			//PORTB = calc;
			if(calc < 100){
				update[8] = calc;
				update[11] = ' ';
			}
			else {
				update[8] = calc;
			}
			/*
			if (adcResult >= ###){
				// LCD output Danger!!!
				LCD_DisplayString(1, "DANGER!!!");
			}
			*/
			LCD_ClearScreen();
			LCD_DisplayString(1, update[30]);
			break;
		case T_reset:
			LCD_ClearScreen();
			LCD_DisplayString(1, text[30]);
			break;
		default:
			break;
	}
	
}

void Motion_Tick(){ 
		//reset = ~PINB & 0x02;
		
		switch(motion_state){ // transitions
		case M_start:
			motion_state = M_wait;
			break;
		case M_wait:
			if ( ((PINB&0x01) == 1) && M_count >= 2000 && !reset){
				motion_state = M_high;
			}
			else if ( reset && (M_count >= 2000)){
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
			LCD_ClearScreen();
			LCD_DisplayString(1, update[30]);
			break;
		case M_wait:
			break;
		case M_high:
			update[29] = 'Y';
			LCD_ClearScreen();
			LCD_DisplayString(1, update[30]);
			break;
		case M_reset:
			update[29] = 'N';
			LCD_ClearScreen();
			LCD_DisplayString(1, text[30]);
			break;
		default:
			break;
	}
}


void Sound_Tick(){
	switch(sound_state){// transitions
		case S_start:
			sound_state = S_wait;
			break;
		case S_wait:
			if ( ((PINC&0x01) == 1) && (S_count >= 1000) && !reset ){ 
				sound_state = S_high;
			}
			else if (reset && (S_count >= 1000)){ 
				sound_state = S_reset;
			}
			
			else {
				S_count++;
				sound_state = S_wait;
			}
			break;
		case S_high:
			S_count = 0;
			sound_state = S_wait;
			break;
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
			break;
		case S_high:
			update[19] = 'Y';
			LCD_ClearScreen();
			LCD_DisplayString(1, update[30]);
			break;
		case S_reset:
			update[19] = 'N';
			LCD_ClearScreen();
			LCD_DisplayString(1, text[30]);
			break;
		default:
			break;	
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // input
	DDRB = 0x00; PORTB = 0xFF; 
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	
	temp_state = T_start; // every 5 secs 
	motion_state = M_start; // every 2 sec
	sound_state = S_start; // every 1 sec
	
	LCD_init();
	ADC_init();
	TimerSet(1);
	TimerOn();
	
	while(1)
	{
		reset = ~PINB & 0x02;
		Sound_Tick(); // take sound every 1 sec
		//_delay_ms(100);
		Temp_Tick(); // take temp every 5 sec
		//_delay_ms(100);
		Motion_Tick(); // take motion every 2 sec
		//_delay_ms(100);
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}


