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
#include "usart.h"
#include "scheduler.h"
#include <util/delay.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
/*
enum Temp_Sensor {T_start, T_high, T_reset, T_wait} temp_state;
enum Motion_Sensor {M_start, M_high, M_reset, M_wait} motion_state;
enum Sound_Sensor {S_start, S_high, S_reset, S_wait} sound_state;
unsigned short T_count = 0;
unsigned short M_count = 0;
unsigned short S_count = 0;
unsigned short sound = 0;
unsigned short motion = 0;
unsigned short temp = 0;
unsigned char lights = 0;
unsigned char reset = 0x00;

void Temp_Tick(){
	//reset = ~PINB & 0x02;
	
	switch(temp_state){
		case T_start:
			temp_state = T_wait;
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
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
			break;
		case T_wait:
			break;
		case T_high:
			temp = 100;
			//lights = lights | 0x02;
			//PORTD = lights;
			break;
		case T_reset:
			temp = 70;
			//lights = 0;
			//PORTD = lights;
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
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
			break;
		case M_wait:
			if ( ((PINB&0x01) == 1) && M_count >= 2000 && !reset){ //if sound detector, detects sound go to S_high
				motion_state = M_high;
			}
			else if ( reset && (M_count >= 2000)){ // go to S_low
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
			motion = 1;
			//lights = lights | 0x01;
			//PORTD = lights;
			break;
		case M_reset:
			motion = 0;
			//lights = 0;
			//PORTD = lights;
			break;
		default:
			break;
	}
}


void Sound_Tick(){
	//reset = ~PINB & 0x02;
	
	switch(sound_state){// transitions
		case S_start:
			sound_state = S_wait;
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
			break;
		case S_wait:
			if ( ((PINC&0x01) == 1) && (S_count >= 1000) && !reset ){ //if sound detector, detects sound go to S_high
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
			sound = 1;
			//lights = lights | 0x04;
			//PORTD = lights;
			break;
		case S_reset:
			sound = 0;
			//lights = 0;
			//PORTD = lights;
			break;
		default:
			break;	
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // input
	DDRB = 0x00; PORTB = 0xFF; 
	DDRC = 0x00; PORTC = 0xFF;
	//DDRD = 0xFF; PORTD = 0x00;
	
	
	temp_state = T_start; // every 5 secs 
	motion_state = M_start; // every 2 sec
	sound_state = S_start; // every 1 sec
	
	ADC_init();
	TimerSet(1);
	TimerOn();
	initUSART(0);
	USART_Flush(0);
	
	while(1)
	{
		reset = ~PINB & 0x02;
		
		Sound_Tick(); // take sound every 1 sec
		while(!USART_IsSendReady(0));
		USART_Send(sound, 0);
		while(!USART_HasTransmitted(0));
		_delay_ms(1000);
		
		Temp_Tick(); // take temp every 5 sec
		while(!USART_IsSendReady(0));
		USART_Send(temp, 0);
		while(!USART_HasTransmitted(0));
		_delay_ms(1000);
		
		Motion_Tick(); // take motion every 2 sec
		while(!USART_IsSendReady(0));
		USART_Send(motion, 0);
		while(!USART_HasTransmitted(0));
		_delay_ms(1000);
		
		
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}
*/

// task scheduler //////////////////////////////////////////////////////////

unsigned short T_count = 0;
unsigned short M_count = 0;
unsigned short S_count = 0;
unsigned short sound = 0;
unsigned short motion = 0;
unsigned short temp = 0;
unsigned char lights = 0;
unsigned char reset = 0x00;
unsigned short adcResult;

enum Temp_Sensor {T_start, T_high, T_reset, T_wait} temp_state;
enum Motion_Sensor {M_start, M_high, M_reset, M_wait} motion_state;
enum Sound_Sensor {S_start, S_high, S_reset, S_wait} sound_state;

