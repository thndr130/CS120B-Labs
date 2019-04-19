/*	Partner 1 Name & E-mail: Catherine Lai clai022@ucr.edu
 *	Partner 2 Name & E-mail: Suhas Jagannath sjaga003@ucr.edu
 *	Lab Section: 28
 *	Assignment: Lab 4 Exercise 5
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum lockStates {init, wait, buttonSharp, sharp, buttonY, y, buttonX, x, unlock, buttonLock, lock, wrongButton} state;
unsigned char A0 = 0x00; //Stores value of PINA bit 0 "X"
unsigned char A1 = 0x00; //Stores value of PINA bit 1 "Y"
unsigned char A2 = 0x00; //Stores value of PINA bit 2 "#"
unsigned char A7 = 0x00; //Stores value of PINA bit 7 "inside button (ib)"
unsigned char B0 = 0x00; //Stores value of PINB bit 0 "lock"
unsigned char allButtons = 0x00;
enum lockStates sequenceStates[] = {buttonX, buttonY, buttonX}; //Stores the sequence of states
unsigned char sequenceButtons[]; //Stores the sequence of buttons
unsigned char count = 0x00; //Stores the number of elements in the sequence

void tick(void) {
	A0 = (PINA & 0x01);
	A1 = (PINA & 0x02) >> 1;
	A2 = (PINA & 0x04) >> 2;
	A7 = (PINA & 0x80) >> 7;
	B0 = PINB & 0x01;
	
	//Set up button sequence
	sequenceButtons[0] = A0;
	sequenceButtons[1] = A1;
	sequenceButtons[2] = A0;
	

	allButtons = A0 + A1 + A2 + A7; //Stores if all buttons are pressed

	if((allButtons) <= 1) {
		switch(state) { //Transitions
			case init:
				state = wait;
				break;
			case wait:
				count = 0; //index of sequence
				if(!A2) { //If # is not pressed
					state = wait;
				}
				if(A2) { //If # is pressed
					state = buttonSharp;
				}
				if(A7 && B0) { //If ib is pressed and lock is unlocked
					state = buttonLock;
				}
				break;
			case buttonSharp:
				if(A2) { //If # is still pressed
					state = buttonSharp;
				}
				if(!A2) { //If # is not pressed
					state = sharp;
				}
				break;
			case sharp:
				if (!sequenceButtons[count] && ((allButtons) != 0)) { //If button in sequence is not pressed and at least one button is pressed
					state = wrongButton; //reset if wrong combo
				}
				if (!sequenceButtons[count] && ((allButtons) == 0)) { //If button in sequence is not pressed and no buttons are pressed 
					state = sharp;
				}
				if(sequenceButtons[count]) { //If button in sequence is pressed
					state = sequenceStates[count]; //State equals next state in sequence
				}
				if(count == 3) { //If sequence has been completed
					state = unlock;
				}
				break;
			case wrongButton:
				if(!sequenceButtons[count] && ((allButtons) != 0)) { //If button in sequence is not pressed and at least one button is pressed
					state = wrongButton;
				}
				if(!sequenceButtons[count] && ((allButtons) == 0)) { //If button in sequence is not pressed and no buttons are pressed 
					state = wait;
				}
				break;
			case buttonX:
				if(A0) { //If X is pressed
					state = buttonX;
				}
				if(!A0) { //If X is not pressed
					state = x;
				}
				break;
			case x:
				state = sharp;
				count = count + 1; //Increment count			
				break;
			case buttonY:
				if(A1) { //If Y is pressed
					state = buttonY;
				}
				if(!A1) { //If Y is not pressed
					state = y;
				}
				break;
			case y:
				state = sharp;
				count = count + 1; //Increment count
				break;
			case unlock:
				state = wait;
				break;
			case buttonLock:
				if(A7) { //If ib is pressed
					state = buttonLock;
				}
				if(!A7) { //If ib is not pressed
					state = lock;
				}
				break;
			case lock:
				state = wait;
				break;
			default:
				state = init;
				break;
		}
		
		switch(state) { //Actions
			case init:
				PORTC = init;
				PORTB = (PORTB & 0xFD) | 0x00; //initially locked
				break;
			case wait:
				PORTC = wait;
				break;
			case buttonSharp:
				PORTC = buttonSharp;
				break;
			case sharp:
				PORTC = sharp;
				break;
			case wrongButton:
				PORTC = wrongButton;
				break;
			case buttonY:
				PORTC = buttonY;
				break;
			case y:
				PORTC = y;
				break;
			case buttonX:
				PORTC = buttonX;
				break;
			case x:
				PORTC = x;
				break;
			case unlock:
				PORTC = unlock;
				if(PORTB) { //If lock is unlocked
					PORTB = 0x00; //Lock the lock
				}
				else {
					PORTB = (PORTB & 0xFD) | 0x01; //Unlock the lock
				}
				break;
			case buttonLock:
				PORTC = buttonLock;
				break;
			case lock:
				PORTC = lock;
				PORTB = 0x00;
				break;
			default:
				break;
		}
		
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	state = -1;
	while (1)
	{
		tick();
	}
	return 1;
}

