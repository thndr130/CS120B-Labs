/*
 * clai022_project_task.c
 *
 * Created: 6/2/2019 1:13:04 AM
 * Author : akiya
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include <stdio.h>

unsigned long int findGCD(unsigned long int a, unsigned long int b){
	unsigned long int c;
	while(1){
		c = a%b;
		if (c==0){
			return b;
		}
		a = b;
		b = c;
	}
	return 0;
}

typedef struct task{
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // input
	DDRB = 0xFF; PORTB = 0x00; // output
	DDRC = 0x00; PORTC = 0xFF; // input
	
	//period of tasks
	unsigned long int motion_tick = 1000;
	unsigned long int sound_tick = 500;
	unsigned long int temp_tick = 5000;
	
	//calculate GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(motion_tick, sound_tick);
	tmpGCD = findGCD(tmpGCD, temp_tick);
	
	unsigned long int GCD = tmpGCD;
	
	unsigned long int motion_period = motion_tick/GCD;
	unsigned long int sound_period = sound_tick/GCD;
	unsigned long int temp_period = temp_tick/GCD;
	
	static task task1, task2, task3, task4;
	task *tasks[] = { &task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	task1.state = -1;
	task1.period = motion_period;
	task1.elapsedTime = motion_period;
	task1.TickFct = &motion_tick;	
	
    while (1) 
    {
    }
}

