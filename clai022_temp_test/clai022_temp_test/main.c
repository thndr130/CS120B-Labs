/*
 * clai022_temp_test.c
 *
 * Created: 5/26/2019 1:09:18 AM
 * Author : akiya
 */ 

/*	Partner 1 Name & E-mail: Catherine Lai clai022@ucr.edu
 *	Partner 2 Name & E-mail: Suhas Jagannath sjaga003@ucr.edu
 *	Lab Section: 28
 *	Assignment: Lab 8 Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


// 76F or 25C is 176 in binary
//125C max = 1+2+4+8+64+256 = 335 
//1+2+4+32+128 = 167 @ 50F

int main(void)
{
	ADC_init();
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned short adcResult;
	
	while (1)
	{
		adcResult = ADC;
		PORTB = (char)(adcResult & 0x00FF);
		PORTD = (char)(adcResult >> 8);
	}
	return 1;
}

/*
#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}



int main(void)
{
	ADC_init();
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned short adcResult;
	unsigned char calc;
	
	while (1)
	{
		adcResult = ADC;
		calc = (char) ((adcResult - 500)/10);
		PORTB = calc; //(char)(adcResult & 0x00FF);
		//PORTD = (char)(adcResult >> 8);
	}
	return 1;
}


*/
/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "io.c"
#include <stdio.h>
#include "bit.h"
//#include "SparkFun_RHT03.h"


enum Temp_Sensor {T_start, T_high, T_reset, T_wait} temp_state;
unsigned short T_count = 0;
unsigned long temp = 0;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

	
void Temp_Tick(){
	//reset = ~PINB & 0x02;
	
	switch(temp_state){
		case T_start:
		temp_state = T_wait;
		
		while(!USART_IsSendReady(0));
		USART_Send(0, 0);
		while(!USART_HasTransmitted(0));
		
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
		break;
		case T_wait:
		break;
		case T_high:
		//temp = 1;
		//lights = lights | 0x02;
		//PORTD = lights;
		break;
		case T_reset:
		//temp = 0;
		//lights = 0;
		//PORTD = lights;
		break;
		default:
		break;
	}
	
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	
	temp_state = T_start;
	ADC_init();
	TimerSet(1);
	TimerOn();

    while (1) 
    {
		temp = ADC;
		Temp_Tick();
		while(!TimerFlag) {}
		TimerFlag = 0;	
    }
}
*/
/*
void Temp_Tick(){
	reset = ~PINB & 0x02;
	
	switch(temp_state){
		case T_start:
			temp_state = T_wait;
			
			while(!USART_IsSendReady(0));
			USART_Send(0, 0);
			while(!USART_HasTransmitted(0));
			
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
			break;
		case T_wait:
			break;
		case T_high:
			//temp = 1;
			//lights = lights | 0x02;
			//PORTD = lights;
			break;
		case T_reset:
			//temp = 0;
			//lights = 0;
			//PORTD = lights;
			break;
		default:
			break;
	}
	
}
*/



/* ------------------------------------------------------------------------------------------------------------------------------------------

#include <avr/io.h>
#include "io.c"

enum countStates {init, wait, buttonDec, decrement, buttonRst, reset, buttonInc, increment} state; //All states
unsigned char A0 = 0x00; //Stores the value of PINA bit 0
unsigned char A1 = 0x00; //Stores the value of PINA bit 1
unsigned char value = 0x00;

void tick(void) {
	A0 = ~PINA & 0x01; //Get PA0
	A1 = ~PINA & 0x02; //Get PA1
	
	switch(state){ //Transitions
		case init: //Initial state
			state = wait;
			break;
		case wait:
			if(!A0 && !A1) { //Null transition
				state = wait;
			}
			if(!A0 && A1) {
				state = decrement;
			}
			if(A0 && A1) {
				state = buttonRst;
			}
			if(A0 && !A1) {
				state = increment;
			}
			break;
		case decrement:
			if(!A0 && !A1) {
				state = wait;
			}
			if(value <= 0) { //If PIN is below threshold
				state = wait;
				break;
			}
			if(A0 && A1) {
				state = reset;
			}
			//state = wait;
			break;
		case buttonRst:
			if(A0 && A1) { //If buttons are still pressed
				state = buttonRst;
			}
			if(!A0 && !A1) { //If buttons are not pressed
				state = reset;
			}
			break;
		case reset:
			state = wait;
			break;
		case increment:
			if(!A0 && !A1) {
				state = wait;
			}
			if(value >= 9) { //If PINC is above threshold
				state = wait;
				break;
			}
			if(A0 && A1) {
				state = reset;
			}
			//state = wait;
			break;
		default:
			state = init;
			break;
	}
	
	switch(state) { //Actions
		case init: //Initialize with value 7
			LCD_ClearScreen();
			LCD_WriteData(value + '0');
			break;
		case wait: //Intentionally left blank
			break;
		case buttonDec: //Intentionally left blank
			break;
		case decrement: //Decrement value
			if(value > 0) {
			LCD_ClearScreen();
			value = value - 0x01;
			LCD_WriteData(value + '0');
			}
			break;
		case buttonRst: //Intentionally left blank
			break;
		case reset: //Reset value to 0
			LCD_ClearScreen();
			value = 0x00;
			LCD_WriteData(value + '0');
			break;
		case buttonInc: //Intentionally left blank
			break;
		case increment: //Increment value
			if(value < 9) {
			LCD_ClearScreen();
			value = value + 0x01;
			LCD_WriteData(value + '0');
			}
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	state = -1;
	TimerSet(60);
	TimerOn();
	LCD_init();
	
	while (1)
	{
		tick();
		while(!TimerFlag) {
		}
		TimerFlag = 0;
	}
	return 1;
}


*/