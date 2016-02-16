# Introduction to IoT with an Arduino

This is a small, practical and simple introduction to the concept of IoT through the use of an Arduino Uno, a lan adapter and a temperature and relative humidity sensor.

## Requirements

* Software:
  - [Arduino IDE](https://www.arduino.cc/en/Main/Software "Arduino IDE")
  - [DHT11 library](http://playground.arduino.cc/Main/DHT11Lib "DHT11 Library")
  - [Ethercard library](https://github.com/jcw/ethercard "Ethercard Library")
* Hardware:
  - See this collection: [eBay collection](http://www.ebay.com/cln/babysclem/arduino-iot/227391728016 "http://www.ebay.com/cln/babysclem/arduino-iot/227391728016")

## Setup

* Install Arduino IDE
* Install the DHT11 library as described [here](https://www.arduino.cc/en/Guide/Libraries "here").
* If you are using a recent version of OSx you might have issues with the USB connection to the Arduino. If so, check those links:
  - http://kiguino.moos.io/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html
  - http://calumk.com/blog/0029/

## Hardware connections

Connect the hardware pieces together according to the following diagrams:

![DHT11](img/dht11_arduino.jpg)

![LAN](img/lan_arduino.jpg)

## Some testing

Before building your own code I recommand you test your hardware.
For each hardware element do the following:

1. Open Arduino IDE
2. Go to File > Examples > Select the name of your library > Select the sketch you want to run
3. Click the upload button (right arrow)
4. Wait for the code to be compiled and uploaded to you Arduino
5. Open the Serial Monitor and verify that the Baud Rate matches the one from the sketch (look for the instruction Serial.begin)
6. You should see some relevant debug information

Nota, for the ethernet it is preferable to use the following MAC address: `0x00,0x15,0x99,0xB7,0x98,0x8C`

## Getting started

The following exercises will help you create your first IoT device.:

* Read the DHT11: [Exercise #1](exercices/exercise_01.md "Exercise #1")
* Create an end point: [Exercise #2](exercices/exercise_02.md "Exercise #2")
* Go IoT: [Exercise #3](exercices/exercise_03.md "Exercise #3")

## How do I debug my code?

The Arduino IDE is not very debug friendly. The best you can do is to output messages through the Serial Monitor everytime you need.
Remember to remove the debug information once your done, it usually eats up program storage space, dynamic memory and speed.
Here is an example:

```c
void setup(void) {
	Serial.begin(115200); // This opens the communication with the serial monitor
	...
}

void loop(void) {
	...
	Serial.print(F("My var = ")); // You can output either constant string
	Serial.println(myVar); // Or any value stored in a variable
	...
}
```

## Best practices

### Keep your code organized

Like in any language it is important to keep your code organized. It will help you find the bugs quickly and build something that can be read by anyone.

1. Use naming conventions, for instance:
  * the constants are snake-uppercase: `MY_CONSTANT`
  * the global variables are camelcase prepended with an underscore: `_myGlobalVariable`
  * the local variables are camelcase: `myLocalVariable`
  * the `volatile` variables are prepended with `v_`: `_v_myVolatileVariable`
  * etc.
2. Define constants, instead of hardcoding values: `#define MY_CONSTANT 123` 
3. Create functions, don't throw everything inside the setup() and loop() routines!
4. Create new tabs when there are too much functions, regroup the functions by theme like `initializers`, `getters`, `setters` etc.

### Don't waste dynamic memory

The Arduinos are quite limited in dynamic memory and using more than 75% of it can result in 'random' behaviour.

1. Use program storage space as much as you can, for instance to store hard-coded strings use the `F` macro: `Serial.print(F("Hello world!"));` instead of `Serial.print("Hello world!");`
2. Be sure to use as fewest bytes as possible when defining your variables
3. When defining an array, try to set its size as tight as possible

### Be carefull with `millis()`!

`millis()` is the function that returns the number of milliseconds since the last boot of the Arduino.

1. `millis()` returns an `unsigned long` so don't try to store it in another type of variable
2. `millis()` can't go beyond the value of 4294967295ms, which are 49 days, 17 hours, 2 minutes and 47.295 seconds. Why? Because an unsigned long is enconded with 4 bytes. What happens next? The next value will be 0, thake that into account if your sketch is using `millis()` over a long periode of time.

### Understand what an interrup is

If you are going to use them it is fundamental that you understand that interrupts are not executed in a different 'thread' than the main program. Arduinos are not multitask. Interrupts are just pausing the main program when required to run.
Reference: [https://www.arduino.cc/en/Reference/Interrupts](https://www.arduino.cc/en/Reference/Interrupts "https://www.arduino.cc/en/Reference/Interrupts")

### Debounce the switches

When using switches on your project think about debouncing the input. There are tons of examples on the Internet, here is what worked great for me:

* Software part:

   ```c
   #define DEBOUNCE_DELAY                50              // Delay during which we ignore the button actions (milliseconds)
   #define BUTTON_PIN                    2               // Interrupt pin
   volatile unsigned long
     _v_lastInterruptTime =              0;
   
   void setup(void) {
     pinMode(BUTTON_PIN, INPUT_PULLUP);
     attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isrButton, FALLING);
   }
   
   void loop(void) {
     // Do what you need to do
   }
   
   void isrButton(void) {
     if (millis() - _v_lastInterruptTime > (unsigned long)DEBOUNCE_DELAY) {
       // Do what you need to do
     }
     _v_lastInterruptTime = millis();
   }
   ```

* Hardware part:

   ![Hard debouncing](http://i.imgur.com/iNz1J.png)

### How long will my battery last?

When you are building an off-the-grid project you will need to power your Arduino with a battery that will supposedly run out some day. Here are some tips on how to easily extend your battery life.

1. Use a low power Arduino like the Pro Mini 3.3V
2. Use a slow Arduino if possible: the Arduino usually runs at 16MHz but you can find some Pro Mini 3.3V running at 8MHz. Why? Because the lesser the number of cycles per seconds the lesser the power need is.
3. Remove the useless LEDs
4. Use an efficient voltage regulator (like the LT1529) and bypass the Arduino's one.

Here is a good link on how to optimise your design: [http://www.home-automation-community.com/arduino-low-power-how-to-run-atmega328p-for-a-year-on-coin-cell-battery/](http://www.home-automation-community.com/arduino-low-power-how-to-run-atmega328p-for-a-year-on-coin-cell-battery/ "http://www.home-automation-community.com/arduino-low-power-how-to-run-atmega328p-for-a-year-on-coin-cell-battery/")

### One Arduino hasn't enough program memory for your sketch?

1. Think about using less memory consuming libraries
2. Think about adding a second Arduino to your project, they can communicate quite easily via i2c, follow this link: [http://dsscircuits.com/articles/arduino-i2c-slave-guide](http://dsscircuits.com/articles/arduino-i2c-slave-guide "http://dsscircuits.com/articles/arduino-i2c-slave-guide").
