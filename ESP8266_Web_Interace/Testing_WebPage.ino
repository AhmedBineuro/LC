#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST77xx.h>
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//File access library
#include <FS.h>
//Arduino Json library
#include <ArduinoJson.h>

const char *ssid = "Kiki";
const char *password = "12345678";

//these two variables will hold the SSID and PASSWORD of the wifi we will try and connect to
char *SSID = "";
char *PASS = "";
JsonObject wifiPass;

#define TFT_RST   D4     // TFT RST pin is connected to NodeMCU pin D4 (GPIO2)
#define TFT_CS    D3     // TFT CS  pin is connected to NodeMCU pin D4 (GPIO0)
#define TFT_DC    D2     // TFT DC  pin is connected to NodeMCU pin D4 (GPIO4)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

ESP8266WebServer server(80);

void setup() {       // Initialize HalloWing-oriented screen
  tft.initR(INITR_GREENTAB);
  tft.setRotation(1); 
  tft.fillScreen(ST77XX_PINK);
  SPIFFS.begin();
  //Include other pages
  server.serveStatic("/",SPIFFS,"/WifiManager.html");
  server.serveStatic("/WifiManager.css",SPIFFS,"/WifiManager.css");
  server.serveStatic("/WifiManager.js",SPIFFS,"/WifiManager.js");
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  printIP();
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.on("/", Index);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
void Index()
{
  //the r indicates a read function
  File file=SPIFFS.open("/main.html","r");
  //Streamfile is used to put up the html file (text/html is to indicate it's type) 
  server.streamFile(file,"text/html");
  file.close();
}
//Function to 
bool loadWiFi(){
  File file=SPIFFS.open("/WIFI.json","r");
  DynamicJsonDocument<1024> doc;
  
}
void printIP(){
  tft.fillScreen(ST77XX_RED);
  tft.setCursor(0,0);
  tft.setTextSize(1);
  tft.print(WiFi.softAPIP());
}
