# Arduino-Temperature-Control-and-Digital-Watch

The following parts are necessary to build the project:

  * Arduino Uno
  * ST7789 Display
  * DS18B20 Temperature Sensor
  * Breadboard
  * Wires
  * Button

The following libraries have been used:

<SPI.h>
<Adafruit_GFX.h>
<Arduino_ST7789_Fast.h>
<Wire.h>
<OneWire.h>
<DallasTemperature.h>

You can download the Adafruit_GFX library via the Arduino IDE. From the menu, we select Sketch -> Include Library -> Manage Libraries. 
Then we search for the Adafruit GFX library and we press Install. 

To download the DS18B20 library, we go to Manage Libraries via the Arduino IDE, then we search for DS18B20, and download the DalasTemperature library

# Description
In this project, a simple menu was created using a ST7789 Display connected using the IIC (I2C) interface to Arduino UNO together with three push buttons.
We have added a DS18B20 temperature sensor to monitor the temperature of the environment. We also added a function were it is possible to check the current
time of the day.

# Credits

The Arduino_ST7789_Fast library can be obtained from the following repository: https://github.com/cbm80amiga/Arduino_ST7789_Fast

# Video
The operation of the project is shown in the following video: https://www.youtube.com/watch?v=KkHouAeKf64

