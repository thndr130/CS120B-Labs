/*	Partner 1 Name & E-mail: Catherine Lai clai022@ucr.edu
 *	Partner 2 Name & E-mail: Suhas Jagannath sjaga003@ucr.edu
 *	Lab Section: 28
 *	Assignment: Lab 8 Exercise 1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/* Part 1
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
	
    while (1) 
    {
		adcResult = ADC;
		PORTB = (char)(adcResult & 0x00FF);
		PORTD = (char)(adcResult >> 8);
    }
	return 1;
}
*/

#include <avr/io.h>
#include "SparkFun_RHT03.h"


/*
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

*/

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
		/*
		adcResult = PINA&0x01;
		PORTB = adcResult;
		*/
	}
	return 1;
}

// room temp is min = 1+2+4+8+16+256+518 = 805



