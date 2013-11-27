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

void setup()
{
  unsigned char ID;
  boolean state;
  unsigned char array[2];
  // Initialize part
  
  Serial.begin(9600);
  Serial.println("TSL2561 example sketch");

  if (light.begin())
    Serial.println("Init success!");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println("Error, init fail\n\n");
    while(1); // Pause forever.
  }

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


  Serial.println("powerdown...");
  light.setPowerDown();
  Serial.print("getStatus: ");
  light.getStatus(state);
  Serial.println(state);

  Serial.println("powerup...");
  light.setPowerUp();
  Serial.print("getStatus: ");
  light.getStatus(state);
  Serial.println(state);

  
  

  // SET GAIN

  Serial.println();

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println("setGain 0");
  light.setGain(0);

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println("setGain 1");
  light.setGain(1);

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println();

  // MANUAL START AND STOP

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println("manualStart");
  light.manualStart();

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println("manualStop");
  light.manualStop();

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  // INTEGRATION TIME

  Serial.println();

  Serial.println("IT = 0");
  light.setIntegrationTime(0x00);

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println("IT = 1");
  light.setIntegrationTime(0x01);

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println("IT = 2");
  light.setIntegrationTime(0x02);

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

  Serial.println("IT = 3");
  light.setIntegrationTime(0x03);

  Serial.print("timing: ");
  array[0] = 0x81;
  light.readBytes(array, 1);
  printBinary(array[0]);
  Serial.println();

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

  readAll();
}

void loop()
{
  
  
  
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


void readAll()
{
  unsigned char array[16];

  array[0] = 0x80;
  light.readBytes(array,16);
  
  Serial.println();
  for (int x = 0; x < 16; x++)
  {
    Serial.print(x,HEX);
    Serial.print(" ");
    Serial.print(array[x],HEX);
    Serial.print(" ");
    printBinary(array[x]);
    Serial.println();
  }
  Serial.println();
}


