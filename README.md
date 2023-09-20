# Design Assignments
* DA1 - Write, simulate, and demonstrate using Microchip Studio 7 an assembly code for the AVR
ATMEGA328P/PB microcontroller that performs the following functions:
    1. Store a 16-bit number 0x1234 at SRAM location 0x402. Verify the number stored in the
    location.
    2. Store a 16-bit number in 0x5678 at SRAM location 0x410. Verify the number stored in the
    location.
    3. Sum the above two numbers and store the result in EEPROM starting location. Verify the
    number stored in the location.
    4. Store 10 16-bit numbers starting from 0x0910 at Program Memory location using code and
    retrieve them to 0x500 SRAM location using X pointer. Sum the 10 numbers and store the
    sum in SRAM location 0x406.

* DA2 - The goal of the assignment is use GPIO, delays, and Interrupts:
    1. Design a delay subroutine to generate a delay of 0.25 sec.
    2. Connect a switch to PORTC.3 (active high - turn on the pull up transistor) to poll for an event to
    turn on the led at PORTB.2 for 1.25 sec after the event.
    3. Connect a switch to INT0 (PD2 pin) (active high - turn on the pull up transistor) and using an
    interrupt mechanism turn on the led at PORTB.1 for 0.5 sec after the event.
    4. All of the above constitutes for a single task. Verify the delay using simulation and logic analyzer.



Student Name: Angelo Nolasco

Student #: 5005497011

Student Email: Nolasco@unlv.nevada.edu

