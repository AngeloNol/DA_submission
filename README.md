# UNLV CpE 301 Embedded System Design, implemented design assignment and midterm project using a ATMEGA 328P in Microchip Studio

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

 * DA4 - The goal of the assignment is to develop the above code to do the following:
    1. Write an AVR C program to measure the distance using the HC-SR04 ultrasonic module.
    2. Use the Timer Capture function to determine the ECHO capture pulse.
    3. Display the results every 1 sec on the serial terminal.

 * DA5 - The goal of the assignment is to develop the above code to do the following:
    1. Write an AVR C program to control the speed of the DC Motor using a potentiometer connected to PC0.
    2. Write an AVR C program to control the speed of the Stepper Motor using a potentiometer connected to PC0. Use a timer in CTC mode to control the delay.
    3. Write an AVR C program to control the position of the Servo Motor using a potentiometer connected to PC0. When pot value is 0 the servo is at position 0 deg. and when pot value is max (approx. 5V) 
       the servo is at position 180 deg.

 * DA6 - The goal of the assignment is to develop the above code to do the following:
    1. Interface the provided ICM-20948 9-DOF IMU Sensor to the ATmega328pb using the I2C
    interface. Using the earlier developed code for UART, display the accelerometer, gyro, and magnetometer data to the UART Terminal.
    2. Apply Simple averaging algorithm to smooth the values to the accelerometer, gyro, and magnetometer data. Plot the above nine values as graphs.
    3. Apply Complementary to the accelerometer, gyro, and magnetometer data to determine the roll, pitch, and yaw of the sensor orientation. Plot the above three values as graphs.



Student Name: Angelo Nolasco

Student #: 5005497011

Student Email: Nolasco@unlv.nevada.edu

