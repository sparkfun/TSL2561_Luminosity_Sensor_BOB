/* SFE_TSL2561 library example sketch

This sketch shows how to use the SFE_TSL2561 library to read the
Bosch TSL2561 light sensor.
https://www.sparkfun.com/products/11824

Like most light sensors, the TSL2561 measures absolute light.
To convert this to the sea-level-compensated light used in weather
reports, you must provide the known altitude at which the light
was measured.

If, on the other hand, you want to use this sensor to measure altitude,
you will need to provide the known light for a given baseline altitude.
This sketch shows the use of both functions.

Hardware connections:

- (GND) to GND
+ (VDD) to 3.3V

(WARNING: do not connect + to 5V or the sensor will be damaged!)

You will also need to connect the I2C pins (SCL and SDA) to your Arduino.
The pins are different on different Arduinos:

                    SDA  SCL
Uno, Redboard, Pro  A4   A5
Mega2560, Due       20   21
Leonardo             2    3

Leave the IO (VDDIO) pin unconnected. This pin is for connecting
the TSL2561 to systems with lower logic levels such as 1.8V

Have fun! -Your friends at SparkFun.

The SFE_TSL2561 library uses floating-point equations developed by the
Weather Station Data Logger project: http://wmrx00.sourceforge.net/

Our example code uses the "beerware" license. You can do anything
you like with this code. No really, anything. If you find it useful,
buy me a beer someday.

V10 Mike Grusin, SparkFun Electronics 10/24/2013
*/

// Your sketch must #include this library, and the Wire library
// (Wire is a standard library included with Arduino):

#include <SFE_TSL2561.h>
#include <Wire.h>

// Create an SFE_TSL2561 object, here called "light":

SFE_TSL2561 light(TSL2561_ADDR);

// Global variables:

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time

void setup()
{
  // Initialize the Serial port:
  
  Serial.begin(9600);
  Serial.println("TSL2561 example sketch");

  // Initialize the SFE_TSL2561 library:

  light.begin();

  // Get factory ID from sensor:
  // (Just for fun, you don't need to do this to operate the sensor)

  unsigned char ID;
  
  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5X");
  }
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else
  {
    byte error = light.getError();
    printError(error);
  }

  // The light sensor has a default integration time of 402ms,
  // and a default gain of low (1X)
  
  // If you would like to change the integration time, you can
  // do so using the setTiming() command
  
  // If gain = false (0), device is set to low gain (1X)
  // If gain = high (1), device is set to high gain (16X)

  gain = 0;

  // If time = 0, integration will be 13.7ms
  // If time = 1, integration will be 101ms
  // If time = 2, integration will be 402ms
  // If time = 3, use manual start / stop to perform your own integration

  unsigned char time = 2;

  // setTiming() will optionally set the third parameter (ms)
  // to the requested integration time in ms:
  
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);

  // To start measurements, power up the sensor:
  
  Serial.println("Powerup...");
  light.setPowerUp();
}

void loop()
{
  // Wait between measurements before retrieving the result
  // (You can also configure the sensor to issue an interrupt
  // when measurements are complete)
  
  // You can also perform your own integration by setting "time"
  // to 3 (manual) in setTiming(), then performing a manualStart()
  // and a manualStop():
  
  // ms = 1000;
  // light.manualStart();
  delay(ms);
  // light.manualStop();
  
  // Once integration is complete, we'll retrieve the data.
  
  // There are two light sensors on the device, one for visible light
  // and one for infrared. Both are needed for lux calculations:

  unsigned int data0, data1;
  
  if (light.getData(data0,data1))
  {
    // getData() returned true, communication was successful
    
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1);
  
    // To calculate lux, pass all your settings and readings
    // to the getLux() function.
    
    // The getLux() function will return 1 if the calculation
    // was successful, or 0 if one or both of the sensors was
    // saturated (too much light). If this happens, you can
    // reduce the integration time and/or gain.
  
    double lux;
    boolean good;
    
    good = light.getLux(gain,ms,data0,data1,lux);
    
    Serial.print(" lux: ");
    Serial.print(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }
  else
  {
    // getData() returned false because of an I2C error

    byte error = light.getError();
    printError(error);
  }
}

void printError(byte error)
  // If there's an error, print out an explanation
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

