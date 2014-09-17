/**************************************************************************/
/*! 
  @file     Triton_WiFi_Server.h

  This is a library for the Triton IoT rapid prototype platform
  
   Check out the links below for our tutorials 
   These chips use SPI to communicate.
	----> https://www.neptcloud.com
  
  The only difference between this implementation and the Ethernet library
  is that a special client reference facade is returned by the available()
  function, instead of a copy of a client like in the Ethernet library.  This
  is necessary to ensure the buffers that client instances contain aren't
  copied and get out of sync.

*/
/**************************************************************************/

#ifndef ADAFRUIT_CC3000_SERVER_H
#define ADAFRUIT_CC3000_SERVER_H

#include "Triton_WiFi.h"

#include "Client.h"
#include "Server.h"

// Assume 4 sockets available, 1 of which is used for listening, so at most 3 
// clients can be connected at once.
#define MAX_SERVER_CLIENTS 3 

// Facade that wraps a reference to a client instance into something that looks
// and acts like a client instance value.  This is done to mimic the semantics 
// of the Ethernet library, without running into problems allowing client buffers
// to be copied and get out of sync.
class Triton_WiFi_ClientRef : public Client {
 public:
  Triton_WiFi_ClientRef(Triton_WiFi_Client* client);
  // Return true if the referenced client is connected.  This is provided for
  // compatibility with Ethernet library code.
  operator bool();
  // Below are all the public methods of the client class:
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

 private:
  // Hide the fact that users are really dealing with a pointer to a client
  // instance.  Note: this class does not own the contents of the client
  // pointer and should NEVER attempt to free/delete this pointer.
  Triton_WiFi_Client* _client;

};


class Triton_WiFi_Server : public Server {
public:
  // Construct a TCP server to listen on the specified port.
  Triton_WiFi_Server(uint16_t port);
  // Return the index of a client instance with data available to read.
  // This is useful if you need to keep track of your own client state, you can
  // index into an array of client state based on the available index returned
  // from this function.  Optional boolean parameter returns by reference true
  // if the available client is connecting for the first time.
  int8_t availableIndex(bool *newClient = NULL);
  // Get a client instance from a given index.
  Triton_WiFi_ClientRef getClientRef(int8_t clientIndex);
  // Return a reference to a client instance which has data available to read.
  Triton_WiFi_ClientRef available();
  // Initialize the server and start listening for connections.
  virtual void begin();
  // Write data to all connected clients.  Buffer is a pointer to an array
  // of bytes, and size specifies how many bytes to write from the buffer.
  // Return the sum of bytes written to all clients.
  virtual size_t write(const uint8_t *buffer, size_t size);
  // Write a byte value to all connected clients.
  // Return the sum of bytes written to all clients.
  virtual size_t write(uint8_t value);
  // Make the overloads of write from the Print base class available.
  using Print::write;

private:
  // Store the clients in a simple array.
  Triton_WiFi_Client _clients[MAX_SERVER_CLIENTS];
  // The port this server will listen for connections on.
  uint16_t _port;
  // The id of the listening socket.
  uint16_t _listenSocket;

  // Accept new connections and update the connected clients.
  bool acceptNewConnections();
};

#endif
