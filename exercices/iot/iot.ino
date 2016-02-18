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
