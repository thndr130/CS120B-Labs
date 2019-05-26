
/* Partner 1 Name & Email: Catherine Lai clai022@ucr.edu
 * Partner 2 Name & Email: Suhas Jagannath sjaga003@ucr.edu
 * Lab Section: 28
 * Assignment: Lab 10 Exercise 3
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

enum Three_States {T_start, fir, sec, thi} three_state;
enum Blinking_States {B_start, B_on, B_off} blink_state;
enum Combine_States {C_start, combine} combine_state;
enum Speaker_States {S_start, S_on, S_off, S_wait} speaker_state;
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char speaker = 0x00;
unsigned short count_blink = 0x00;
unsigned short count_three = 0x00;
unsigned short count_speaker = 0x00;
unsigned char temp = 0x00;

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
				// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
				// So when TCNT1 register equals 125,
				// 1 ms has passed. Thus, we compare to 125.
				// AVR timer interrupt mask register
				TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

				//Initialize avr counter
				TCNT1=0;

				_avr_timer_cntcurr = _avr_timer_M;
				// TimerISR will be called every _avr_timer_cntcurr milliseconds

				//Enable global interrupts
				SREG |= 0x80; // 0x80: 1000000
			}

			void TimerOff() {
				TCCR1B = 0x00; // bit3bit1bit0=000: timer off
			}

			void TimerISR() {
				TimerFlag = 1;
			}

			// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
			ISR(TIMER1_COMPA_vect) {
				// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
				_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
				if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
					TimerISR(); // Call the ISR that the user uses
					_avr_timer_cntcurr = _avr_timer_M;
				}
			}

			// Set TimerISR() to tick every M ms
			void TimerSet(unsigned long M) {
				_avr_timer_M = M;
				_avr_timer_cntcurr = _avr_timer_M;
			}
			
			
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
		
		// prevents OCR0A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR0A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0);
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
			

void ThreeLEDs()
{
	switch(three_state) { // transitions
		case T_start:
			three_state = fir;
			break;
		
		case fir:
			if(count_three<300){
				three_state = fir;
				++count_three;
			}
			else{
				three_state = sec;
				count_three = 0;
			}
			break;
			
		case sec:
			if(count_three<300){
				three_state = sec;
				++count_three;
			}
			else{
				three_state = thi;
				count_three = 0;
			}
			break;
			
		case thi:
			if(count_three<300){
				three_state = thi;
				++count_three;
			}
			else{
				three_state = fir;
				count_three = 0;
			}
			break;
			
		default:
			break;
	}
	switch(three_state) // state actions
	{
		case T_start:
			break;
			
		case fir:
			threeLEDs = 0x01;
			break;
		
		case sec:
			threeLEDs = 0x02;
			break;
		
		case thi:
			threeLEDs = 0x04;
			break;
		
		default:
			break;
		
	}
}

void BlinkingLED(){
	switch(blink_state){ // transitions
		case B_start:
		blink_state = B_on;
		break;
		
		case B_on:
		if(count_blink<1000){
			blink_state = B_on;
			++count_blink;
		}
		else{
			blink_state = B_off;
			count_blink = 0;
		}
		break;
		case B_off:
		if(count_blink<1000){
			blink_state = B_off;
			++count_blink;
		}
		else{
			blink_state = B_on;
			count_blink = 0;
		}
		break;
	}
	
	switch(blink_state){ // actions
		case B_start:
		break;
		case B_on:
		blinkingLED = 0x08;
		break;
		case B_off:
		blinkingLED = 0x00;
		break;
	}
}

/*
void BlinkingLED(){
	switch(blink_state){ // transitions
		case B_start:
		blink_state = on;
		break;
		
		case on:
		if(count<10){
			blink_state = on;
			++count;
		}
		else{
			blink_state = off;
			count = 0;
		}
		break;
		case off:
		if(count<10){
			blink_state = off;
			++count;
		}
		else{
			blink_state = on;
			count = 0;
		}
		break;
	}
	
	switch(blink_state){ // actions
		case B_start:
		break;
		case on:
		blinkingLED = 0x08;
		break;
		case off:
		blinkingLED = 0x00;
		break;
	}
}
*/

void Speaker(){
	switch(speaker_state){
		case S_start:
			speaker_state = S_wait;
			count_speaker = 0;
			break;
		case S_on:
			if(((~PINA & 0x04) == 0x04) && (count_speaker < 2)) {
				speaker_state = S_on;
				++count_speaker;		
			}
			else if ((~PINA & 0x04) == 0x04){
				speaker_state = S_off;
				count_speaker = 0;
			}
			else {
				speaker_state = S_wait;
			}
			break;
		case S_off:
			if(((~PINA & 0x04) == 0x04) && (count_speaker < 2)){
				speaker_state = S_off;
				++count_speaker;
			}
			else if ((~PINA & 0x04) == 0x04){
				speaker_state = S_on;
				count_speaker = 0;
			}
			else {
				speaker_state = S_wait;
			}
			break;
		case S_wait:
			if((~PINA & 0x04) == 0x04){
				speaker_state = S_on;
			}
			else {
				speaker_state = S_wait;
			}
			break;
		default:
			break;
	}
	switch(speaker_state){
		case S_start:
			break;
		case S_on:
			speaker = 0x40;
			//set_PWM(261.62);
			break;
		case S_off:
			speaker = 0;	
			//set_PWM(0.0);
			break;
		case S_wait:
			speaker = 0x00;
			count_speaker = 0;
			break;
		default:
			break;
	}
}

void CombineLEDs(){
	switch(combine_state){// transitions
		case C_start:
			combine_state = combine;
			break;
		case combine:
			break;
		default:
			break;
	}
	
	switch(combine_state){ // actions
		case C_start:
			break;
		case combine:
			temp = threeLEDs | blinkingLED | speaker;
			PORTB = temp;
			break;	
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	three_state = T_start;
	blink_state = B_start;
	speaker_state = S_start;
	combine_state = C_start;
	
	count_blink = 0;
	count_three = 0;
	count_speaker = 0;
	
	TimerSet(1);
	TimerOn();
	//PWM_on();
	
	while(1)
	{
		ThreeLEDs();
		BlinkingLED();
		Speaker();
		CombineLEDs();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}


