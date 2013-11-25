/*
  SFE_TSL2561 illumination sensor library for Arduino
	
  Mike Grusin
  http://www.sparkfun.com
	
	version 1.0 2013/10/25 initial version
*/


#include <SFE_TSL2561.h>
#include <Wire.h>
#include <stdio.h>
#include <math.h>

SFE_TSL2561::SFE_TSL2561(char i2c_address)
{
	_i2c_address = i2c_address;
//	Wire.begin();
}

char SFE_TSL2561::begin()
{
	Wire.begin();
		
	return(1);
}

//value is address of a short (16-bit) int
char SFE_TSL2561::readInt(char address, int *value)
{
	unsigned char data[2];

	data[0] = address;
	if (readBytes(data,2))
	{
		*value = (((int)data[0]<<8)|(int)data[1]);
		//if (*value & 0x8000) *value |= 0xFFFF0000; // sign extend if negative
		return(1);
	}
	*value = 0;
	return(0);
}

//value is address of a short (16-bit) int
char SFE_TSL2561::readUInt(char address, unsigned int *value)
{
	unsigned char data[2];

	data[0] = address;
	if (readBytes(data,2))
	{
		*value = (((unsigned int)data[0]<<8)|(unsigned int)data[1]);
		return(1);
	}
	*value = 0;
	return(0);
}


//value is address of a short (16-bit) int
char SFE_TSL2561::writeUInt(char address, unsigned int value)
{
	unsigned char data[3];

	data[0] = address;
	data[1] = (unsigned char)(value & 0x00FF);
	//Serial.println(data[1],HEX);
	data[2] = (unsigned char)((value >> 8) & 0x00FF);
	//Serial.println(data[2],HEX);
	if (writeBytes(data,3))
		return(1);

	return(0);
}


//values is an array of char, first entry should be the register to read from
//subsequent entries will be filled with return values
char SFE_TSL2561::readBytes(unsigned char *values, char length)
{
	char x;

	Wire.beginTransmission(_i2c_address);
	Wire.write(values[0]);
	if (Wire.endTransmission(false) == 0);
	{
		Wire.requestFrom(_i2c_address,length);
		while(Wire.available() != length) ; // wait until bytes are ready
		for(x=0;x<length;x++)
		{
			values[x] = Wire.read();
		}
		return(1);
	}
	return(0);
}

//value is an array of char, first entry should be the register to write to
//subsequent entries will be values to write to that register
char SFE_TSL2561::writeBytes(unsigned char *values, char length)
{
	char x;
	
	Wire.beginTransmission(_i2c_address);
	Wire.write(values,length);
	if (Wire.endTransmission(true) == 0)
		return(1);

	return(0);
}

//char SFE_TSL2561::getReg(char address, char *value)
//{
//  Wire.beginTransmission(_i2c_address);      // i2c address (use 7-bit address, wire library will modify for read/write)
//  Wire.write((address & B00001111) | B10000000);                   // register to read (0x03 = first result register, see datasheet)
//	if (Wire.endTransmission() == 0)
//	{
//		Wire.requestFrom(_i2c_address, 1);      // read next six registers (0x03 to 0x08, see datasheet)
//		if (Wire.available() == 1)              // make sure read was successful
//		{
//			value[0] = Wire.read();    // read out six bytes = three readings (x,y,z)
//			return(1);
//		}
//	}
//	return(0);
//}


char SFE_TSL2561::clearInterrupt(void)
{
	unsigned char data[1];
	data[0] = 0xC0;
	if (writeBytes(data,1))
		return(1);
	else
		return(0);
}


char SFE_TSL2561::setPowerUp(void)
{
	unsigned char data[2];
	data[0] = 0x80; data[1] = 0x03;
	if (writeBytes(data,2))
		return(1);
	else
		return(0);
}

char SFE_TSL2561::setPowerDown(void)
{
	unsigned char data[2];
	data[0] = 0x80; data[1] = 0x00;
	if (writeBytes(data,2))
		return(1);
	else
		return(0);
}

char SFE_TSL2561::getStatus(boolean &status)
{
	unsigned char data[1];
	data[0] = 0x80;
	if (readBytes(data,1))
	{
		status = ((data[0] & 0x03) == 0x03);
		return(1);
	}
	else
		return(0);
}


char SFE_TSL2561::setGain(boolean gain)
{
	unsigned char data[2], timing;
	
	// Get timing register 0x01

	data[0] = 0x81;
	if (readBytes(data,1))
	{
		timing = data[0];

		if (gain)
			timing |= 0x10;
		else
			timing &= ~0x10;

		data[0] = 0x81; data[1] = timing;

		if (writeBytes(data,2))
			return(1);
	}
	return(0);
}


