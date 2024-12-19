#include "wakeOnLan.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
/*DEFAULT SPI PIN ESP32
MOSI 
MISO 
VCC
SC 33
SCLK 
GND 

*/
EthernetUDP udp;
IPAddress broadcastIP(255, 255, 255, 255);            // Replace with your network broadcast IP
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Replace with your W5500 MAC address
byte targetMAC[] = { 0x00, 0xe0, 0x4c, 0x3a, 0x3f, 0x6a }; // Replace with target MAC address


static void sendMagicPacket(byte *mac); 


void init_wol(){ 
  Ethernet.init(33);
  Ethernet.begin(mac);
  udp.begin(9);
  Serial.println("TEsting eth  connection  "); 
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      Serial.flush();
    }

  EthernetLinkStatus  st ; 
    Serial.println("TEsting Link status "); 
    st = Ethernet.linkStatus() ; 
    while (st != LinkON) {
      delay(20);
      Serial.print(".");
      Serial.flush();
      st = Ethernet.linkStatus() ;
      switch (st ) {
		    case LinkON:   Serial.println("LINK ON ");break;
		    case Unknown:   Serial.println("UNKNOW");break;
		    case LinkOFF:  Serial.println("LINK OFF");break;
		    default:        Serial.println("Default ");break;
	    }
  }
      Serial.println("connection success  "); 

}
void wakePC(){ 
for ( int i = 0 ; i < 10 ; i ++)
{       
    sendMagicPacket(targetMAC);
    Serial.println("Sending magic packet...");
    Serial.println(i);
    delay(2000);
    Serial.flush();
}
}

void sendMagicPacket(byte *mac) {
  byte magicPacket[102]; // Magic packet: 6 FFs + 16 MAC addresses
  memset(magicPacket, 0xFF, 6); // Fill first 6 bytes with 0xFF

  // Append MAC address 16 times
  for (int i = 6; i < 102; i++) {
    magicPacket[i] = mac[(i - 6) % 6];
  }

  // Send the packet
  udp.beginPacket(broadcastIP, 9); // Port 9 is standard for WoL
  udp.write(magicPacket, sizeof(magicPacket));
  udp.endPacket();
}
