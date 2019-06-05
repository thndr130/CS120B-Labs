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
/*
//String: Temp C: 70 Sound: N Motion: N ---- has 30 characters
//temp starts writing at index 8, sound starts writing at 19, motion starts writing at 29

unsigned char temp;
unsigned char sound;
unsigned char motion;
unsigned char scrollIndex = 0;
unsigned char text[30] = "Temp C: --- Sound: - Motion: -";
unsigned char update[30] = "Temp C: --- Sound: - Motion: -";
unsigned char message[16] = "";

enum scrollLcdStates {scrollWait, scrollPrint} scrollLcdState;
enum updateSounds {S_wait, S_update} sound_state;
enum updateTemps {T_wait, T_update} temp_state;
enum updateMotions {M_wait, M_update} motion_state;

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
	
void scrollLcdTick() {
	//Local variables
	
	//State machine transitions
	switch (scrollLcdState) {
		case scrollWait:
			scrollLcdState = scrollPrint;
			break;
		case scrollPrint:
			if( (scrollIndex < sizeof(text)) ) {
				scrollIndex++;
			}
			else {
				scrollIndex = 0;
			}
			scrollLcdState = scrollWait;
			break;
		default:
			scrollLcdState = scrollWait;
			break;
	}
	//State machine actions
	switch (scrollLcdState) {
		case scrollWait:
			break;
		case scrollPrint:
			messageToSend(update[30], scrollIndex);
			LCD_DisplayString(1, message);
			break;
		default:
			scrollLcdState = scrollWait;
			break;
	}
}

void updateSound(){
	switch(sound_state){
		case S_wait:
			sound_state = S_update;
			break;
		case S_update:
			if (USART_HasReceived(0)){
				sound = USART_Receive(0);
				sound_state = S_wait;
			}
			else {
				sound_state = S_update;
			}
			break;		
	}
	switch(sound_state){
		case S_wait:
			break;
		case S_update:
			if (sound == 0){
				update[19] = 'N';
			}
			else {
				update[19] = 'Y';
			}
			break;
		
	}
}

void updateTemp(){
	switch(temp_state){
		case T_wait:
			temp_state = T_update;
			break;
		case T_update:
			if (USART_HasReceived(0)){
				temp = USART_Receive(0);
				temp_state = T_wait;
			}
			else {
				temp_state = T_update;
			}
			break;
	}
	switch(temp_state){
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
		
	}
}

void updateMotion(){
	switch(motion_state){
		case M_wait:
			motion_state = M_update;
			break;
		case M_update:
			if (USART_HasReceived(0)){
				motion = USART_Receive(0);
				motion_state = M_wait;
			}
			else {
				motion_state = M_update;
			}
			break;
	}
	switch(motion_state){
		case M_wait:
			break;
		case M_update:
			if (motion == 0){
				update[29] = 'N';
			}
			else {
				update[29] = 'Y';
			}
			break;
		
	}
}
*/
/*//////////////////////////////////////////////////////////////////////
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
			////////////////////
			if (adcResult >= ###){
				// LCD output Danger!!!
				LCD_DisplayString(1, "DANGER!!!");
			}
			///////////////////
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
*///////////////////////////////////////////////////////////////////
/*
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	scrollLcdState = scrollWait;
	LCD_init();
	TimerSet(1);
	TimerOn();
	initUSART(0);
	USART_Flush(0);
	
	while(1)
	{
		updateSound();
		_delay_ms(500);
		updateTemp();
		_delay_ms(500);
		updateMotion();
		_delay_ms(500);
		
		scrollLcdTick();
		_delay_ms(5000);
		
	}
}
*/

// task scheduler /////////////////////////////////////////////////////////

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

int scrollLcdTick(int scrollState) {
	//Local variables
	
	//State machine transitions
	switch (scrollState) {
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
	//State machine actions
	switch (scrollState) {
		case scrollWait:
			//LCD_DisplayString(1, "helloSS");
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
				LCD_DisplayString(1,"Received");
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
			//LCD_DisplayString(1, "helloUS");
			break;
		case S_update:
			if(sound == 0){
				update[19] = 'N';
			}
			else {
				update[19] = 'Y';
			}
			LCD_DisplayString(1, message);
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
				LCD_DisplayString(1,"Received");
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
			//LCD_DisplayString(1, "helloUT");
			break;
		case T_update:
			if(temp < 100){
				update[8] = temp;
				update[11] = ' ';
			}
			else {
				update[8] = temp;
			}
			LCD_DisplayString(1, message);
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
				LCD_DisplayString(1,"Received");
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
			//LCD_DisplayString(1, "helloUM");
			break;
		case M_update:
			if(motion == 0){
				update[29] = 'N';
			}
			else {
				update[29] = 'Y';
			}
			LCD_DisplayString(1, message);
			break;
		default:
			break;
	}
	return M_state;
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00; // LCD control lines
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines 
	
	LCD_init();
	initUSART(0);
	USART_Flush(0);
	
	//Period for tasks
	unsigned long int scrollLcdCalc = 25;
	unsigned long int soundsCalc = 5000;
	unsigned long int tempsCalc = 5000;
	unsigned long int motionCalc = 5000;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(scrollLcdCalc, soundsCalc);
	tmpGCD = findGCD(tmpGCD, tempsCalc);
	tmpGCD = findGCD(tmpGCD, motionCalc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD for scheduler
	unsigned long int scrollLcdPeriod = scrollLcdCalc/GCD;
	unsigned long int soundPeriod = soundsCalc/GCD;
	unsigned long int tempPeriod = tempsCalc/GCD;
	unsigned long int motionPeriod = motionCalc/GCD;
	
	//Declare array of tasks
	static task task1, task2, task3, task4;
	task *tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	task1.state = -1;
	task1.period = motionPeriod;
	task1.elapsedTime = motionPeriod;
	task1.TickFct = &updateMotion;
	
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
	task4.period = scrollLcdPeriod;
	task4.elapsedTime = scrollLcdPeriod;
	task4.TickFct = &scrollLcdTick;
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i;
	
	LCD_ClearScreen();
	LCD_DisplayString(1, "hello");
	_delay_ms(100);
	
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
			_delay_ms(100);
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}