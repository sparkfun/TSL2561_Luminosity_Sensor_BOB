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
  char ID;
  boolean state;
  
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

  if (light.getID(&ID))
  {
    Serial.print("Got part ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5?");
  }
  else
  {
    Serial.println("Error, couldn't retrieve part ID\n\n");
    while(1); // Pause forever.
  }

  Serial.println("powerup...");
  light.setPowerUp();
  Serial.print("getStatus: ");
  light.getStatus(&state);
  Serial.println(state);

  Serial.println("powerdown...");
  light.setPowerDown();
  Serial.print("getStatus: ");
  light.getStatus(&state);
  Serial.println(state);
}

void loop()
{
  
  
  
}
