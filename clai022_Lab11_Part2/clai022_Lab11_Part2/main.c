/*	Partner 1 Name & E-mail: Catherine Lai clai022@ucr.edu
 *	Partner 2 Name & E-mail: Suhas Jagannath sjaga003@ucr.edu
 *	Lab Section: 28
 *	Assignment: Lab 11 Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include "io.c"
#include <stdio.h>

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------
unsigned char scrollIndex = 0;
unsigned char text[] = "CS120B is Legend... wait for it DARY!";
unsigned char update[30] = "Temp C: --- Sound: - Motion: -";
unsigned char message[16] = "";
//--------End Shared Variables------------------------------------------------

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

//--------User defined FSMs---------------------------------------------------
enum scrollLcdStates {scrollWait, scrollPrint} scrollLcdState;
	
int scrollLcdTick(int state) {
	//Local variables
	
	//State machine transitions
	switch (state) {
		case scrollWait:
			state = scrollPrint;
			break;
		case scrollPrint:
			if(scrollIndex < sizeof(text)) {
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
			messageToSend(update, scrollIndex);
			LCD_DisplayString(1, message);
			break;
		default:
			state = scrollWait;
			break;
	}
	
	return state;
}
// --------END User defined FSMs-----------------------------------------------


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	LCD_init();
	
	//Period for tasks
	unsigned long int scrollLcdCalc = 25;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(tmpGCD, scrollLcdCalc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD for scheduler
	unsigned long int scrollLcdPeriod = scrollLcdCalc/GCD;
	
	//Declare array of tasks
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	task1.state = -1;
	task1.period = scrollLcdPeriod;
	task1.elapsedTime = scrollLcdPeriod;
	task1.TickFct = &scrollLcdTick;
	
	TimerSet(1);
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
		    tasks[i]->elapsedTime += 1;
	    }
	    while(!TimerFlag);
	    TimerFlag = 0;
    }

    // Error: Program should not exit!
    return 0;
    }
