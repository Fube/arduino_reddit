#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>

const int rs = 4, en = 0, d4 = 12, d5 = 13, d6 = 15, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// WIFI Params
const char* ssid = "as";
const char* password = "if";

/**
 * name path: data -> name
 * karma path: data -> total_karma
 */

const String BASE_URL = "https://www.reddit.com/user/";
String url;
String username;

HTTPClient http;
int httpCode;

void setup() {

  Serial.begin(115200);

  delay(1000);
  Serial.println("What is the username?");

  while (!Serial.available()) {}
  username = Serial.readStringUntil('\n');
  url = BASE_URL + username +"/about.json";
  
  lcd.begin(16, 2);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Still connecting...");
  }

  Serial.println("Connected");
  Serial.println(url);
}

void loop() {
  
  lcd.clear();
  
  http.begin(url, "DB:E9:D5:FE:EB:EF:68:34:55:FD:62:BA:C9:BB:04:D4:E3:22:18:81");

  httpCode = http.GET();

  Serial.println(httpCode);

  if(httpCode > 0){
    
    String payload = http.getString();
    DynamicJsonDocument doc(2493);
    DeserializationError error = deserializeJson(doc, payload);

    if(error){
      Serial.println("Error on deserialization error");
    }else{
      String name = doc["data"]["name"];
      String karma = doc["data"]["total_karma"];
      
      Serial.println(name);
      Serial.println(karma);

      lcd.print(name);
      
      lcd.setCursor(0,1);
      
      lcd.print("Karma: ");
      lcd.print(karma);
    }
    
  }else{
    Serial.println("There was an error GETting the specified URL");
  }

  delay(120000);
 }