int Temp_Tick(int temp_state){
	switch(temp_state){
		case T_start:
			temp_state = T_wait;
			/*
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
			*/
			break;
		case T_wait:
			if ( ((PINA&0x01) == 1) && T_count >= 20 && !reset){
				temp_state = T_high;
			}
			else if (reset && T_count >= 20){
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
			temp_state = T_wait;
			break;
	}
	switch(temp_state){
		case T_start:
			break;
		case T_wait:
			break;
		case T_high:
			adcResult = ADC;
			temp = (char) ((adcResult - 500)/10);
			if (USART_IsSendReady(0)){
				USART_Send(temp, 0);
			}
			//lights = lights | 0x02;
			//PORTD = lights;
			break;
		case T_reset:
			temp = 70;
			if (USART_IsSendReady(0)){
				USART_Send(temp, 0);
			}
			//lights = 0;
			//PORTD = lights;
			break;
		default:
			break;
	}
	return temp_state;
}

int Motion_Tick(int motion_state){
	switch(motion_state){
		case M_start:
			motion_state = M_wait;
			/*
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
			*/
			break;
		case M_wait:
			if ( ((PINB&0x01) == 1) && M_count >= 20 && !reset){ //if sound detector, detects sound go to S_high
				motion_state = M_high;
			}
			else if ( reset && (M_count >= 20)){ // go to S_low
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
			motion_state = M_wait;
			break;
	}
	switch(motion_state){
		case M_start:
			break;
		case M_wait:
			break;
		case M_high:
			motion = 1;
			if (USART_IsSendReady(0)){
				USART_Send(motion, 0);
			}
			//lights = lights | 0x01;
			//PORTD = lights;
			break;
		case M_reset:
			motion = 0;
			if (USART_IsSendReady(0)){
				USART_Send(motion, 0);
			}
			//lights = 0;
			//PORTD = lights;
			break;
		default:
			break;
	}
	return motion_state;
}

int Sound_Tick(int sound_state){
	switch(sound_state){
		case S_start:
			sound_state = S_wait;
			/*
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
			*/
			break;
		case S_wait:
			if ( ((PINC&0x01) == 1) && (S_count >= 10) && !reset ){ //if sound detector, detects sound go to S_high
				sound_state = S_high;
			}
			else if (reset && (S_count >= 10)){
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
			sound_state = S_wait;
			break;
	}
	switch(sound_state){
		case S_start:
			break;
		case S_wait:
			break;
		case S_high:
			sound = 1;
			if (USART_IsSendReady(0)){
				USART_Send(sound, 0);
			}
			//lights = lights | 0x04;
			//PORTD = lights;
			break;
		case S_reset:
			sound = 0;
			if (USART_IsSendReady(0)){
				USART_Send(sound, 0);
			}
			//lights = 0;
			//PORTD = lights;
			break;
		default:
			break;
	}
	return sound_state;
}

int main(void){
	DDRA = 0x00; PORTA = 0xFF; // input
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	
	ADC_init();
	initUSART(0);
	USART_Flush(0);
	
	unsigned long int tempCalc = 200;
	unsigned long int soundCalc = 100;
	unsigned long int motionCalc = 200;
	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(tempCalc, soundCalc);
	tmpGCD = findGCD(tmpGCD, motionCalc);
	
	unsigned long int GCD = tmpGCD;
	
	unsigned long int tempPeriod = tempCalc/GCD;
	unsigned long int soundPeriod = soundCalc/GCD;
	unsigned long int motionPeriod = motionCalc/GCD;
	
	static task task1, task2, task3;
	task *tasks[] = {&task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	
	task1.state = -1;
	task1.period = soundPeriod;
	task1.elapsedTime = soundPeriod;
	task1.TickFct = &Sound_Tick;
	
	//Task 2
	
	task2.state = -1;
	task2.period = tempPeriod;
	task2.elapsedTime = tempPeriod;
	task2.TickFct = &Temp_Tick;
	
	//Task 3
	
	task3.state = -1;
	task3.period = motionPeriod;
	task3.elapsedTime = motionPeriod;
	task3.TickFct = &Motion_Tick;
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i;
	
	while (1)
	{// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += GCD;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
	
}
