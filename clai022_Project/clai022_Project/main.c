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
//#include "usart.h"

enum Temp_Sensor {T_start, T_low, T_high, T_reset, T_wait} temp_state;
//enum Motion_Sensor {M_start, M_low, M_high, M_reset, M_wait} motion_state;
//enum Sound_Sensor {S_start, S_low, S_high, S_reset, S_wait} sound_state;
//enum Output {O_start, O_low, O_high, O_reset} out_state;
unsigned short T_count = 0;
//unsigned short M_count = 0;
//unsigned short S_count = 0;
unsigned char temp_current = 140;
//unsigned short sound = 0;
//unsigned short motion = 0;
unsigned char lights = 0;
const char temp_max = 200;
const char temp_min = 100;


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

/*
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
*/

void Temp_Tick()
{
	switch(temp_state) { // transitions
		case T_start:
			temp_state = T_wait;
			break;
			
		case T_wait:
			if ((temp_current > temp_min) && (temp_current < temp_max) && (T_count >= 5000)){ // if temp inside the car is below 100F and above 50F then stay in low state
				temp_state = T_low;
			}
			else if(((temp_current < temp_min) || (temp_current > temp_max)) && (T_count >= 5000)){ // else if temp is above 100F or below 50F then set off warning
				temp_state = T_high;	
			}
			else {
				T_count++;
				temp_state = T_wait;
			}
			break;
			
		case T_high:// send info to raspberry pi to relay to parent, send notification saying car is at dangerous temperatures
			T_count = 0;
			temp_state = 0;
			break;
		
		case T_low: 
			T_count = 0;
			temp_state = T_wait;
			break;
		case T_reset:
			break;
		default:
			break;
	}
	
	switch(temp_state) // state actions
	{
		case T_start:
			break;
		case T_wait:
			break;
		case T_high:
			lights = lights | 0x01;
			PORTB = lights;
			break;
		case T_low:
			lights = 0;//lights & 0x06;
			PORTB = lights;
			break;
		case T_reset:
			lights = 0;
			PORTB = lights;
			break;		
		default:
			break;
	}
}

/* /////////////////////////////////// works but has its own system period
void Motion_Tick(){ 
		switch(motion_state){ // transitions
		case M_start:
			motion_state = M_wait;
			break;
		case M_wait:
			if (((PINA&0x01) == 1) && M_count >= 500){ //if sound detector, detects sound go to S_high
				motion_state = M_high;
			}
			else if (((PINA&0x01) == 0) && (M_count >= 500)){ // go to S_low
				motion_state = M_low;
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
		case M_low:
			M_count = 0;
			motion_state = M_wait;
			break;
		case M_reset:
			break;
		default:
			break;
	}
	
	switch(motion_state){ // actions
		case M_start:
			break;
		case M_wait:
			break;
		case M_high:
			lights = lights | 0x02;
			PORTB = lights;
			break;
		case M_low:
			lights = 0;//lights & 0x05;
			PORTB = lights;
			break;
		case M_reset:
			lights = 0;
			PORTB = lights;
			break;
		default:
			break;
	}
}
*/
/* /////////////////////////////////////sound sensor works
void Sound_Tick(){
	switch(sound_state){// transitions
		case S_start:
			sound_state = S_wait;
			break;
		case S_wait:
			if (((PINA&0x01) == 1) && S_count >= 500){ //if sound detector, detects sound go to S_high
				sound_state = S_high;
			}
			else if (((PINA&0x01) == 0) && (S_count >= 500)){ // go to S_low
				sound_state = S_low;
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
		case S_low: // might not need this state bc once warning is sent, won't return to low until reset
			S_count = 0;
			/*
			if (S_count < 500){
				S_count++;
			}
			else if (S_count >= 500){
				sound_state = S_wait;
			}
			*/
/*			sound_state = S_wait;
			break;
		case S_reset:
			
			break;
		default:
			break;
	}
	
	switch(sound_state){ // actions
		case S_start:
			break;
		case S_wait:
			sound = ADC;
			break;
		case S_high:
			sound = 0;
			lights = lights | 0x04;
			PORTB = lights;
			break;
		case S_low:
			sound = 0;
			lights = 0;//lights & 0x03; 
			PORTB = lights;
			break;
		case S_reset:
			lights = 0;
			PORTB = lights;
			break;
		default:
			break;	
	}
}
*/
/* //////////////////////// might not need this 
void Out_Tick(){
	switch(out_state){ // transistions
		case O_start:
			break;
		case O_high:
			break;
		case O_low:
			break;
		case O_reset:
			break;
		default:
			break;
	}
	switch(out_state) { // actions
		case O_start:
			break;
		case O_high:
			break;
		case O_low:
			break;
		case O_reset:
			break;
		default:
			break;
	}
}
*/
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // taking input
	DDRB = 0xFF; PORTB = 0x00; // output 
	temp_state = T_start;
	//motion_state = M_start;
	//sound_state = S_start;
	//out_state = O_start;
	
	ADC_init();
	TimerSet(1);
	TimerOn();
	//initUSART(0);
	
	while(1)
	{
		Temp_Tick(); // take temp every 10 sec
		//Motion_Tick(); // take motion every 5 sec
		//Sound_Tick(); // take sound every 5 sec
		//Out_Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------

/* Partner 1 Name & Email: Catherine Lai clai022@ucr.edu
 * Partner 2 Name & Email: Suhas Jagannath sjaga003@ucr.edu
 * Lab Section: 28
 * Assignment: Lab 10 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 
/*
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


*/