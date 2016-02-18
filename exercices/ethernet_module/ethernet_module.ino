#include <EtherCard.h>

#define ETHER_PIN 10     // what pin DHT the CS is connected to

// Ethernet MAC address
static byte mymac[] = { 0x00,0x15,0x99,0xB7,0x98,0x8C };

const char page[] PROGMEM =
"HTTP/1.0 202 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<html>"
  "<head>"
    "<title>Workshop IoT</title>"
    "<meta charset=\"UTF-8\">"
  "</head>"
  "<body>"
    "<h1>Hello World!</h1>"
  "</body>"
"</html>";

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

void setup(){
  Serial.begin(115200);

  if (ether.begin(sizeof Ethernet::buffer, mymac, ETHER_PIN) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
    
  ether.printIp(F("My IP: "), ether.myip);
}

void loop() {
  // wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) {
    // copy the page in the buffer
    memcpy(ether.tcpOffset(), page, sizeof page);
    // send the page
    ether.httpServerReply(sizeof page - 1);
  }
}
