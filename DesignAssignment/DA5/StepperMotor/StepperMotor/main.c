/*
 * StepperMotor.c
 *
 * Created: 4/11/2022 1:56:22 PM
 * Author : Nolas
 */ 

#define F_CPU 8000000UL						/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <avr/interrupt.h>
#include <stdio.h>							/* Include std. library file */
#include <util/delay.h>						/* Include Delay header file */

volatile float adc_value;

void ADC_Init()								/* ADC Initialization function */
{
	ADMUX = (0<<REFS1) | (1<<REFS0) | (1<<ADLAR) | (0<<MUX2) | (0<<MUX1) | (1<<MUX0);
	
	ADCSRA = (1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
}

void ADC_Read(char channel)					/* ADC Read function */
{
	unsigned char i = 10;
	adc_value = 0;
	while (i--)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_value += ADC;
	}
	adc_value = adc_value /10;
}



int main(void)
{
	ADC_Init();								/* Initialize ADC */
	DDRD = 0x0F;					/* Make PORTD lower pins as output */	
	PORTC |= (1<<0);		//set my ADC port
	DDRB = (1<<1);				//set PORTB1

	ICR1 = 4999;		//set as TOP
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1);//set CTC mode
	TCCR1A |= (1<<WGM11);//set CTC mode
	TCCR1B |= (1<<WGM12) | (1<<WGM13);//set CTC mode
	TCCR1B |= (1<<CS10) | (1<<CS11);//set prescaler to 64

	while(1)
	{
		ADC_Read(0);//calls the ADC read
		_delay_ms(50);//wait 
		
		
		PORTD = 0x66;
		_delay_ms(50);
		PORTD = 0xCC;
		_delay_ms(50);
		PORTD = 0x99;
		_delay_ms(50);
		PORTD = 0x33;
		_delay_ms(50);
		
		//depending of the potentiometer adc conversion it set another delay
		if((adc_value >= 973) && (adc_value < 1024)){
			_delay_ms(10);
		}
		else if((adc_value < 972) && (adc_value >= 768)){
			_delay_ms(10);
		}
		else if((adc_value < 767) && (adc_value >= 51)){
			_delay_ms(10);
		}
		else{
			
			PORTD = 0x00;
			_delay_ms(10);
		}
	}
	return 0;
}

