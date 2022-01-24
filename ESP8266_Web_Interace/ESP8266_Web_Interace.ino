#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//file systemm library
#include <FS.h>
//Arduino json library to store and transport password and ssid
#include <ArduinoJson.h>

//SSID and password information
String ssid="ring.onion";
String pass="kimchijiggae0229";
//port number
int port=80;
bool loggedIn;
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
    trials++;
    if(trials>=30000)
    {
      Serial.print('\n');
      Serial.println("Connection Failed, check your internet and try again");
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

//Function to get index file
void Index()
{
  //the r indicates a read function
  File file=SPIFFS.open("/WifiManager.html","r");
  //Streamfile is used to put up the html file (text/html is to indicate it's type) 
  server.streamFile(file,"text/html");
  file.close();
 /*
  //the r indicates a read function
  File file=SPIFFS.open("index.html","r");
  //Streamfile is used to put up the html file (text/html is to indicate it's type) 
  server.streamFile(file,"text/html");
  file.close();
  */
}
