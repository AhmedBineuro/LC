#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <FS.h>
#include <ArduinoJson.h>

//Define ssid and password to connect
#define ssid "ENTER YOUR SSID" 
#define pass "ENTER YOUR PASSWORD"
//setting up static IP
IPAddress local_IP(192,168,1,188);//Enter any IP you want
IPAddress gateway(_,_,_,_);//Use your router's gateway instead of the underscores
IPAddress subnet(255,255,255,0);
//JSON object holding ID and msg
JsonObject jsonPkg;
const int arraySize=20;
String Message[arraySize];
int ID;
ESP8266WebServer server(80);//Set up server at port 80 for http connection 

int trialsMax=300;//trials before declaring failed to connect

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //SPIFFS.begin();
  //attemp cnnection to wifi
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  //set static ip to ESP
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid,pass);
    for(int i=0;i<trialsMax&&WiFi.status()!=WL_CONNECTED;i++)
    {
      Serial.print('.');
      delay(100);
    }
  server.on("/",SendMessage);
  server.on("/fetch",fetchMessage);
  //start server if connected to wifi ,else post error message
  if(WiFi.status()==WL_CONNECTED)
  {
    Serial.println("Connection established");
    Serial.print("IP");
    Serial.println(WiFi.localIP());
    server.begin();
  }
  else
  Serial.println("Failed to connect to wifi");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()==WL_CONNECTED)
    server.handleClient();
}
//function to send message
void SendMessage()
{
  //check if we detect a body
  if (server.hasArg("plain")== false)
  { 
    //Check if body received
    server.send(200, "text/plain", "Body not received");
    return;
  }
  //Extract the JSON segment and formulate a message to send back to client
  String JSONpacket=server.arg("plain");
  String message = "Body received\n";
 //Parsing the JSON segment
 DynamicJsonDocument buff(512);
 deserializeJson(buff,JSONpacket);
 jsonPkg= buff.as<JsonObject>();
 ID=jsonPkg["id"].as<int>();
 if(ID>=arraySize)
    message+=" Index not available";
 else
 {
    Message[ID]=jsonPkg["msg"].as<String>();
    message=jsonPkg["msg"].as<String>();
    Serial.println(message);
 }
 //Send back the comfirmation back to the client
  server.send(200, "text/plain", message);
  Serial.println(ID);
}
//communicate by using HTTP POST method
void fetchMessage()
{
  //check if we detect a body
  if (server.hasArg("plain")== false)
  { 
    //Check if body received
    server.send(200, "text/plain", "Body not received");
    return;
  }
  //Extract the JSON segment and formulate a message to send back to client
  String JSONpacket=server.arg("plain");
  String message = "{\"msg\":\"your message\"}";
 //Parsing the JSON segment
 DynamicJsonDocument buff(512);
 deserializeJson(buff,JSONpacket);
 jsonPkg= buff.as<JsonObject>();
 //Send back the comfirmation back to the client
  ID=jsonPkg["id"].as<int>();
  if(ID<arraySize)
  {
    server.send(200, "text/plain", Message[ID]);
    Serial.println(Message[ID]);
  }
  Serial.println(ID);
}
