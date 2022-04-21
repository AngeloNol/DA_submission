/*
 * Servo_motor.c
 *
 * Created: 4/20/2022 12:26:20 PM
 * Author : Nolas
 */ 

#define F_CPU 8000000UL						/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <avr/interrupt.h>
#include <stdio.h>							/* Include std. library file */
#include <util/delay.h>						/* Include Delay header file */

/*************Function Protype*********************/
void ADC_Init();
int ADC_Read(char);
/*************************************************/



int main(void)
{
	ADC_Init();
	DDRB |= (1 << PB1);    // turn On the Pull-up
	
	TCNT1 = 0;
	ICR1 = 2499;
	

	
	TCCR1A |= (1<<WGM11)|(1<<COM1A1);
	TCCR1B |= (1<<WGM12)|(1<<WGM13)| (1<<CS10)|(1<<CS11);/* Set Fast PWM with Fosc/64 Timer0 clock */
	while (1)
	{
		OCR1A = (ADC_Read(0)/4.35);				/* Read ADC and map it into 0-255 to write in OCR0 register */
	}

}



void ADC_Init(){
	DDRC = 0x00;							/* Make ADC port as input */
	ADCSRA = 0x87;							/* Enable ADC, with freq/128  */
	ADMUX = 0x40;					/*set REFS0 to AVcc with external capacitor*/
}

int ADC_Read(char channel){
	ADMUX = 0x40 | (channel & 0x07);		/* set input channel to read */
	ADCSRA |= (1<<ADSC);					/* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));			/* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);					/* Clear interrupt flag */
	_delay_us(1);							/* Wait a little bit */
	return ADCW;
	
}


