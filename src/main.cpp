//I, Noe Puna, 000781646, I acknowledge that this my own work and no other
// person's work has been copied without acknowledgement.

#include <Arduino.h> 
#include <ESP8266WiFi.h> 
 
// access credentials for WiFi network. 
const char* ssid = "xxxxx"; 
const char* password = "xxxxx"; 
 
// WiFI server.  Listen on port 80, which is the default TCP port for HTTP 
WiFiServer server(80); 

#include <OneWire.h> 
// DS18B20 sensor library 
#include <DallasTemperature.h> 
 
// Pin that the  DS18B20 is connected to 
const int oneWireBus = D3;      
 
// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWire(oneWireBus); 
 
// Pass our oneWire reference to Dallas Temperature sensor  
DallasTemperature DS18B20(&oneWire); 

DeviceAddress tempDeviceAddress;
 
 
void setup() 
{ 
  Serial.begin(115200); 
  Serial.println("\nWeb Server Demo"); 
 
  Serial.printf("\nConnecting to %s ", ssid); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(" connected"); 
 
  server.begin(); 
  Serial.printf("Web server started, open %s in a web browser\n", 
WiFi.localIP().toString().c_str()); 

// Start the DS18B20 sensor 
  DS18B20.begin(); 

  int deviceID = DS18B20.getAddress(tempDeviceAddress, 0);

  if (deviceID){
    Serial.println("DEVICE ID: " + String(deviceID));
  } else {
    Serial.println("DISCONNECTED");
    Serial.end();
    
  }
} 
 
 
// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(80);               // prevent ram fragmentation
  //BELLOW DOESN'T SEND HTML CONTENT PROPERLY SO I COMMENTED IT OUT
  htmlPage = F(/*"HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "Refresh: 5\r\n"         // refresh the page automatically every 5 sec
               "\r\n"
               "<!DOCTYPE HTML>"
               "<html>"*/
               "000781646, Noe Puna");
  return htmlPage;
}

void loop() 
{ 
  float fTemp; 
  String judgement;
 
    // ask DS18B20 for the current temperature 
    DS18B20.requestTemperatures(); 
  
    // fetch the temperature.  We only have 1 sensor, so the index is 0. 
    fTemp = DS18B20.getTempCByIndex(0); 

  WiFiClient client = server.available(); 
 
  // wait for a client (web browser) to connect 
  if (client) 
  { 
    Serial.println("\n>> Client connected"); 
    Serial.println(">> Client request:"); 
    while (client.connected()) 
    { 
      // read line by line what the client (web browser) is requesting 
      if (client.available()) 
      { 
        // print information about the request (browser, OS, etc) 
        String line = client.readStringUntil('\r'); 
        Serial.print(line); 
        // wait for end of client's request 
        if (line.length() == 1 && line[0] == '\n') 
        { 
          // send some data back to the browser.  Note:  this is not 
          // proper HTML! 
          client.println("Hello from the HTTP Server"); 
          client.println("The temperature is: " + String(fTemp));
          client.println(prepareHtmlPage());
          Serial.println(">> Response sent to client"); 
          break; 
        } 
      } 
    } 
 
    // keep read client request data (if any).  After that, we can terminate 
    // our client connection 
    while (client.available()) { 
      client.read(); 
    } 
 
    // close the connection: 
    client.stop(); 
    Serial.println(">> Client disconnected"); 

    

    
  } 
} 
