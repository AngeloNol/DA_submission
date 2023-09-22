# UNLV CpE 301 Embedded System Design

# OverView
* implemented design assignment and midterm project using a ATMEGA 328P in Microchip Studio, written in C

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

# MidTerm
* Midterm 1 : UART Interface
   - Program the UART Module to interact with the ATmega328pb:
        1. On transmitting the following keys from the host terminal, the following actions will be performed:
        1. On-reboot or ‘h’ key – help screen (list all keys and functionalities)
        2. ‘o’ - turns ON LED at PB5, ‘O’ turns OFF the LED at PB5.
        3. ‘p’ - Blink (on-off) the LED PB3. Choose your own period. Only use timer mode. ‘P’ turns off
        the LED PB3/stops this operation.
        4. ‘f’ - fade the intensity of LED PB1. Choose your own frequency and step resolution. ‘F’ turns
        off the LED PB1/stops this operation.
        5. ‘b’ – reads the status of the switch at PC1. Display the status in terminal. Exit this function
        when there is change of switch status.
        6. Use UART RX interrupt for all of the above operations.

* Midterm 2 : US + Servo Motor
      1. Mount the HC-SR04 Ultrasonic sensor on to the servo motor using the mounting plate/horn. Scan
    the servo motor from 0 – 180 deg. Collect the US distance/raw value continuously during the scan.
    The resolution of scan has to be less that 2.5 deg.
    2. Display your results as a two dimensional, 0 -180 deg distance graph. Update your scan after every
    scan range. 

# KiCad PCB Design
* Design a Atmega 328P with compnents like LED, potentiometer, two push buttons, ICM-20948 sensor



# Components
Software:
* Mircochip Studio
* KiCad
  
Hardware:
* ATMEGA 328P Xplained Mini
* Ardunio Multi-function Shield
* Arduino Prototype Shield
* USB Logic Analyzer
* LCD
* Stepper Motor
* DC Motor
* Servo Motor
* Ultrasonic Sensor
* MPU-6050




# Student Info
* Student Name: Angelo Nolasco
* Student Email: Nolasco@unlv.nevada.edu
* youtube link DAs: https://www.youtube.com/playlist?list=PLQKv7ukC_Zva5RjXT9Pd358gahEjTsdrr 
* youtube link MidTerm: https://www.youtube.com/playlist?list=PLQKv7ukC_ZvZvBtWoRn7U3fXBtwM975KZ


