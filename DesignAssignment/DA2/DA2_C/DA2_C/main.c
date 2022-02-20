/*
 * DA2_C.c
 *
 * Created: 2/17/2022 9:33:22 PM
 * Author : Nolas
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>


int main(void)
{
	/****************
	*Task 2
	**************/
   DDRC &=~(1<<3);//Makes PortC.3 input
   PORTC |=(1<<3); //Pull up
   	
   
   /*************************
   *Task 3
   *************************/
   DDRB |= (1<<4);//PORTB.4 OUTPUT
   PORTB &=~(1<<4);//PORTB.4 OUTPUT
   DDRD &=~(1<<2);//PORTD.2 INPUT
   PORTD |= (1<<2);//PORTD.2 INPUT
   EICRA =0x2;//falling edge
   
   EIMSK = (1<<INT0);
   sei(); //enable interrupt
   
   
   
    while (1) 
    {
		if(!(PINC &(1<<3)))
		{
			DDRB |= (1<<2);		// PORTB.2 OUTPUT "LED ON"
			_delay_ms(1250);	//DELAY FOR 1.25sec	
			DDRB &=~(1<<2);  // PORTB.2 OUTPUT "LED OFF"
			PORTB &=~ (1<<2); // PORTB.2 OUTPUT "LED OFF"
		}
			
    }
}

ISR(INT0_vect)
{
	DDRB |= (1<<4);		// PORTB.4 OUTPUT "LED ON"
	_delay_ms(500);	//DELAY FOR 1.25sec
	DDRB &=~(1<<4);  // PORTB.4 OUTPUT "LED OFF"
	PORTB &=~ (1<<4); // PORTB.4 OUTPUT
}

