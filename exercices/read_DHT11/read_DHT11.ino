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
