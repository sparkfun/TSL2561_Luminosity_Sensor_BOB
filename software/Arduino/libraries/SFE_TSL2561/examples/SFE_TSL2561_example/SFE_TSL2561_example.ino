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

// Your sketch must #include this library, and the Wire library.
// (Wire is a standard library included with Arduino.):

#include <SFE_TSL2561.h>
#include <Wire.h>

// You will need to create an SFE_TSL2561 object, here called "light":

SFE_TSL2561 light(TSL2561_ADDR);

unsigned int ms;

void setup()
{
  unsigned char ID;
  boolean state;
  unsigned char array[2];
  // Initialize part
  
  Serial.begin(9600);
  Serial.println("TSL2561 example sketch");

  light.begin();

  // Get ID number from part
  // (just for fun, you don't need to do this to operate the part)

  if (light.getID(ID))
  {
    Serial.print("Got part ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5X");
  }
  else
  {
    Serial.println("Error, couldn't retrieve part ID\n\n");
    while(1); // Pause forever.
  }
  
  // POWER UP AND DOWN

  Serial.println("powerup...");
  light.setPowerUp();

  // set timing
  

  
  Serial.println("set timing...");
  light.setTiming(0,2,ms);
  
  delay(ms);
  
  Serial.println("get data...");
  unsigned int data0, data1;
  
  light.getData(data0,data1);
  
  Serial.print("data0: ");
  Serial.print(data0);
  Serial.print(" data1: ");
  Serial.println(data1);
  
/*
  // int threshold low

  unsigned int myuint;

  Serial.println();
  light.setInterruptThreshold(0x0123,0xABCD);

  light.readUInt(2,myuint);
  Serial.println(myuint,HEX);
  
  light.readUInt(4,myuint);
  Serial.println(myuint,HEX);

  // INT CTL
  unsigned char mybyte;

  Serial.print("control: ");
  light.readByte(6,mybyte);
  printBinary(mybyte);
  Serial.println();

  light.setInterruptControl(0xFF,0xFF); 

  Serial.print("control: ");
  light.readByte(6,mybyte);
  printBinary(mybyte);
  Serial.println();
*/
}

void loop()
{
  delay(ms);
  
  //Serial.println("get data...");
  unsigned int data0, data1;
  
  light.getData(data0,data1);
  
//  Serial.print("data0: ");
//  Serial.print(data0);
//  Serial.print(" data1: ");
//  Serial.println(data1);
  
  Serial.print(data0);
  Serial.print("  ");
  Serial.print(data1);
  Serial.print("  ");

  double lux;
  boolean good;
  good = light.getLux(0,ms,data0,data1,lux);
  Serial.print(lux);
  Serial.print(" ");
  Serial.println(good);  
  
}

void printBinary(byte x)
// Utility routine to print a number (8 bits) in binary
{
  char y;
  
  // Step through all eight bits, MSB to LSB
  for (y = 7; y >= 0; y--)
  {
    // Print a space between the upper and lower nybbles
    if (y == 3) Serial.print(" ");
    
    // Check if the bit is a 1 or 0 and print that out
    if (x & (1 << y)) Serial.print("1"); else Serial.print("0");
  }
}

