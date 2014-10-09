/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "Hi, I'm Triton!" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/


#include <Triton_WiFi.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <PubSubClient.h>

// Local Network Settings
#define WLAN_SSID       "myNetwork"        // cannot be longer than 32 characters!
#define WLAN_PASS       "myPassword"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define WEBSITE      "test.mosquitto.org"

Triton_WiFi_Client wifiClient;
uint32_t ip;

// Update these with values suitable for your network.

//byte server[] = { 85, 119, 83, 194 };

PubSubClient client(WEBSITE, 1883, callback, wifiClient);
/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!wifi.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); wifi.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); wifi.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); wifi.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); wifi.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); wifi.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}




void setup()
{
  Serial.begin(115200);
  Serial.println(F("Hello, WiFi!\n")); 
/* Initialise the module */
  Serial.println(F("\nInitialising the WiFi module..."));
  if (!wifi.begin())
  {
    Serial.println(F("Unable to initialise the WiFi module! Check your wiring?"));
    while(1);
  }

/* Attempt to connect to an access point */
  char *ssid = WLAN_SSID;             /* Max 32 chars */
  Serial.print(F("\nAttempting to connect to ")); Serial.println(ssid);
  
  /* NOTE: Secure connections are not available in 'Tiny' mode!
     By default connectToAP will retry indefinitely, however you can pass an
     optional maximum number of retries (greater than zero) as the fourth parameter.
  */
  if (!wifi.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("connect to AP Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!wifi.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  ip = 0;
  // Try looking up the website's IP address
  Serial.print(WEBSITE); Serial.print(F(" -> "));
  while (ip == 0) {
    if (! wifi.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }
wifi.printIPdotsRev(ip);

if (client.connect("arduinoClient")) {
    client.publish("outTopic","Hi, I'm Triton!");
    client.subscribe("inTopic");
  }
}

void loop()
{
  client.loop();
}

