/*
 * MidTerm2.c
 *
 * Created: 3/31/2022 5:01:02 PM
 * Author : Nolas
 */ 

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)


//the following are definitions for the HC-SR04 sensor
#define trigPin 3
#define echoPin 2

//header files
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//volatile global variables,
//must be volatile as can now change in ISR
static volatile unsigned int echoFlag = 0;	//volatile int , echoFlag = 0.
static volatile unsigned int pulseEnd = 0;	//volatile int , pulseEnd = 0.


void initUSART()
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); //this sets the BAUD rate, must shift UBRROH 8 bits to the right
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);	//sets lower 8 bits of UBRR0
	UCSR0B = (1<<TXEN0); //enables send of data
	UCSR0C = (3<<UCSZ00); //it is set to 8 bit
}


void init()
{
	DDRB = 0xFF;	//set as output
	PORTB = 0x00;	//set as input
	DDRD &= ~(1<<echoPin);	//echoPin is input
	EIMSK |= (1<<INT0);		//interrupt INT0 enabled
	TIMSK2 |= (1<<TOIE2);	//enable overflow interrupt on Timer 2
	EICRA |= (1<<ISC00); // interrupt on rising edge and falling edge
}


void initServo()
{
	DDRB |= (1<<PINB1); // Set pin 1 to output PWM
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);// Set Fast PWM mode 
	TCCR1B |= (1 << WGM13) | (1 << WGM12) |(1 << CS11);//Set pre-scale of 8,non-inverting mode 

	ICR1 = 40000;

}


void signalPulse()
{
	PORTB |= (1<<trigPin);	//set trigPin high
	_delay_us(10); // delay 10us
	PORTB &= ~(1<<trigPin);		//set trigPin low again
}

void putStringUSART(char* stringPtr)	
{
	while (*stringPtr != 0) {	//while the data the pointer points to is not null
		while (!(UCSR0A & (1<<UDRE0)));	//wait to receive data
		UDR0 = *stringPtr;	//UDR0 = dereferenced stringPtr characters
		stringPtr++;	//advance stringPtr by 1
	}
}


void Loop()
{
	int16_t pulseRange = 0;	//counter is 0 initially
	char pulseString[5];		//char array, used for sending string to USART
	char angleString[5];		//char array, for sending angle to USART
	
	//measure from 0 to 180 degrees
	for (int angle = 0; angle <= 180; angle++) {
		
		OCR1A = angle*25;	//OCR1A scales with the angle, multiplied by a constant
		_delay_ms(30);	//30ms delay
		signalPulse();		//call signal pulse
		pulseRange = pulseEnd;	//pulseRange takes the value of TCNT2
		putStringUSART("Distance: ");	
		itoa(pulseRange, pulseString, 10);		
		putStringUSART(pulseString);		
		putStringUSART(" cm, ");	
		putStringUSART("Angle: ");	
		itoa(angle, angleString, 10);		
		putStringUSART(angleString);	
		putStringUSART(".\n");	
		_delay_ms(1);	
	}
	
	//measure from 180 back to 0 degrees
	for (int angle = 180; angle >= 0 ; angle--) {
		OCR1A = angle*25;		//OCR1A scales with the angle, multiplied by a constant
		_delay_ms(30);		//30ms delay
		signalPulse();		//call signal pulse
		pulseRange = pulseEnd;		//pulseRange takes the value of TCNT2
		putStringUSART("Distance: ");	
		itoa(pulseRange, pulseString, 10);		
		putStringUSART(pulseString);		
		putStringUSART(" cm, ");	
		putStringUSART("Angle: ");	
		itoa(angle, angleString, 10);		
		putStringUSART(angleString);	
		putStringUSART(".\n");		
		_delay_ms(1);			
	}
}

int main()
{
	initUSART(); //USART initialization
	init();	//output initialization
	initServo();
	_delay_ms(50);
	sei();		//set external interrupt
	while(1) {
		Loop();	//go to angleLoop
	}
	return 0;
}

ISR(INT0_vect)
{
	
	if(echoFlag == 0) {
		TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);	//1024 pre-scaler
		echoFlag = 1;	//set flag to 1
	}
	else {
		TCCR2B = 0;	//stop timer
		pulseEnd = TCNT2;	//pulse takes values of counter
		TCNT2 = 0;		//reset counter
		echoFlag = 0;		//reset flag
	}
}


ISR(TIMER2_OVF_vect){
	TCCR2B = 0;	//stop timer
	TCNT2 = 0;	//reset counter
	echoFlag = 0;	//reset flag
}