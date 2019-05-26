/* Partner 1 Name & Email: Catherine Lai clai022@ucr.edu
 * Partner 2 Name & Email: Suhas Jagannath sjaga003@ucr.edu
 * Lab Section: 28
 * Assignment: Lab 9 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>

enum States {start, inc, dec, init, wait, switch1} state;
double array[9] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25, 0};
double temp =  0;
unsigned char bottom = 0x00;
unsigned char top = 0x07;
unsigned char position = 0x00;
unsigned char clicker = 0x00;
unsigned char tmp = 0x00;

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



void Tick()
{
	switch(state) { // transitions
		case start:
			state = init;
			break;
			
		case init:
			if((~PINA & 0x07) == 0x01){
				state = inc;
				break;
			}
			else if((~PINA & 0x07) == 0x02){
				state = dec;
				break;
			}
			else if((~PINA & 0x07) == 0x04){
				state = switch1;
				break;
			}
			else{
				state = init;
				break;
			}
			
		case inc:
			temp = array[position];
			state = wait;
			break;
		
		case dec:
			temp = array[position];
			state = wait;
			break;
		
		case wait:
			if((~PINA & 0x07) == 0x00){
				state = init;
				break;
			}
			else{
				state = wait;
				break;
			}
		
		case switch1:
			state = wait;
			break;
		
		default:
			break;
	}
	switch(state) // state actions
	{
		case start:
			break;
			
		case init:
			break;
		
		case inc:
			if((position + 1) > top){
				position = top;
				break;
			}
			else{
				++position;
				break;
			}
		
		case dec:
			if((position - 1) < bottom){
				position = bottom;
				break;
			}
			else{
				--position;
				break;
			}
		
		case wait:
		
			temp = array[position];
			set_PWM(temp);
			break;
		
		case switch1:
			if(clicker){
				PWM_off();
				tmp = position;
				position = 8;
				clicker = 0;
				break;
			}
			else{
				PWM_on();
				position = tmp;
				clicker = 1;
				break;
			}
		
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
	clicker = 1;
	
	while(1)
	{
		Tick();
	}
}

