;
; DA2_asm.asm
;
; Created: 2/16/2022 9:12:53 AM
; Author : Nolas
;


	JMP main
.ORG 0x02;location for external interrupt 0
	JMP EX0_ISR

	;task 1
	CALL DELAY_250ms
	;task 2
	SBI PORTC,3	            ;set PORTC.3 to pull-up resistor


	;task 3
	LDI R20,HIGH(RAMEND)
	OUT SPH,R20
	LDI R20,LOW(RAMEND)
	OUT SPL,R20;initialize stack

main:
	;task 2
	CBI DDRB,3
	CBI DDRB,4	
	CBI DDRB,5	
	SBIS PINC,3              ;if button at PORTC.3 pressed
	RJMP button              ; then it jump to label button


	;task 3
	LDI R20,0x2;make INT0 falling edge triggered
	STS EICRA,R20
	SBI DDRB,4;PORTB.1 = output
	SBI PORTD,2;pull-up activated
	LDI R20,1<<INT0;enable INT0
	OUT EIMSK,R20
	SEI;enable interrupts



RJMP main

button:
	;task 2		
	SBI DDRB, 2				 ;set PORTB.2 output ‘LED ON”	 					 
	CALL DELAY_1250ms		 ;CALL DELAY_1250ms		
	CBI DDRB, 2				 ;clear PORTB.2 output		
	CBI PORTB, 2			 ; turn off LED at PORTB.2	

	SBIS PINC, 3		     ;if button at PORTC.3 pressed			
	RJMP button				 ; then it jump to label button

RJMP main


;task 3
EX0_ISR:
	SBI DDRB, 1				 ;set PORTB.2 output ‘LED ON”	 
	CALL DELAY_500ms
	CBI DDRB, 1				 ;clear PORTB.2 output		
	CBI PORTB, 1			 ; turn off LED at PORTB.2	
	RETI


	
;task 1
DELAY_250ms: ; 0.25 second delay
 LDI R20, 4
D0:LDI R21,100
D1:LDI R22, 124
	NOP
	NOP
D2:NOP
 NOP
 DEC R22
 BRNE D2
 DEC R21
 BRNE D1
 DEC R20
 BRNE D0
 RET

 ;task 2
DELAY_1250ms: ; 1.25 seconds
 LDI R20, 125
B0:
 LDI R21, 125
B1:
 LDI R22, 255
B2:
 NOP
 NOP
 DEC R22
 BRNE B2
 DEC R21
 BRNE B1
 DEC R20
 BRNE B0
 RET

  ;task 3
DELAY_500ms:  ;0.5 second delay
 LDI R20, 4
C0:
  LDI R21,200
C1:
  LDI R22, 124
C2:
 NOP
 NOP
 DEC R22
 BRNE C2
 DEC R21
 BRNE C1
 DEC R20
 BRNE C0
 RET

