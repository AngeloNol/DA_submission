/*
 * DC_Motor.c
 *
 * Created: 4/6/2022 9:55:55 AM
 * Author : Nolas
 */ 

#define F_CPU 8000000UL						
#include <avr/io.h>							
#include <avr/interrupt.h>
#include <stdio.h>							
#include <util/delay.h>						

/*************Function Protype*********************/
void ADC_Init();	
int ADC_Read(char);
/*************************************************/



int main(void)
{
	DDRD &= ~(1<<PD2);						/* Make INT0 pin as Input */
	PORTD |= (1 << PD2);    // turn On the Pull-up
	DDRD |= (1<<PD6);						/* Make OC0 pin as Output */
	EICRA |= (1 << ISC01);    // set INT0 to trigger to falling edge
	EIMSK |= (1 << INT0);     // Turns on INT0
	sei();  /*enable interrupt*/
	ADC_Init();  /*calls the ADC_int function*/
	TCNT0 = 0;								/* Set timer0 count zero */
	TCCR0A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0A1);/*set to CTC mode, and fast PWM*/
	TCCR0B |= (1<<CS00)|(1<<CS02);/* Set Fosc/64 Timer0 clock */
	
    while (1) 
    {
		OCR0A = (ADC_Read(0)/4);				/* Read ADC and map it into 0-255 to write in OCR0 register */
	}

}

ISR(INT0_vect)
{
	TCCR0B |= (0<<CS00)|(0<<CS01);/* Set Fast PWM with Fosc/64 Timer0 clock */
	_delay_us(5000);							/* Software de-bouncing control delay */
	TCCR0B |= (1<<CS00)|(1<<CS02);/* Set Fast PWM with Fosc/64 Timer0 clock */

}

void ADC_Init(){
	DDRC = 0x00;							/* set ADC port to PINC0 */
	ADCSRA = 0x87;							/* Enable ADC, with freq/128  */
	ADMUX = 0x40;							/*set REFS0 to AVcc with external capacitor*/
}

int ADC_Read(char channel){
	ADMUX = 0x40 | (channel & 0x07);		/* set input channel to read */
	ADCSRA |= (1<<ADSC);					/* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));			/* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);					/* Clear interrupt flag */
	_delay_us(1);							/* Wait a little bit */
	return ADCW;
	
}

