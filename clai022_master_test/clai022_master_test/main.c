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

// task scheduler //////////////////////////////////////////////////////////

unsigned char temp = 70;
unsigned char sound = 0;
unsigned char motion = 0;
unsigned char scrollIndex = 0;
unsigned char text[] = "Temp C: --- Sound: - Motion: -";
unsigned char update[30] = "Temp C: --- Sound: - Motion: -";
unsigned char message[16] = "";

enum scrollLcdStates {scrollWait, scrollPrint} scrollState;
enum updateSounds {S_wait, S_update} S_state;
enum updateTemps {T_wait, T_update} T_state;
enum updateMotions {M_wait, M_update} M_state;
	
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

int scrollLcdTick(int scrollState){
	switch(scrollState){
		case scrollWait:
			scrollState = scrollPrint;
			break;
		case scrollPrint:
			if(scrollIndex < sizeof(text)) {
				scrollIndex++;
			}
			else {
				scrollIndex = 0;
			}
			scrollState = scrollWait;
			break;
		default:
			scrollState = scrollWait;
			break;
	}
	switch(scrollState){
		case scrollWait:
			break;
		case scrollPrint:
			messageToSend(update, scrollIndex);
			LCD_DisplayString(1, message);
			break;
		default:
			break;
	}
	return scrollState;
}

int updateSound(int S_state){
	switch(S_state){
		case S_wait:
			S_state = S_update;
			break;
		case S_update:
			if(USART_HasReceived(0)){
				sound = USART_Receive(0);
				S_state = S_wait;
			}
			else {
				S_state = S_update;
			}
			break;
		default:
			S_state = S_wait;
			break;
	}
	switch(S_state){
		case S_wait:
			break;
		case S_update:
			if(sound == 0){
				update[19] = 'N';
			}
			else {
				update[19] = 'Y';
			}
			break;
		default:
			break;
	}
	return S_state;
	
}

int updateTemp(int T_state){
	switch(T_state){
		case T_wait:
			T_state = T_update;
			break;
		case T_update:
			if(USART_HasReceived(0)){
				temp = USART_Receive(0);
				T_state = T_wait;
			}
			else {
				T_state = T_update;
			}
			break;
		default:
			T_state = T_wait;
			break;
	}
	switch(T_state){
		case T_wait:
			break;
		case T_update:
			if(temp < 100){
				update[8] = temp;
				update[11] = ' ';
			}
			else {
				update[8] = temp;
			}
			break;
		default:
			break;
	}
	return T_state;
}

int updateMotion(int M_state){
	switch(M_state){
		case M_wait:
			M_state = M_update;
			break;
		case M_update:
			if(USART_HasReceived(0)){
					motion = USART_Receive(0);
					M_state = M_wait;
			}
			else {
				M_state = M_update;
			}
			break;
		default:
			M_state = M_wait;
			break;
	}
	switch(M_state){
		case M_wait:
			break;
		case M_update:
			if(motion == 0){
				update[29] = 'N';
			}
			else {
				update[29] = 'Y';
			}
			break;
		default:
			break;
	}
	return M_state;
}

int main(void){
	DDRA = 0xFF; PORTA = 0x00; // input
	DDRC = 0xFF; PORTC = 0x00;
	
	LCD_init();
	initUSART(0);
	//USART_Flush(0);
	
	unsigned long int scrollLcdCalc = 25;
	unsigned long int soundsCalc = 500;
	unsigned long int tempsCalc = 500;
	unsigned long int motionsCalc = 500;
	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(scrollLcdCalc, soundsCalc);
	tmpGCD = findGCD(tmpGCD, tempsCalc);
	tmpGCD = findGCD(tmpGCD, motionsCalc);
	
	unsigned long int GCD = tmpGCD;
	
	unsigned long int scrollLcdPeriod = scrollLcdCalc/GCD;
	unsigned long int tempPeriod = tempsCalc/GCD;
	unsigned long int soundPeriod = soundsCalc/GCD;
	unsigned long int motionPeriod = motionsCalc/GCD;
	
	static task task1, task2, task3, task4;
	task *tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	
	task1.state = -1;
	task1.period = scrollLcdPeriod;
	task1.elapsedTime = scrollLcdPeriod;
	task1.TickFct = &scrollLcdTick;
	
	//Task 2
	
	task2.state = -1;
	task2.period = soundPeriod;
	task2.elapsedTime = soundPeriod;
	task2.TickFct = &updateSound;
	
	//Task 3
	
	task3.state = -1;
	task3.period = tempPeriod;
	task3.elapsedTime = tempPeriod;
	task3.TickFct = &updateTemp;
	
	//Task 4
	task4.state = -1;
	task4.period = motionPeriod;
	task4.elapsedTime = motionPeriod;
	task4.TickFct = &updateMotion;
	
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
