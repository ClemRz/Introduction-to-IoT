# Exercise 1: How to "read" the DHT11

In this first set of exercises we are going to connect to the temperature & relative humidity sensor (DHT11) and get the readings from it.

## Hardware

Connect the hardware parts together according to the following diagram:

![DHT11](../img/dht11_arduino.jpg)

## Software

### Hello World!

Let's learn how to use an Arduino sketch with the classic "Hello World!" example.

When you open the Arduino IDE you will see a new sketch that includes the following code:

```c
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
```

The way the Atmega328p works (the microcontroller that's on the Arduino board) is as follows:

1. Once the Arduino boots up it runs only one time the `setup()` function
2. As long as the Arduino is not reseted or shut down it runs the `loop()` function again and again

Quite easy ha!

To output debug information we are going to use the Serial communication that connects the Arduino to your computer via the USB cable. To initialize a Serial communication you will need to specify the following in the `setup()` of your sketch:

```c
Serial.begin(115200);
```

This way you can use the methods from the Serial class to output what you need in the Serial Monitor. Check out the documentation [here](https://www.arduino.cc/en/Reference/Serial "here")!

Let's append the following to the `setup()` function:

```c
Serial.println("Hello World!");
```

Now you can upload your sketch to the Arduino (right arrow icon) and open the Serial Monitor (magnifier icon). Make sure the bauds rate is 115200 (at the bottom right corner).

What do you see ?

Now append the following to the `loop()` function:

```c
Serial.println("Hello you!");
```

Upload it, what's goin'on?

Okay, I think you got it, we need to slow this m#$%*ker down!

To make the Arduino wait we use the `delay()` function, see the documentation [here](https://www.arduino.cc/en/Reference/Delay "here")).

Is it better now?

Here is the complete sketch:

```c
void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");
}

void loop() {
  delay(3000);
  Serial.println("Hello you!");
}
```

### Read the DHT11

First thing we need to do is start a new sketch.
At the top of it we will import the library we need:

```c
#include "DHT.h"
```

Then we need to define some constants right below, those are placeholders in your sketch that will be replaced by the specified value just befor compilation (see documentation [here](https://www.arduino.cc/en/Reference/Define "here"):

```c
#define DHTPIN    2
#define DHTTYPE   DHT11
```

`DHT11` is already a constant defined in the DHT library!

The next thing we need is a global variable that holds the DHT class instanciation. To make a variable global you just have to write it outside of any function (see the scope of vars [here](https://www.arduino.cc/en/Reference/Scope "here"))

```c
DHT dht(DHTPIN, DHTTYPE);
```

In the `setup()` function we need to start the Serial communication (refer to the "Hello World!" example) and to start the communication with the DHT11:

```c
dht.begin();
```

Now in the `loop()` function we are going to get and display the temperature and the relative humidity read from that crazy sensor.

If you open the file /path/to/your/arduino/workspace/libraries/DHT/DHT.h you will see that there are 2 usefull methods in the DHT class: `float readTemperature(bool S=false);` and `float readHumidity(void);`.

Try to store both values in variables and output it in the Serial Monitor.

How does it look?

Here is the complete script:

```c
#include "DHT.h"

#define DHTPIN    2      // what pin DHT output is connected to
#define DHTTYPE   DHT11  // what DHT we're using

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  dht.begin();
}

void loop(){
  float h = dht.readHumidity(),
        t = dht.readTemperature();
  
  Serial.print(F("Relative humidity: ")); Serial.print(h); Serial.println(F("%RH"));
  Serial.print(F("Temperature: ")); Serial.print(t); Serial.println(F("°C"));
  delay(3000);
}
```

Have you noticed? What is that `F()` function? Look at this [playground](http://playground.arduino.cc/Learning/Memory "playground").

Nice ha! This is a good practice. Each time you have to use constant strings don't forget to encapsulate them with the `F()` macro.

## Good job!

You've just finished the first set of exercices, well done! Let's go to the [next](exercise_02.md "next") one!