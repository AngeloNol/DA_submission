/*
 * DA6I2C.c
 *
 * Created: 4/20/2022 12:39:42 PM
 * Author : Nolas
 */ 


#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "i2cmaster.h"

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) -1)

#define ICM20948	(0x68<<1)     // (1101001 << 1) I2C slave address when AD0=1
#define WHO_AM_I	 0x00
#define PWR_MGMT_1	 0x01
#define ACCEL_XOUT_H 0x2D
#define ACCEL_YOUT_H 0x2F
#define ACCEL_ZOUT_H 0x31
#define GYRO_XOUT_H	 0x33
#define GYRO_YOUT_H	 0x35
#define GYRO_ZOUT_H	 0x37
#define AK09916_ADDRESS  0x0C 
#define AK09916_XOUT_H   0x12
#define AK09916_YOUT_H   0x14
#define AK09916_ZOUT_H   0x16
#define I2C_MST_CTRL   0x01
#define USER_CTRL         0x03 
#define I2C_SLV0_ADDR      		0x03
#define I2C_SLV0_REG    0x04
#define I2C_SLV0_D0 0x06
#define AK09916_CNTL_3 0x32
#define AK09916_HXL 0x11
#define AK09916_WHO_AM_I_1 0x4809
#define AK09916_WHO_AM_I_2 0x0948
#define AK09916_WIA_1 0x00
#define AK09916_CNTL_2 0x31
#define I2C_SLV0_CTRL 0x05
#define EXT_SLV_SENS_DATA_00 0x38

#define DEVICE_ID	 0xEA

uint8_t raw;
uint16_t bigraw;

uint16_t accel_x,accel_y,accel_z ;
uint16_t gyro_x,gyro_y,gyro_z;


void USART_init(unsigned int ubrr);
void USART_tx_string(char * data);



void ICM20948_writereg(uint8_t reg, uint8_t val);
	
/* functions i modified*/
uint16_t ICM20948_readreg16(uint8_t reg);
void ICM20948_Init(void);
void ICM20948_InitMAG(void);
void ICM20948_verify_whoami(void);
void ICM_write(uint8_t reg, uint8_t data);

char buffer[30], myfloat[5];



int main(void)
{
	 float X_a,y_a,z_a;
	  float X_g,y_g,z_g;
	  uint16_t datam;

	
	PORTC |= (1<<5) | (1<<4);			// enable pull ups for TWI pins
	

	i2c_init();							// initialize TWI
	USART_init(BAUD_PRESCALLER);		// initialize USART
	USART_tx_string("UART Connected!\r\n");
    
	
	
	ICM_write(0x7F, 0x20);    // select User Bank 2
	ICM_write(0x01, 0x29);    // set gyro rate for 250 with LPF of 17Hz
	ICM_write(0x00, 0x0A);    // set gyroscope sample rate for 100Hz
	ICM_write(0x14, 0x15);    // set accelerometer low pass filter to 136Hz and the rate to 8G
	ICM_write(0x11, 0x0A);    // set accelerometer rate to 100hz


	ICM20948_Init();				// change clksel on icm
	ICM20948_verify_whoami();			// verify we are connected
	MAG_enread(AK09916_HXL, 0x08);
	ICM20948_InitMAG();
	MAG_write(AK09916_CNTL_3, 0x01);
	ICM_write(PWR_MGMT_1, 0x80);
	
	MAG_write(AK09916_CNTL_2, 0x08);
	
	_delay_ms(200);

	while(1){
		accel_x = ICM20948_readreg16(ACCEL_XOUT_H) ;
		accel_y = ICM20948_readreg16(ACCEL_YOUT_H);
		accel_z = ICM20948_readreg16(ACCEL_ZOUT_H) ;
		gyro_x = ICM20948_readreg16(GYRO_XOUT_H) ;
		gyro_y = ICM20948_readreg16(GYRO_YOUT_H)  ;
		gyro_z = ICM20948_readreg16(GYRO_ZOUT_H)  ;
		 datam = ICM20948_readreg16(EXT_SLV_SENS_DATA_00);
		
		X_a = accel_x / 16384;
		y_a = accel_y / 16384;
		z_a = accel_z / 16384;
		
		X_g = gyro_x / 131;
		y_g = gyro_y / 131;
		z_g = gyro_z / 131;

		
		dtostrf( X_a, 3, 2, myfloat );
		snprintf(buffer,sizeof(buffer),"ACCEL X: %s g\t",myfloat);
		USART_tx_string(buffer);
		
		dtostrf( y_a, 3, 2, myfloat );
		snprintf(buffer,sizeof(buffer),"ACCEL Y: %s g\t",myfloat);
		USART_tx_string(buffer);
		
		dtostrf( z_a, 3, 2, myfloat );
		snprintf(buffer,sizeof(buffer),"ACCEL Z: %s g\t",myfloat);
		USART_tx_string(buffer);
		
		USART_tx_string("\r\n");
		
		dtostrf( X_g, 3, 2, myfloat );
		snprintf(buffer,sizeof(buffer),"GYRO X:  %s g\t",myfloat,0xF8);
		USART_tx_string(buffer);
		
		dtostrf( y_g, 3, 2, myfloat );
		snprintf(buffer,sizeof(buffer),"GYRO Y:  %s g\t",myfloat,0xF8);
		USART_tx_string(buffer);
		
		dtostrf( z_g, 3, 2, myfloat );
		snprintf(buffer,sizeof(buffer),"GYRO Z:  %s g\t",myfloat,0xF8);
		USART_tx_string(buffer);
		
		dtostrf( datam, 3, 2, myfloat );
		snprintf(buffer,sizeof(buffer),"MAG: %s g\t",myfloat);
		USART_tx_string(buffer);

		USART_tx_string("\r\n");
		_delay_ms(200); 
	}
}

