/*

This is an example of Triton Updating Channel Feeds to ThingSpeak Cloud
 
  ----> https://www.neptcloud.com/
  
  Check out the links above for our tutorials and wiring diagrams
  These chips use SPI to communicate.
 
 Getting Started with ThingSpeak:
 
    * Sign Up for New User Account - https://www.thingspeak.com/users/new
    * Create a New Channel by selecting Channels and then Create New Channel
    * Enter the Write API Key in this sketch under "ThingSpeak Settings"

*/
#include <Triton_WiFi.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// Local Network Settings
#define WLAN_SSID       "myNetwork"        // cannot be longer than 32 characters!
#define WLAN_PASS       "myPassword"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define WEBSITE      "www.thingspeak.com"

// ThingSpeak Settings
String writeAPIKey = "TUEJD2SBH3Q35DUL";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 10000;        // Time interval in milliseconds to update ThingSpeak   
long value = 500; 
Triton_WiFi_Client client;

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int resetCounter = 0;
uint32_t ip;
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


}

void loop()
{
  client = wifi.connectTCP(ip, 80);
  String analogPin0 = String(value, DEC);
  value = value + 10;
  Serial.println(analogPin0);
 
  
  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println();
    Serial.println("...disconnected.");
    Serial.println();
    
    client.close();
  }
  
  // Update ThingSpeak
  //if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
if( (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("field1="+analogPin0);

Serial.println(F("Reading answer ..."));
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  }
  
  lastConnected = client.connected();
// Close connection and disconnect
  
  client.close();
  Serial.println(F("Closing connection"));
  // Wait 10 seconds until next update
  delay(30000);
}

void updateThingSpeak(String tsData)
{
  if (client.connected())
  { 
    Serial.println("Connected to ThingSpeak...");
    Serial.println();
        
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    
    resetCounter = 0;
    
  }
  else
  {
    Serial.println("Connection Failed.");   
    Serial.println();
    
    resetCounter++;
    
    if (resetCounter >=5 ) {resetEthernetShield();}

    lastConnectionTime = millis(); 
  }
}

void resetEthernetShield()
{
  Serial.println("Resetting Ethernet Shield.");   
  Serial.println();
  
  client.close();
  delay(1000);
  
   if (!wifi.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("connect to AP Failed!"));
    while(1);
  }
  delay(1000);
}


