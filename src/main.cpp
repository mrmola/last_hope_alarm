#include <Arduino.h>


#include "WiFi.h"
#include "time.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "alarm.h"

#define TIME_DATA_MINUTE_NAME "MINUTES"
// Replace with your network credentials
const char* ssid = "SkibidiSpec";
const char* password = "SkibidiRizzRPI";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println("gottem");
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.printf("Bruh?");
    request->send(SPIFFS, "/1.html", String(), false, nullptr);
  });

  server.on("/stuff.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/stuff.js", String(), false, nullptr);
  });

  //This input gets thrown into the garbage, we don't care. 
  server.on("/WAIT_TIME", HTTP_POST, [](AsyncWebServerRequest *request){
    int headers = request->headers();
    int i;
    int sleepMinutes;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      if(strcmp(h->name().c_str(),TIME_DATA_MINUTE_NAME)==0){ 
        sleepMinutes = atoi(h->value().c_str()); 
      }
    }
    Serial.printf("SLEEP VALUE: %d\n", sleepMinutes);
    //SAVE WHATEVER THE INPUT IS HERE
    request->send(200, "text/plain", "NICE JOB!");
    
    //analogWriteFrequency(sleepMinutes);
    alarmForSecondsLater(sleepMinutes*60);
  });
  // Start server
  initialize();
  if(wakeup_reason == ESP_SLEEP_WAKEUP_GPIO) {
    Serial.printf("GPIO_WAKEUP");
  } else if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.printf("TIMER_WAKEUP");
    alarmLogic();
  }
  Serial.println("GOING");
  server.begin();
  Serial.println("GOING");
  Serial.println("GOING");
}
 
void loop(){
}