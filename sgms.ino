#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <ArduinoJson.h>

WiFiClient client;



//....................................................
//wifi creds
const char* ssid = "Sam";
const char* password = "classroom";

//......................................................

//web server
const char* url = "http://bindata.bse23-5.one//insert.php";
//..................................

//BIN DETAILS
//..........................................................

int bin_id = 5;

//.......................................................


void setup() {

  Serial.begin(9600);
  
  //connect to wifi
  // wifiSerial.begin(115200);
  WiFi.begin(ssid, password);
  
   while (WiFi.status() != WL_CONNECTED) {
       Serial.print(".");
       delay(500);
   }

   Serial.println("");
   Serial.print("IP Address: ");
   Serial.println(WiFi.localIP());
}

void loop() {
  handleRequest();
  delay(10000);
}

void handleRequest()
{
  
  DynamicJsonDocument doc(1024);
  
  double air_quality = 0, bin_level = 0;
  
  // Reading the response
  boolean messageReady = false;
  String message = "";
  while(messageReady == false) { // blocking but that's ok
    if(Serial.available()) {
      message = Serial.readString();
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc,message);
  if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  
  bin_level = doc["bin_level"];
  air_quality = doc["air_quality"];

  sendToServer(bin_level,air_quality);

  delay(5000);
  
}

//..........................................
//send data to server
//..........................................
void sendToServer(double bin_level,double air_quality){

  String request = "bin_id=" + String(bin_id) + "&bin_level=" + String(bin_level) + "&bin_air_quality=" + String(air_quality);
  
  // Make HTTP request to your endpoint
  HTTPClient http;

  http.begin(client,url);
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  
  int httpResponseCode = http.POST(request);
  
  http.end();
}
