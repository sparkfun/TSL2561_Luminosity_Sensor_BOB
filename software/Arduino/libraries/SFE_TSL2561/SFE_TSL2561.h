/*
  SFE_TSL2561 illumination sensor library for Arduino
	
  Mike Grusin
  http://www.sparkfun.com
	
	version 1.0 2013/10/25 initial version
*/

#ifndef SFE_TSL2561_h
#define SFE_TSL2561_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SFE_TSL2561
{
	public:
		SFE_TSL2561(char i2c_address);

		char begin();
			// initialize TSL2561 before use
			// returns 1 if success, 0 if failure (bad component or I2C bus shorted?)
		
		char clearInterrupt(void);
			// command TSL2561 to start a temperature measurement
			// returns n (number of ms to wait) for success, 0 for fail

		char setPowerUp(void);
			// command TSL2561 to start a temperature measurement
			// returns n (number of ms to wait) for success, 0 for fail

		char setPowerDown(void);
			// command TSL2561 to start a temperature measurement
			// returns n (number of ms to wait) for success, 0 for fail
//
		char getStatus(boolean *status);
			// return temperature measurement from previous startTemperature command
			// places returned value in T variable (deg C)
			// returns 1 for success, 0 for fail
//
//		char setGain(boolean gain);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
//		char setManualStart(void);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
//		char setIntegrationTime(unsigned char time);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
//		char setInterruptThresholdLow(unsigned int threshold);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
//		char setInterruptThresholdHigh(unsigned int threshold);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
//		char setInterruptConfig(unsigned char control, unsigned char persist);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
		char getID(char *ID);
			// command TSL2561 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns n (number of ms to wait) for success, 0 for fail

//		char getData0(unsigned int *data);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
//		char getData1(unsigned int *data);
//			// command TSL2561 to start a pressure measurement
//			// oversampling: 0 - 3 for oversampling value
//			// returns n (number of ms to wait) for success, 0 for fail
//
//		char getLux();

	private:
	
		char getReg(char address, char *result);

	
		char readInt(char address, int *value);
			// read an signed int (16 bits) from a TSL2561 register
			// address: TSL2561 register address
			// *value: pointer to signed int for returned value (16 bits)
			// returns 1 for success, 0 for fail, with read value(s) in *value

		char readUInt(char address, unsigned int *value);
			// read an unsigned int (16 bits) from a TSL2561 register
			// address: TSL2561 register address
			// *value: pointer to unsigned int for returned value (16 bits)
			// returns 1 for success, 0 for fail, with read value(s) in *value

		char readBytes(unsigned char *values, char length);
			// read a number of bytes from a TSL2561 register
			// values: array of char with register address in first location [0]
			// length: number of bytes to read back
			// returns 1 for success, 0 for fail, with read bytes in values[] array
			
		char writeBytes(unsigned char *values, char length);
			// write a number of bytes to a TSL2561 register (and consecutive subsequent registers)
			// values: array of char with register address in first location [0]
			// length: number of bytes to write
			// returns 1 for success, 0 for fail
			
		char _i2c_address;

};

#define TSL2561_ADDR_0 0x29 // address with '0' shorted on board
#define TSL2561_ADDR   0x39 // default address
#define TSL2561_ADDR_1 0x49 // address with '1' shorted on board

#define	TSL2561_REG_CONTROL   0x00
#define	TSL2561_REG_TIMING    0x01
#define	TSL2561_REG_THRESH_LL 0x02
#define	TSL2561_REG_THRESH_LH 0x03
#define	TSL2561_REG_THRESH_HL 0x04
#define	TSL2561_REG_THRESH_HH 0x05
#define	TSL2561_REG_INTERRUPT 0x06
#define	TSL2561_REG_DATA_0L   0x0C
#define	TSL2561_REG_DATA_0H   0x0D
#define	TSL2561_REG_DATA_1L   0x0E
#define	TSL2561_REG_DATA_1H   0x0F

#endif
