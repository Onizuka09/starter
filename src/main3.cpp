#include <Ethernet.h>
#include "wakeOnLan.h"
// MAC address for your Ethernet shield (should be unique)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
//http://172.16.121.101:5000/test
IPAddress server(172, 16, 121, 101); // Example: IP for example.com (or use a domain name)
EthernetClient client;
void performGET(); 
void setup() {
  Serial.begin(115200);
     Ethernet.init(33);
  Ethernet.begin(mac);
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
      Serial.println("ETH connection success  "); 

  // Start the Ethernet connection


  // Give the Ethernet shield time to initialize
  delay(1000);
  Serial.println("Ethernet initialized");
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());

  // Make a GET request

}
void performPost();

void loop() {
  // Read the server response
performGET(); 
delay(3000);
performPost();
delay(3000);

}

void performPost(){ 
    String QuerryParams = "?key1=value1&key2=value2"; 
    String reqPath  ="/receive"; 
    String postData = "{\"key\":\"value\", \"number\":123}";
    String test="POST /receive HTTP/1.1";
    if (client.connect(server, 5000)) {
        // send psot reques 
        Serial.println("Connected to server");
        // client.println("GET /test HTTP/1.1");
        client.println("POST "+reqPath +" HTTP/1.1");
        client.println("Connection: close");
        client.println("Content-Type: application/json");
        client.printf("Content-Length: %d\r\n", postData.length()); // Length of the post data

        // END of headers 
        client.println();

        // Send the POST body
        client.println(postData); // Send the actual data to the server
        
    // wait for sever response 
        while ( client.connected()){ 
            String line = client.readStringUntil('\n'); 
            Serial.println(line);
            if ( line == "\r"){ 
                Serial.println("Headers Received");
                break;
            }
        }
        Serial.println("Reasponse Body: ");
        while ( client.available()){ 
            String line = client.readStringUntil('\n');
            Serial.println(line); 
        } 
        client.stop(); 
        Serial.println("Connection closed "); 
  } else {
    Serial.println("Connection failed");
    return ; 
  }
}

void performGET() {
    String QuerryParams = "?key1=value1&key2=value2"; 
    String reqPath  ="/receive"+QuerryParams; 
    String test="GET /receive HTTP/1.1";
    if (client.connect(server, 5000)) {
        Serial.println("Connected to server");
        // client.println("GET /test HTTP/1.1");
        client.println("GET "+reqPath +" HTTP/1.1");
        // client.println(test);
        // client.printf("Host: %s\n\r",server); // Use the domain name of your server
        client.println("Connection: close");
        client.println();

    // wait for sever response 
        while ( client.connected()){ 
            String line = client.readStringUntil('\n'); 
            Serial.println(line);
            if ( line == "\r"){ 
                Serial.println("Headers Received");
                break;
            }
        }
        Serial.println("Reasponse Body: ");
        while ( client.available()){ 
            String line = client.readStringUntil('\n');
            Serial.println(line); 
        } 
        client.stop(); 
        Serial.println("Connection closed "); 
  } else {
    Serial.println("Connection failed");
    return ; 
  }

}
