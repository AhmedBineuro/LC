//include libraries
#include <ESP8266WebServer.h> //Esp webserver
#include <ESP8266HTTPClient.h> //http client library to send post requests
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <ArduinoJson.h> //Json library

//defines
#define TFT_RST   D4     // TFT RST pin is connected to NodeMCU pin D4 (GPIO2)
#define TFT_CS    D3     // TFT CS  pin is connected to NodeMCU pin D4 (GPIO0)
#define TFT_DC    D2     // TFT DC  pin is connected to NodeMCU pin D4 (GPIO4)
#define SSID_ "YOUR SSID"
#define PASS_ "YOUR PASSWORD"
#define BACKGROUND 0xFB5A
#define TEXT 0xFFFF
#define TEXT_FONT 1.5
#define URL "ENTER YOUR LC SERVER URL/IP" //The url should use the URL ending with /fetch 

//JSON object to hold ID and message
JsonObject MSG;
String message;
//create display oject
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//Initiate webserver on port 80
ESP8266WebServer server(80);

//Create http client object
HTTPClient http;
WiFiClient client;
//Create timer variable for requests cooldown
int rqstTimer=200000;
void setup() {
  //Start serial monitor
  Serial.begin(115200);
  //intitialize screen
  tft.initR(INITR_GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(BACKGROUND);
  tft.setCursor(0,0);
  tft.setTextSize(TEXT_FONT);
  tft.setTextColor(TEXT);
  tft.print("Waiting for message! :D");
  //Start wifi
  WiFi.begin(SSID_,PASS_);
  Serial.print("Connecting to:");
  Serial.println(SSID_);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println(WiFi.localIP());
  server.on("/message",displayMsgFromHttp);
  delay(2000);
}

void loop() {
  server.handleClient();
  if(rqstTimer<=0)
  {
    rqstTimer=2000;
    requestMessage();
  }
  else
    rqstTimer--;
}
//Function to fetch http request and parse message and display it
void displayMsgFromHttp()
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
  String message = "Body received:\n";
 //Parsing the JSON segment
 DynamicJsonDocument buff(512);
 deserializeJson(buff,JSONpacket);
 MSG= buff.as<JsonObject>();
 //
 //Send back the comfirmation back to the client
  server.send(200, "text/plain", message);
  tft.fillScreen(BACKGROUND);
  tft.setCursor(0,0);
  tft.setTextSize(TEXT_FONT);
  message=MSG["msg"].as<String>();
  tft.print(message);
}
void requestMessage()
{
  //Initialize http object
  http.begin(client,URL);
  http.addHeader("Content-Type", "application/json");
  int httpCode=http.POST("{\"id\":1}");
  String payload=http.getString();
  http.end();
  if(httpCode==200&&message!=payload)
  {
    message=payload;
    server.send(200, "text/plain", "Got object");
    tft.fillScreen(BACKGROUND);
    tft.setCursor(0,0);
    tft.setTextSize(TEXT_FONT);
    tft.print(message);
  }
}
