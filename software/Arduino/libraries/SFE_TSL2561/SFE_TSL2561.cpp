/*
  SFE_TSL2561 illumination sensor library for Arduino
	
  Mike Grusin
  http://www.sparkfun.com
	
	version 1.0 2013/10/25 initial version
*/


#include <SFE_TSL2561.h>
#include <Wire.h>
//#include <stdio.h>
//#include <math.h>

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
char SFE_TSL2561::readBytes(unsigned char *values, unsigned int length)
//values is an array of char, first entry should be the register to read from
//subsequent entries will be filled with return values
//char SFE_TSL2561::readBytes(unsigned char *values, char length)
{
	unsigned int x;
	
	Wire.beginTransmission(_i2c_address);
	Wire.write((values[0] & 0x0F) | 0x80);
	_error = Wire.endTransmission();
	if (_error == 0)
	{
		Wire.requestFrom(_i2c_address,length);
		if (Wire.available() == length)
		{
			for(x = 0; x < length; x++)
			{
				values[x] = Wire.read();
			}
			return(1);
		}
	}
	return(0);
}


//value is address of a short (16-bit) int
char SFE_TSL2561::readByte(unsigned char address, unsigned char &value)
//values is an array of char, first entry should be the register to read from
//subsequent entries will be filled with return values
//char SFE_TSL2561::readBytes(unsigned char *values, char length)
{
	Wire.beginTransmission(_i2c_address);
	Wire.write((address & 0x0F) | 0x80);
	_error = Wire.endTransmission();
	if (_error == 0)
	{
		Wire.requestFrom(_i2c_address,1);
		if (Wire.available() == 1)
		{
			value = Wire.read();
			return(1);
		}
	}
	return(0);
}


char SFE_TSL2561::writeByte(unsigned char address, unsigned char value)
{
	Wire.beginTransmission(_i2c_address);
	Wire.write((address & 0x0F) | 0x80);
	Wire.write(value);
	_error = Wire.endTransmission();
	if (_error == 0)
		return(1);

	return(0);
}


//value is address of a short (16-bit) int
char SFE_TSL2561::readUInt(unsigned char address, unsigned int &value)
{
	char high, low;
	
	Wire.beginTransmission(_i2c_address);
	Wire.write((address & 0x0F) | 0x80);
	_error = Wire.endTransmission();
	if (_error == 0)
	{
		Wire.requestFrom(_i2c_address,2);
		if (Wire.available() == 2)
		{
			low = Wire.read();
			high = Wire.read();
			value = word(high,low);
			return(1);
		}
	}
	return(0);
}


//value is address of a short (16-bit) int
char SFE_TSL2561::writeUInt(unsigned char address, unsigned int value)
{
  if (writeByte(address,lowByte(value)) 
  && writeByte(address + 1,highByte(value)))
		return(1);

	return(0);
}


char SFE_TSL2561::getError()
//values is an array of char, first entry should be the register to read from
//subsequent entries will be filled with return values
{
	return(_error);
}


char SFE_TSL2561::clearInterrupt(void)
{
	Wire.beginTransmission(_i2c_address);
	Wire.write(0xC0);
	_error = Wire.endTransmission();
	if (_error == 0)
		return(1);

	return(0);
}


char SFE_TSL2561::setPowerUp(void)
{
	return(writeByte(0x00,0x03));
}


char SFE_TSL2561::setPowerDown(void)
{
	return(writeByte(0x00,0x00));
}


char SFE_TSL2561::getStatus(boolean &status)
{
	unsigned char result;
	
	if (readByte(0x00,result))
	{
		status = ((result & 0x03) == 0x03);
		return(1);
	}
	return(0);
}


char SFE_TSL2561::setGain(boolean gain)
{
	unsigned char timing;

	if (readByte(1,timing))
	{
		if (gain)
			timing |= 0x10;
		else
			timing &= ~0x10;

		if (writeByte(1,timing))
			return(1);
	}
	return(0);
}


char SFE_TSL2561::manualStart(void)
	// command TSL2561 to start a pressure measurement
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	unsigned char timing;
	
	if (readByte(1,timing))
	{
		timing |= 0x08;
		if (writeByte(1,timing))
			return(1);
	}
	return(0);
}


char SFE_TSL2561::manualStop(void)
	// command TSL2561 to start a pressure measurement
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	unsigned char timing;
	
	if (readByte(1,timing))
	{
		timing &= ~0x08;
		if (writeByte(1,timing))
			return(1);
	}
	return(0);
}


char SFE_TSL2561::setIntegrationTime(unsigned char time)
	// time = 0 to 3
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	unsigned char timing;
	
	if (readByte(1,timing))
	{
		timing &= ~0x03;
		timing |= (time & 0x03);

		if (writeByte(1,timing))
			return(1);
	}
	return(0);
}


char SFE_TSL2561::setInterruptThreshold(unsigned int low, unsigned int high)
	// time = 0 to 3
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	if (writeUInt(2,low) && writeUInt(4,high))
		return(1);
		
	return(0);
}


char SFE_TSL2561::setInterruptControl(unsigned char control, unsigned char persist)
	// time = 0 to 3
	// oversampling: 0 - 3 for oversampling value
	// returns n (number of ms to wait) for success, 0 for fail
{
	if (writeByte(5,((control | 0B00000011) << 4) & (persist | 0B00001111)))
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

char SFE_TSL2561::getID(unsigned char &ID)
{
	unsigned char timing;
	
	if (readByte(0x0A,ID))
		return(1);

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