char SFE_TSL2561::manualStart(void)
	// command TSL2561 to start a pressure measurement
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	unsigned char data[2], timing;
	
	// Get timing register 0x01

	data[0] = 0x81;
	if (readBytes(data,1))
	{
		timing = data[0];

		timing |= 0x08;

		data[0] = 0x81; data[1] = timing;

		if (writeBytes(data,2))
			return(1);
	}
	return(0);
}


char SFE_TSL2561::manualStop(void)
	// command TSL2561 to start a pressure measurement
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	unsigned char data[2], timing;
	
	// Get timing register 0x01

	data[0] = 0x81;
	if (readBytes(data,1))
	{
		timing = data[0];

		timing &= ~0x08;

		data[0] = 0x81; data[1] = timing;

		if (writeBytes(data,2))
			return(1);
	}
	return(0);
}


char SFE_TSL2561::setIntegrationTime(unsigned char time)
	// time = 0 to 3
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	unsigned char data[2], timing;
	
	// Get timing register 0x01

	data[0] = 0x81;
	if (readBytes(data,1))
	{
		timing = data[0];

		timing &= ~0x03;
		timing |= (time & 0x03);

		data[0] = 0x81; data[1] = timing;

		if (writeBytes(data,2))
			return(1);
	}
	
	return(0);
}


char SFE_TSL2561::setInterruptThresholdLow(unsigned int threshold)
	// time = 0 to 3
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	if (writeUInt(0x82,threshold))
		return(1);
		
	return(0);
}


/*
		char setInterruptThresholdHigh(unsigned int threshold);
			// command TSL2561 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns n (number of ms to wait) for success, 0 for fail

		char setInterruptConfig(unsigned char control, unsigned char persist);
			// command TSL2561 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns n (number of ms to wait) for success, 0 for fail
*/

char SFE_TSL2561::getID(char &ID)
{
	unsigned char data[1];
	
	data[0] = 0x8A;
	if (readBytes(data,1))
	{
		ID = data[0];
		return(1);
	}
	
	return(0);
}

/*
char SFE_TSL2561::getID(char *ID)
{
	if (getReg(0x0A,ID))
	return(1);
	else
	return(0);
}
*/
/*
			// command TSL2561 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns n (number of ms to wait) for success, 0 for fail

		char getData0(unsigned int *data);
			// command TSL2561 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns n (number of ms to wait) for success, 0 for fail

		char getData1(unsigned int *data);
			// command TSL2561 to start a pressure measurement
			// oversampling: 0 - 3 for oversampling value
			// returns n (number of ms to wait) for success, 0 for fail

		char getLux();



char SFE_TSL2561::startTemperature(void)
{
	unsigned char data[2], result;
	
	data[0] = TSL2561_REG_CONTROL;
	data[1] = TSL2561_COMMAND_TEMPERATURE;
	result = writeBytes(data, 2);
	if (result) // good write?
		return(5); // return the delay in ms (rounded up) to wait before retrieving data
	else
		return(0); // or return 0 if there was a problem communicating with the BMP
}

char SFE_TSL2561::startPressure(char oversampling)
{
	unsigned char data[2], result, delay;
	
	data[0] = TSL2561_REG_CONTROL;

	switch (oversampling)
	{
		case 0:
			data[1] = TSL2561_COMMAND_PRESSURE0;
			delay = 5;
		break;
		case 1:
			data[1] = TSL2561_COMMAND_PRESSURE1;
			delay = 8;
		break;
		case 2:
			data[1] = TSL2561_COMMAND_PRESSURE2;
			delay = 14;
		break;
		case 3:
			data[1] = TSL2561_COMMAND_PRESSURE3;
			delay = 26;
		break;
		default:
			data[1] = TSL2561_COMMAND_PRESSURE0;
			delay = 5;
		break;
	}
	result = writeBytes(data, 2);
	if (result) // good write?
		return(delay); // return the delay in ms (rounded up) to wait before retrieving data
	else
		return(0); // or return 0 if there was a problem communicating with the BMP
}


/*
// sealevel()
// given a pressure P (mb) taken at a specific altitude (meters), return the equivalent pressure (mb) at sea level
double SFE_TSL2561::sealevel(double P, double A)
{
	return(P / pow(1-(A/44330.0),5.255));
}

// altitude()
// given a pressure measurement P (mb) and the pressure at a baseline P0 (mb), return altitude (meters) above baseline
double SFE_TSL2561::altitude(double P, double P0)
{
	return(44330.0*(1-pow(P/P0,1/5.255)));
}
*/