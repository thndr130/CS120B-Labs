
/* Partner 1 Name & Email: Catherine Lai clai022@ucr.edu
 * Partner 2 Name & Email: Suhas Jagannath sjaga003@ucr.edu
 * Lab Section: 28
 * Assignment: Lab 10 Exercise 5
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

enum BinaryLED_States {inc, dec, wait, reset, button_reset} BL_state;
unsigned char count = 0x00;
unsigned long BL_elapsed = 0x00;
unsigned long BL_time = 0x00;
unsigned long BL_limit = 1000;

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
			
void BinaryLED_Tick(){
	switch(BL_state){
		case wait:
			if (((~PINA & 0x01) == 0x01) && count < 9 && BL_elapsed > BL_limit){
				BL_state = inc;
				BL_elapsed = 0;
			}
			else if (((~PINA & 0x02) == 0x02) && count > 0 && BL_elapsed > BL_limit){
				BL_state = dec;
				BL_elapsed = 0;
			}
			else {
				BL_state = wait;
				BL_elapsed++;
			}
			
			break;
		
		case inc:
			if (BL_time > 3){
				BL_limit = 400;
				//BL_time = 0;
			}
			else {
				BL_limit = 1000;
			}
			if((~PINA & 0x02) == 0x02){
				BL_state = reset;
			}
			else {
				BL_state = wait;
				BL_time++;
			}
			
			break;
		case dec:
			if (BL_time > 3){
				BL_limit = 400;
				//BL_time = 0;
			}
			else {
				BL_limit = 1000;
			}
			if((~PINA & 0x01) == 0x01){
				BL_state = reset;
			}
			else {
				BL_state = wait;
				BL_time++;
			}
			break;
		case reset:
			if ((~PINA & 0x03) == 0x03){
				BL_state = reset;
			}
			else {
				BL_state = button_reset;
			}
			break;
		case button_reset:
			count = 0;
			BL_state = wait;
			break;
			
	}
	switch(BL_state){
		case wait:
			PORTB = count;
			if ((~PINA & 0x03) == 0x00){
				BL_time = 0;
			}
			break;
		case inc:
			count++;
			
			break;
		case dec:
			count--;
			
			break;
		
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1);
	TimerOn();
	BL_state = wait;
	
	while(1)
	{
		BinaryLED_Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}


