/* Partner 1 Name & Email: Catherine Lai clai022@ucr.edu
 * Partner 2 Name & Email: Suhas Jagannath sjaga003@ucr.edu
 * Lab Section: 28
 * Assignment: Lab 9 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>

enum States {start, init, on, off} state;

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << WGM02) | (1 << WGM00) | (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


void tick(){
	switch(state){ // actions
		case start:
			state = init;
			break;
		
		case init:
			if((~PINA & 0x07)!=0x00){
				state = on;
				break;
			}
			else {
				state = init;
				break;
			}
		case on:
			if ((~PINA&0x07)!=0x00){
				state = on;
				break;
			}
			else if ((~PINA&0x07)==0x03){
				state = off;
				break;
			}
			else if ((~PINA&0x07)==0x05){
				state = off;
				break;
			}
			else if ((~PINA&0x07)==0x06){
				state = off;
				break;
			}
			else if ((~PINA&0x07)==0x07){
				state = off;
				break;
			}
			else {
				state = off;
				break;
			}
		case off:
			state = init;
			break;
		default:
			break;
		
	}
	switch(state){ // transitions
		case start:
			break;
		case init:
			set_PWM(0);
			break;
		case on:
			if ((~PINA & 0x07) == 0x01){
				set_PWM(261.63);
				break;
			}
			else if ((~PINA & 0x07) == 0x02){
				set_PWM(293.66);
				break;
			}
			else if ((~PINA & 0x07) == 0x04){
				set_PWM(329.63);
				break;
			}
		case off:
			set_PWM(0);
			break;
		default:
			break;
		}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = start;
	PWM_on();
	/* Replace with your application code */
	while (1)
	{
		tick();
	}
	//PWM_off();
}
