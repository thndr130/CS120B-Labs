/*
 * clai022_Lab8_Part1.c
 *
 * Created: 4/30/2019 5:50:20 PM
 * Author : akiya
 */ 

#include <avr/io.h>

unsigned short value = 0;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

void tick(){
	value = ADC;
	case one:
		break;
	case two:
		break;
	case three:
		break;
	case four:
		break;
	case five:
		break;
	case six:
		break;
	case seven:
		break;
	case eight:
		break;
	case nine:
		break;
	case ten:
		break;
}


int main(void)
{
    /* Replace with your application code */
	ADC_init();
    while (1) 
    {
    }
}