void USART_init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_tx_string(char * data){
	while((*data != '\0')){
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

/* Ensure we are talking to ICM, print WHO_AM_I to terminal, should be 0xEA or 234 */
void ICM20948_verify_whoami(void){
	uint8_t who_am_i = 0;
	i2c_start(ICM20948+I2C_WRITE);		// 68 << 1 = D0
	i2c_write(WHO_AM_I);				// select who_am_i
	i2c_stop();							// halt i2c
	i2c_start(ICM20948+I2C_READ);		// D0 + 1(TWI READ)
	who_am_i = i2c_readNak();			// save to variable
	snprintf(buffer,sizeof(buffer),"DEVICE ID: %02x\r\n",who_am_i);
	USART_tx_string(buffer);
	i2c_stop();
}

void MAG_write(uint8_t reg, uint8_t val){
	ICM_write(I2C_SLV0_ADDR, AK09916_ADDRESS);
	ICM_write(I2C_SLV0_REG, reg);
	ICM_write(I2C_SLV0_D0, val);
}

void ICM_write(uint8_t reg, uint8_t data){
	i2c_start(ICM20948+I2C_WRITE);    // start I2C for writing 0xD0
	i2c_write(reg);        // write register address to read
	i2c_write(data);    // write data to be saved to register
	i2c_stop();            // stop I2C
}
/* Change clksel to use best available clock source  */
void ICM20948_Init(void){
	i2c_start(0xD0);					// select ICM20948 (0x68<<1)+0
	i2c_write(0x06);					// select pwr_mgmt_1
	i2c_write(0x01);					// set bit 1
	i2c_stop();
}

ICM20948_InitMAG(void){
	i2c_start(AK09916_ADDRESS);
	ICM_write(0x7F, 0x00);

	
	
}

/* modified to writes to and reads from reg+1 */
uint16_t ICM20948_readreg16(uint8_t reg)
{
	i2c_start(ICM20948+I2C_WRITE);		// set device address and write mode
	i2c_write(reg);						// ACCEL_XOUT
	i2c_start(ICM20948+I2C_READ);		// set device address and read mode
	raw = i2c_readNak();                // read one intermediate byte
	i2c_start(ICM20948+I2C_WRITE);
	i2c_write(reg + 1);
	i2c_start(ICM20948+I2C_READ);
	bigraw = (raw<<8) | i2c_readNak();  // read last byte
	i2c_stop();
	return bigraw;
}

void MAG_enread(uint8_t reg, uint8_t num){
	ICM_write(I2C_SLV0_ADDR, AK09916_ADDRESS|0x80);
	ICM_write(I2C_SLV0_REG, reg);
	ICM_write(I2C_SLV0_CTRL, 0x80 );
}