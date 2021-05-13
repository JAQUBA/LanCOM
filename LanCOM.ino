#include <SPI.h>
#include <UIPEthernet.h>

#define STATIC
#define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x05
#define LISTENPORT 1000
#define UARTBAUD 9600

#ifdef STATIC
  #define MYIPADDR 192,168,0,101
  #define MYIPMASK 255,255,255,0
  #define MYDNS 192,168,0,1
  #define MYGW 192,168,0,1
#endif

EthernetServer server(LISTENPORT);

void setup() {
  Serial.begin(UARTBAUD);
  uint8_t mac[6] = {MACADDRESS};
#ifdef STATIC
  uint8_t myIP[4] = {MYIPADDR};
  uint8_t myMASK[4] = {MYIPMASK};
  uint8_t myDNS[4] = {MYDNS};
  uint8_t myGW[4] = {MYGW};
  Ethernet.begin(mac, myIP, myDNS, myGW, myMASK);
#else
  Ethernet.begin(mac);
#endif
  server.begin();

  pinMode(8, OUTPUT);
}

void loop() {
  EthernetClient client = server.available();
  
  size_t size;
  if(client) {
    while((size = client.available()) > 0) {
      
      uint8_t* msg = (uint8_t*)malloc(size+1);
      
      memset(msg, 0, size+1);
      
      size = client.read(msg,size);
      
      Serial.write(msg, size);
  
      free(msg);
    }
  }
  if(Serial.available() > 0) {
    delay(20);
    while((size = Serial.available()) > 0) {

      digitalWrite(8, HIGH);
      
      /*char* msg = (char*)malloc(size+1);
      
      memset(msg, 0, size+1);
      
      size = Serial.readBytes(msg, size);
      
      server.write(msg, size);
  
      free(msg);*/

      char recv = (char)Serial.read();
      server.write(recv);

      digitalWrite(8, LOW);
    }
  }
}
