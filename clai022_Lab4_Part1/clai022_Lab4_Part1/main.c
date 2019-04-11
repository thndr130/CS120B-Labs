/*
 * clai022_Lab4_Part1.c
 *
 * Created: 4/10/2019 5:21:36 PM
 * Author : Catherine Lai
 */ 

#include <avr/io.h>
//#include "RIMS.h"
enum LED_States {LED_Start, LED_S0, LED_S1} LED_State;


void TickFct_Latch(){ // state transistions
	switch(LED_State){ 
		case LED_Start:
			LED_State = LED_S0;
			break;
		case LED_S0:
			if (!PINA){
				while(!PINA)
				{
					LED_State = LED_S0;
				}
			}
			else if (PINA){
				while(PINA){
					LED_State = LED_S1;
				}
			}
			break;
		case LED_S1:
			if (!PINA){
				while(!PINA){
					LED_State = LED_S1;
				}
			}
			else if (PINA){
				while(PINA){
					LED_State = LED_S0;
				}
			}
			break;
		default:
			LED_State = LED_Start;
			break;
	}
	
	switch(LED_State){ // what changes in the states
		case LED_S0:
			PORTB = 0x01;
			break;
		case LED_S1:
			PORTB = 0x02;
			break;
		default:
			break;
		
	}
}



int main(void)
{
	DDRA = 0x00; PORTA = 0XFF;
	DDRB = 0xFF; PORTB = 0x00;
		
    LED_State = LED_Start;
	
	while(1){
		TickFct_Latch();
	}
	return 0;
}

