/**************************************************************************/
/*! 
  @file     Triton_WiFi.h

  This is a library for the Triton IoT rapid prototype platform
 
  Check out the links below for our tutorials 
  These chips use SPI to communicate.
   ----> https://www.neptcloud.com

  @release

  v1.0    - Initial release
*/
/**************************************************************************/

#ifndef TRITON_WIFI_H
#define TRITON_WIFI_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "utility/cc3000_common.h"
#include "utility/wlan.h"
#include "utility/netapp.h"
#include "ccspi.h"
#include "Client.h"

#if defined(__arm__) && defined(__SAM3X8E__) // Arduino Due
  #define SPI_CLOCK_DIVIDER 6 // used to set the speed for the SPI bus; 6 == 14 Mhz on the Arduino Due
#else
  #define SPI_CLOCK_DIVIDER SPI_CLOCK_DIV2 // Don't set this to a slower speed (i.e. larger div value)
                                           // or communication will be flakey on 16mhz chips!
#endif

/* Oliver:enable debug printer */
#define ENABLE_CC3K_PRINTER

// These are the interrupt and control pins
#define CC3K_IRQ   CC3K_IRQ  // MUST be an interrupt pin!
// These can be any two pins
#define CC3K_VBAT  CC3K_VBAT
#define CC3K_CS    PIN_SPI_SS3


#ifdef ENABLE_CC3K_PRINTER
#define CHECK_PRINTER if(CC3KPrinter != 0)
#else
#define CHECK_PRINTER if(false)
#endif

#if defined(UDR0) || defined(UDR1) || defined(CORE_TEENSY) || ( defined (__arm__) && defined (__SAM3X8E__) )
  #define CC3K_DEFAULT_PRINTER &Serial
#else
  #define CC3K_DEFAULT_PRINTER 0
#endif

#define WLAN_CONNECT_TIMEOUT 10000  // how long to wait, in milliseconds
#define RXBUFFERSIZE  64 // how much to buffer on the incoming side
#define TXBUFFERSIZE  32 // how much to buffer on the outgoing side

#define WIFI_ENABLE 1
#define WIFI_DISABLE 0
#define WIFI_STATUS_CONNECTED 1

typedef struct Result_Struct
{
  uint32_t  num_networks;
  uint32_t  scan_status;
  uint8_t   rssiByte;
  uint8_t   Sec_ssidLen;
  uint16_t  time;
  uint8_t   ssid_name[32];
  uint8_t   bssid[6];
} ResultStruct_t;   /**!ResultStruct_t : data struct to store SSID scan results */

/* Enum for wlan_ioctl_statusget results */
typedef enum 
{
  STATUS_DISCONNECTED = 0,
  STATUS_SCANNING     = 1,
  STATUS_CONNECTING   = 2,
  STATUS_CONNECTED    = 3
} status_t;

class Triton_WiFi;

class Triton_WiFi_Client : public Client {
 public:
  Triton_WiFi_Client(uint16_t s);
  Triton_WiFi_Client(void);
  Triton_WiFi_Client(const Triton_WiFi_Client& copy);
  void operator=(const Triton_WiFi_Client& other);
  
  // NOTE: If public functions below are added/modified/removed please make sure to update the 
  // Triton_WiFi_ClientRef class to match!

  int connect(IPAddress ip, uint16_t port);
  int connect(const char *host, uint16_t port);

  uint8_t connected(void);
  size_t write(uint8_t c);

  size_t fastrprint(const char *str);
  size_t fastrprintln(const char *str);
  size_t fastrprint(char *str);
  size_t fastrprintln(char *str);
  size_t fastrprint(const __FlashStringHelper *ifsh);
  size_t fastrprintln(const __FlashStringHelper *ifsh);

  size_t write(const void *buf, uint16_t len, uint32_t flags = 0);
  int read(void *buf, uint16_t len, uint32_t flags = 0);
  int read(void);
  int32_t close(void);
  int available(void);

  int read(uint8_t *buf, size_t size);
  size_t write(const uint8_t *buf, size_t size);
  int peek();
  void flush();
  void stop();
  operator bool();

  uint8_t _rx_buf[RXBUFFERSIZE], _rx_buf_idx;
  int16_t bufsiz;

 private:
  int16_t _socket;

};

// Ugly but necessary to include the server header after the client is fully defined.
// A forward reference in the server header won't cut it because the server needs to contain
// instances of the client.  The client definition above can be pulled into a separate
// header in a later change to make this cleaner.
#include "Triton_WiFi_Server.h"

class Triton_WiFi {
  public:
    Triton_WiFi(uint8_t csPin, uint8_t irqPin, uint8_t vbatPin, uint8_t spispeed = SPI_CLOCK_DIVIDER, Print* cc3kPrinter = CC3K_DEFAULT_PRINTER);
    bool     begin(uint8_t patchReq = 0, bool useSmartConfigData = false, const char *_deviceName = NULL);
    void     reboot(uint8_t patchReq = 0);
    void     stop(void);
    bool     disconnect(void);
    bool     deleteProfiles(void);
    void     printHex(const byte * data, const uint32_t numBytes);
    void     printHexChar(const byte * data, const uint32_t numBytes);
    void     printIPdots(uint32_t ip);
    void     printIPdotsRev(uint32_t ip);
    uint32_t IP2U32(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
    bool     getMacAddress(uint8_t address[6]);
    bool     setMacAddress(uint8_t address[6]);
    bool     setStaticIPAddress(uint32_t ip, uint32_t subnetMask, uint32_t defaultGateway, uint32_t dnsServer);
    bool     setDHCP();

    bool     connectToAP(const char *ssid, const char *key, uint8_t secmode, uint8_t attempts = 0);
    bool     connectSecure(const char *ssid, const char *key, int32_t secMode);
    bool     connectOpen(const char *ssid); 
    bool     checkConnected(void);
    bool     checkDHCP(void);
    bool     getIPAddress(uint32_t *retip, uint32_t *netmask, uint32_t *gateway, uint32_t *dhcpserv, uint32_t *dnsserv);

    bool     checkSmartConfigFinished(void);

    Triton_WiFi_Client connectTCP(uint32_t destIP, uint16_t destPort);
    Triton_WiFi_Client connectUDP(uint32_t destIP, uint16_t destPort);
     
    #ifndef CC3000_TINY_DRIVER
    bool     getFirmwareVersion(uint8_t *major, uint8_t *minor);
    status_t getStatus(void);
    bool     startSSIDscan(uint32_t *index);
    void     stopSSIDscan();
    uint8_t  getNextSSID(uint8_t *rssi, uint8_t *secMode, char *ssidname);

    bool     listSSIDResults(void);
    bool     startSmartConfig(const char *_deviceName = NULL, const char *smartConfigKey = NULL);

    bool     getIPConfig(tNetappIpconfigRetArgs *ipConfig);


    uint16_t ping(uint32_t ip, uint8_t attempts=3,  uint16_t timeout=500, uint8_t size=32);
    uint16_t getHostByName(char *hostname, uint32_t *ip);
    #endif

    /* Functions that aren't available with the tiny driver */
    #ifndef CC3000_TINY_DRIVER
    bool     scanSSIDs(uint32_t time);
    #endif

    void setPrinter(Print*);

  private:
    bool _initialised;

};

extern Print* CC3KPrinter;
extern Triton_WiFi wifi;
#endif
