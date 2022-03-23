/*
 * midterm1.c
 *
 * Created: 3/23/2022 10:33:06 AM
 * Author : Nolas
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define TRUE 1
#define FALSE 0

#define CHAR_NEWLINE '\n'
#define CHAR_RETURN '\r'
#define RETURN_NEWLINE "\r\n"

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <string.h>
#include <util/delay.h>


/************Global Variables*******************/
// The inputted commands are never going to be
// more than 8 chars long. Volatile for the ISR.
volatile unsigned char data_in[8];
volatile unsigned char command_in[8];

volatile unsigned char data_count;
volatile unsigned char command_ready;

// Variables to hold current settings
unsigned int sensitivity = 223;

volatile unsigned int adc_temp;
char outs[20];
/*********************************************/
/************Function Protype*******************/
void USART0_Put(uint8_t data);
void USART0_PutString(char *ptr);
void copy_command ();
void timer1(void);
void process_command();
void setPWM(uint8_t data);
void Fade_LED(void);
void adc_init(void);
void read_adc(void);
void adc(void);
/*********************************************/
/************Function Implementation*******************/
void USART0_Put(uint8_t data){
	
	//Checking to see if USART TX buffer is empty for new data
	while(!(UCSR0A & (1<<UDRE0)));
	
	//Initiating transfer
	UDR0 = data;

} // USART0_Put()
void USART0_PutString(char *ptr){
	
	while(*ptr){			// Loop until end of string (*s = '\0')
		USART0_Put(*ptr++);	// Send the character and point to the next one
	}
	
} // USART0_PutString()

void copy_command ()
{
	// The USART might interrupt this - don't let that happen!
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		// Copy the contents of data_in into command_in
		memcpy(command_in, data_in, 8);

		// Now clear data_in, the USART can reuse it now
		memset(data_in[0], 0, 8);
	}
}

void timer1(void){//the function use timer1 in CTC mode
	DDRB |= (1 << 3) ; //configure PB3 as output
	TCCR1B = (1 << CS12); //set the pre-scalar as 256
	OCR1A = 46875; //750ms delay
	TCNT1 = 0;
	while (1)
	{
		//If flag is set toggle the led
		while ((TIFR1 & (1 << OCF1A)) == 0); // wait till the timer overflow flag is SET
		PORTB ^= (1 << 3);
		TCNT1 = 0;
		TIFR1 |= (1 << OCF1A) ; //clear timer1 overflow flag
		

	}
	
	
	
}



void setPWM(uint8_t data){
	OCR1A = data;
}

void Fade_LED(void){

	uint8_t brightness;
	
	TCCR1A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0A1);
	TCCR1B = 1;
	DDRB|=(1<<2);
	
	brightness = 200;
	while(1)
	{
		for(brightness = 0; brightness < 255; brightness++){
			setPWM(brightness);
			_delay_ms(1000);
		}
		
		for(brightness = 255; brightness > 0; brightness--){
			setPWM(brightness);
			_delay_ms(1000);
		}
		
	}
	
	
	
}
void adc_init(void)
{
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|    // AVcc - external cap at AREF
	(0<<ADLAR)|    // ADC Left Adjust Result
	(1<<MUX2)|     // Analog Channel Selection Bits
	(1<<MUX1)|     // ADC0 (PC0 PIN23)
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|    // ADC ENable
	(0<<ADSC)|     // ADC Start Conversion
	(0<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // ADC Interrupt Flag
	(0<<ADIE)|     // ADC Interrupt Enable
	(1<<ADPS2)|    // ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
}

void read_adc(void)
{
	unsigned char i = 4;
	adc_temp = 0;
	while (i--)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp / 4;  // Average a few samples
}
void adc(void){
	adc_init();             // Initialize the ADC
	_delay_ms(2000);         // wait a bit
	while(1)
	{
		read_adc();
		snprintf(outs,sizeof(outs),"%3d\r\n", adc_temp);  // print it
		USART0_PutString(outs);
		_delay_ms(2000);        // wait a bit
		
		if(adc_temp > adc_temp){
			exit(0);
		}
	}
}



void process_command()
{
	switch (command_in[0]) {
		case 'h':
		USART0_PutString("Help Screen\n");
		USART0_PutString("‘o’ - turns ON LED at PB5,‘O’ turns OFF the LED at PB5 \n");
		USART0_PutString("‘p’ - Blink (on-off) the LED PB3,‘P’ turns off the LED PB3 \n");
		USART0_PutString("‘f’ - fade the intensity of LED PB1,‘F’ turns off the LED PB1 \n");
		USART0_PutString("‘b’ – reads the status of the switch at PC1 \n");
		break;
		case 'o':
		PORTB &=~(1<<5);
		DDRB &=~(1<<5);//turn off LED PB5
		break;
		case 'O':
		DDRB |= (1<<5);//set PB5 as output
		
		break;
		case 'p':
		
		timer1();//call timer1 function
		break;
		case 'P':
		DDRB &=~(1<<3);  // PORTB.3 OUTPUT "LED OFF"
		PORTB &=~ (1<<3); // PORTB.3 OUTPUT "LED OFF"
		break;
		case 'f':
		
		Fade_LED();//call fade_led function
		break;
		case 'F':
		DDRB &=~(1<<2);//turn off LED PB1
		PORTB &=~(1<<2);
		break;
		case'b':
		adc();
		break;
		
		default:
		break;
	}//end of switch statement
}

/************************************************************/
int main(void)
{
	// Turn on USART hardware (RX, TX)

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	// 8 bit char sizes
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	// Set baud rate
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	// Enable the USART Receive interrupt
	UCSR0B |= (1 << RXCIE0 );

	// Globally enable interrupts
	sei();
	
	
	
	USART0_PutString("‘h’ key – help screen\n");
	USART0_PutString("‘o’ - turns ON LED at PB5,‘O’ turns OFF the LED at PB5\n");
	USART0_PutString("‘p’ - Blink (on-off) the LED PB3,‘P’ turns off the LED PB3\n");
	USART0_PutString("‘f’ - fade the intensity of LED PB1,‘F’ turns off the LED PB1\n");
	USART0_PutString("‘b’ – reads the status of the switch at PC1\n");
	

	while(1) {

		if (command_ready == TRUE) {
			copy_command();
			process_command();

			command_ready = FALSE;
		}

	}
}

ISR(USART0_RX_vect)
{
	// Get data from the USART in register
	data_in[data_count] = UDR0;

	// End of line!
	if (data_in[data_count] == '\n') {
		command_ready = TRUE;
		// Reset to 0, ready to go again
		data_count = 0;
		} else {
		data_count++;
	}
}