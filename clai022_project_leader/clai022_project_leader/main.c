/*
 * clai022_project_leader.c
 *
 * Created: 6/5/2019 7:34:56 PM
 * Author : akiya
 */ 

#include <avr/io.h>
#include "io.c"
#include "timer.h"
#include "scheduler.h"
#include "usart.h"

unsigned char input = 0;
unsigned short motion = 0;
enum sendStates {S_init, S_send} S_state;

int sendState(int S_state){
	switch(S_state){
		case S_init:
			S_state = S_send;
			break;
		case S_send:
			S_state = S_send;
			break;
		default:
			S_state = S_init;
			break;
	}
	switch(S_state){
		case S_init:
			motion = 0;
			break;
		case S_send:
			motion = PINB & 0x01;
			if (USART_IsSendReady(0)){
				USART_Send(motion, 0);
			}
			break;
		default:
			break;
	}
	return S_state;
}

int main(void)
{
    DDRB = 0x00; PORTB = 0xFF;
	
	const unsigned short numTasks = 1;
	task tasks[numTasks];
	unsigned char i = 0;
	
	tasks[i].state = 0;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &sendState;
	
	TimerSet(100);
	TimerOn();
	initUSART(0);
		
    while (1) 
    {// Scheduler code
	    for ( i = 0; i < numTasks; i++ ) {
		    // Task is ready to tick
		    if ( tasks[i].elapsedTime >= tasks[i].period ) {
			    // Setting next state for task
			    tasks[i].state = tasks[i].TickFct(tasks[i].state);
			    // Reset the elapsed time for next tick.
			    tasks[i].elapsedTime = 0;
		    }
		    tasks[i].elapsedTime += 100;
	    }
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
}

