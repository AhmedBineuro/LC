//Include the libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//file systemm library
#include <FS.h>
//Arduino json library to store and transport password and ssid
#include <ArduinoJson.h>

//SSID and password information
String ssid="";
String pass="";
//port number
int port=80;
bool FailedToConnect=false;
//Start server at the designated port
ESP8266WebServer server;
void setup() {
  //Start communication with SPIFFS method
  SPIFFS.begin();
  //Basically tell the browser where to look when it wants to access the files
  server.serveStatic("/pallete.css",SPIFFS,"/pallete.css");
  server.serveStatic("/WifiManager.js",SPIFFS,"/WifiManager.js");
  server.serveStatic("/index.html",SPIFFS,"/index.html");
  server.serveStatic("/",SPIFFS,"/WifiManager.html");
  
  //Initialize serial monitor to print local IP
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  Serial.print("\nConnecting to ");
  Serial.print(ssid);
  int trials=0;
  //Connection check and handling
  do{
    //trials counter
    trials++;
    if(trials>=30000)
    {
      Serial.print('\n');
      Serial.println("Connection Failed, check your internet and try again");
      FailedToConnect=true;
      break;
    }
    Serial.print('.');
    delay(100);
  }while(WiFi.status()!=WL_CONNECTED);
  if(WiFi.status()==WL_CONNECTED)
  {
    loggedIn=true;
    Serial.print('\n');
    Serial.println("Connection Established");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    //I think it is used to tell the server which page to start on
    server.on("/",Index);
    server.begin();
  }
}

void loop() {
   server.handleClient();
}
