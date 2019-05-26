/*
 * clai022_Lab4_Part2.c
 *
 * Created: 4/10/2019 5:52:22 PM
 * Author : akiya
 */ 
/*
#include <avr/io.h>
enum ID_States {ID_Start, ID_Inc, ID_Dec, ID_Reset, ID_Wait} ID_State;

void TickFct_Latch(){ // state transitions
	
	switch(ID_State){
		case ID_Start:
		if ((PINA & 0x03) == 0x01){
			ID_State = ID_Inc;
		}
		if ((PINA & 0x03) == 0x02){
			ID_State = ID_Dec;
		}
		if ((PINA & 0x03) == 0x03){
			ID_State = ID_Reset;
		}
		else {
			ID_State = ID_Start;
		}
		break;
		case ID_Wait:
		if (((PINA & 0x03) == 0x01)||((PINA & 0x03) == 0x02)){
			ID_State = ID_Wait;
		}
		if ((PINA & 0x03) == 0x03){
			ID_State = ID_Reset;
		}
		else {
			ID_State = ID_Start;
		}
		break;
		case ID_Inc:
		ID_State = ID_Wait;
		break;
		case ID_Dec:
		ID_State = ID_Wait;
		break;
		case ID_Reset:
		if (((PINA & 0x03) == 0x01)||((PINA & 0x03) == 0x02)){
			ID_State = ID_Reset;
		}
		else{
			ID_State = ID_Start;
		}
		break;
		default:
		break;
	}

	switch(ID_State){
		case ID_Start:
		
		break;
		case ID_Wait:
		
		break;
		case ID_Inc:
		if (PORTC < 0x09){ 
			PORTC = PORTC + 0x01; 
		}
		else if (PORTC >= 0x09) { 
			PORTC = 0x09; 
		}
		break;
		case ID_Dec:
		if (PORTC > 0x00){ 
			PORTC = PORTC - 0x01; 
		}
		else if (PORTC <= 0x00) { 
			PORTC = 0x00; 
		}
		break;
		case ID_Reset:
		PORTC = 0x00;
		break;
	}

}

int main(void)
{
	DDRA = 0x00; PORTA = 0XFF;
	DDRC = 0xFF; PORTC = 0x07;
	
	ID_State = ID_Start;
	
	while(1){
		TickFct_Latch();
	}
	return 0;
}
*/

/*	Partner 1 Name & E-mail: Catherine Lai clai022@ucr.edu
 *	Partner 2 Name & E-mail: Suhas Jagannath sjaga003@ucr.edu
 *	Lab Section: 28
 *	Assignment: Lab 4 Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum countStates {init, wait, buttonDec, decrement, buttonRst, reset, buttonInc, increment} state; //All states
unsigned char A0 = 0x00; //Stores the value of PINA bit 0
unsigned char A1 = 0x00; //Stores the value of PINA bit 1

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
				state = buttonDec;
			}
			if(A0 && A1) {
				state = buttonRst;
			}
			if(A0 && !A1) {
				state = buttonInc;
			}
			break;
		case buttonDec:
			if(PINB <= 0) { //If PIN is below threshold
				state = wait;
				break;
			}
			if(!A0 && A1) { //If button is still pressed
				state = buttonDec;
			}
			if(!A0 && !A1) { //If button is unpressed
				state = decrement;
			}
			break;
		case decrement:
			state = wait;
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
		case buttonInc:
			if(PINB >= 9) { //If PINC is above threshold
				state = wait;
				break;
			}
			if(A0 && !A1) { //If button is still pressed
				state = buttonInc;
			}
			if(!A0 && !A1) { //If button is not pressed
				state = increment;
			}
			break;
		case increment:
			state = wait;
			break;
		default:
			state = init;
			break;
	}
	
	switch(state) { //Actions
		case init: //Initialize with value 7
			PORTB = 0x00;
			break;
		case wait: //Intentionally left blank
			break;
		case buttonDec: //Intentionally left blank
			break;
		case decrement: //Decrement value
			PORTB = PORTB - 0x01;
			break;
		case buttonRst: //Intentionally left blank
			break;
		case reset: //Reset value to 0
			PORTB = 0x00;
			break;
		case buttonInc: //Intentionally left blank
			break;
		case increment: //Increment value
			PORTB = PORTB + 0x01;
			break;
		default:
			break;
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	state = -1;
    while (1) 
    {
		tick();
    }
	return 1;
}

