#include <esp8266wifi.h>                        //required header file for esp2866
#include <esp8266httpclient.h>                  //required header file for esp8266 as http client
#include <ArduinoJson.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

StaticJsonBuffer<200> jsonBuffer;

const char* ssid = "SSID GOES HERE";            //credentials for connecting to the WiFi
const char* password = "PASSWORD GOES HERE";

const char* channel_id = "Youtube Channel Id Goes Here";
const char* api_key = "Your api key goes here";

String subscriber;

void setup () {
    Serial.begin(115200);                       //setting the baudrate for serial communication
    WiFi.begin(ssid, password);                 //connecting to the WiFi
 
    while (WiFi.status() != WL_CONNECTED) {     //if being connected to the wifi
        delay(1000);
        Serial.print("Connecting..");
    }

    lcd.begin(16, 2);
    lcd.print("Welcome");
    delay(1000);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
 
        http.begin("https://www.googleapis.com/youtube/v3/channels?part=statistics&id="+$channel_id+"&fields=items/statistics/subscriberCount&key="+$api_key);
        int httpCode = http.GET();
 
        if (httpCode > 0) {
            String payload = http.getString();
            JsonObject& root = jsonBuffer.parseObject(payload);

            if(!root.success()) {
                Serial.println("parseObject() failed");
            }

            subscriber = root["items"][0]["statistics"]["subscriberCount"];
            lcd.setCursor(0, 1);
            lcd.print(subscriber);
        }
 
        http.end();
    }
    delay(60000);
}