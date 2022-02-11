;
; DA1.asm
;
; Created: 2/10/2022 3:27:15 PM
; Author : Angelo Nolasco
;


.include <m328pbdef.inc>

.set EEMWE = EEMPE
.set EEWE = EEPE
.EQU SUM = 0x406

.def num1_H = R18 ; define upper byte of number 1 as r18
.def num1_L = R19 ; define lower byte of number 1 as r19
.def num2_H = R20 ; define upper byte of number 2 as r20
.def num2_L = R21 ; define lower byte of number 2 as r21
.def num10 = R23  ;define number of time the number will be add as r23




.ORG 0 ;ROM starting at 0
	;part of task 3
	LDI R22,HIGH(RAMEND)
	OUT SPH,R22
	LDI R22,LOW(RAMEND)
	OUT SPL,R22

	;task 1, store 16-bit number 0x1234 into the SRAM location 0x402
	LDI R16, 0x34 ;R16 = 0x34
	LDI XL,LOW(0x402) ;load the low byte of X with value 0x02
	LDI XH,HIGH(0x402) ;load the high byte of X with value 0x4
	ST X+, R16 ;copy R16 to memory location X
	LDI R16, 0x12 ;R16 = 0x12
	ST X+, R16 ;copy R16 to memory location X

	;task 2, store 16-bit number 0x5678 into the SRAM location 0x410
	LDI R17, 0x56 ;R17 = 0x56
	LDI XL,LOW(0x410);load the low byte of X with value 0x10
	LDI XH,HIGH(0x410) ;load the high byte of X with value 0x4
	ST X+, R17;copy R17 to memory location X
	LDI R17, 0x78 ;R17 = 0x78
	ST X+, R17 ;copy R17 to memory location X

	;task 3,need to add 0x1234 and 0x5678, then store into the EEPROM
	LDI num1_L, 0x34 ;num1_L = 0x34
	LDI num2_L, 0x56 ;num1_L = 0x56
	LDI num1_H, 0x12 ;num1_H = 0x12
	LDI num2_H, 0x78 ;num1_H = 0x78
	ADD num1_L,num2_L ;adding the lower bytes of number
	ADC num1_H,num2_H  ;adding the upper bytes of number

	LDI XH,HIGH(0x20) ;load the high byte of X with value 0x0
	LDI XL,LOW(0x20) ;load the low byte of X with value 0x2
	mov R22,num1_L
	CALL STORE_IN_EEPROM
	LDI XH,HIGH(0x21);load the high byte of X with value 0x1
	LDI XL,LOW(0x21);load the low byte of X with value 0x2
	mov R22,num1_H
	CALL STORE_IN_EEPROM

	;task 4, store 16 bit numbers into 0x0910 then retrieve to 0x500 SRAM and add the 10 number, store it to 0x406
	LDI R25,0x00 ;R25 =0x00
	LDI num10,0xA ;num10 = 0xA
	LDI XL,LOW(0x500) ;load the low byte of X with value 0x00
	LDI XH,HIGH(0x500) ;load the high byte of X with value 0x5
	LDI ZH,HIGH(XSQR_TABLE<<1) ; look-up table high-byte addr
	LDI ZL,LOW(XSQR_TABLE<<1) ; look-up table low-byte addr
	;it loop until all the data from flash to SRAM loaction 0x500
	L1:LPM R24,Z+ ;read the table, then increment Z
	   ST X+, R24 ;store R24 in RAM and inc X
	   ADD R25,R24 ;add 24 to r25
	   dec num10 ; decrement num10 by 1
	   BRNE L1 ;check if num10 != 0, then jump to L1
	   STS SUM,R25 ; save the sum of r25 in location 0x406


HERE:
	RJMP HERE
;part of task 3
STORE_IN_EEPROM:
	SBIC EECR, EEPE
	RJMP STORE_IN_EEPROM
	OUT EEARH,XH
	OUT EEARL,XL
	OUT EEDR,R22
	SBI EECR,EEMPE
	SBI EECR,EEPE
	RET

;part of task 4
.ORG 0x0910
	XSQR_TABLE:
	.DB 0,2,6,9,13,15,21,25,30,36