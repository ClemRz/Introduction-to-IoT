# Exercise 3: Let's go IoT!

This time you will find out how to put together the two previous exercises in order to create a real-time web page that shows the temperature and the relative humidity of the room. So exciting!

## Hardware

Refer to the previous exercices to have both sensor and ethernet module connected.

## Software

You will now put together both softwares but, how are you going to put the sensor readings into that `page` constant we defined?

Here is the tricky part :)

Look at the `printf()` function [reference](http://playground.arduino.cc/Main/Printf "reference") and implement it in your sketch.

Is it not compiling? Whyyyyyyyyy? Maybe because our `page` is stored into the program memory (via `PROGMEM`)!

Ok then, let's try to use `sprintf_P()` instead! You will find some documentation [here](http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__stdio_1ga2b829d696b17dedbf181cd5dc4d7a31d.html "here").

Basically, it copies a constant `char` array into another `char` array applying the formating defined. Here is a hint:

```c
sprintf_P(bigstring, page, t, h);
```

Be careful to define the size of your local variable `bigstring` correctly or it will be truncated.

Is it working now? Can you see that beautiful page in your browser?
Or do you need help? Raise your hand!

Here is the complete script:

```c
#include <EtherCard.h>
#include "DHT.h"

#define DHTPIN 2      // what pin we're connected to
#define ETHER_PIN 10
#define DHTTYPE DHT11 // what DHT we're using


// Ethernet IP, default gateway and MAC addresses
static byte mymac[] = { 0x00,0x15,0x99,0xB7,0x98,0x8C };

const char page[] PROGMEM =
"HTTP/1.0 202 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<html>"
  "<head>"
    "<title>Workshop IoT</title>"
    "<meta charset=\"UTF-8\">"
    "<script>setTimeout(function(){ window.location.reload(1); }, 1000);</script>"
  "</head>"
  "<body>"
    "<h3>Workshop IoT</h3>"
    "<div>Temperature: %02d°C</div>"
    "<div>Humidity: %02d%%RH</div>"
  "</body>"
"</html>";

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  
  dht.begin();
  
  if (ether.begin(sizeof Ethernet::buffer, mymac, ETHER_PIN) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
  
  ether.printIp(F("My IP: "), ether.myip);
}

void loop(){
  // wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) {
    char bigstring[sizeof page-4];
    int h = dht.readHumidity(),
        t = dht.readTemperature();
    // build the page
    sprintf_P(bigstring, page, t, h);
    Serial.println(bigstring);
    // copy the page in the buffer
    memcpy(ether.tcpOffset(), bigstring, sizeof bigstring);
    // send the page
    ether.httpServerReply(sizeof bigstring - 1);
  }
}
```

As you can see, some JavaScript is used to refresh the page each second.

## Congrats!

You may or may not did it entirely by yourself but that's ok! In any case you follow this tutorial up to the end and I'm sure you learn usefull stuff!

Remember that there are tons of examples of usage of the Arduino on the Internet and you can still ask your favourite instructor for help whenever you need!

Cheers.