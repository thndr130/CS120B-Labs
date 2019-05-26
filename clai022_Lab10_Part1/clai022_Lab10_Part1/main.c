
/* Partner 1 Name & Email: Catherine Lai clai022@ucr.edu
 * Partner 2 Name & Email: Suhas Jagannath sjaga003@ucr.edu
 * Lab Section: 28
 * Assignment: Lab 10 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

enum Three_States {T_start, fir, sec, thi} three_state;
enum Blinking_States {B_start, on, off} blink_state;
enum Combine_States {C_start, combine} combine_state;
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned short count = 0x00;
unsigned short cnt = 0x00;
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

void ThreeLEDs()
{
	switch(three_state) { // transitions
		case T_start:
			three_state = fir;
			break;
		
		case fir:
			if(cnt<1000){
				three_state = fir;
				++cnt;
			}
			else{
				three_state = sec;
				cnt = 0;
			}
			break;
			
		case sec:
			if(cnt<1000){
				three_state = sec;
				++cnt;
			}
			else{
				three_state = thi;
				cnt = 0;
			}
			break;
			
		case thi:
			if(cnt<1000){
				three_state = thi;
				++cnt;
			}
			else{
				three_state = fir;
				cnt = 0;
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
			blink_state = on;
			break;
			
		case on:
			if(count<1000){
				blink_state = on;
				++count;
			}
			else{
				blink_state = off;
				count = 0;
			}
			break;
		case off:
			if(count<1000){
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

void CombineLEDs(){
	switch(combine_state){// transitions
		case C_start:
			combine_state = combine;
			break;
		case combine:
			break;
	}
	
	switch(combine_state){ // actions
		case C_start:
			break;
		case combine:
			temp = threeLEDs | blinkingLED;
			PORTB = temp;
			break;	
	}
}

int main(void)
{
	//DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	three_state = T_start;
	blink_state = B_start;
	combine_state = C_start;
	count = 0;
	cnt = 0;
	TimerSet(1);
	TimerOn();
	
	while(1)
	{
		ThreeLEDs();
		BlinkingLED();
		CombineLEDs();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}


