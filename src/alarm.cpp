// Big thanks to https://randomnerdtutorials.com/esp32-timer-wake-up-deep-sleep/

#include <EEPROM.h>
#include "alarm.h"
#include "time.h"

#define TIMESTORAGE 0
#define AMOUNTSTORAGE 16
#define WAKEUP_PIN 4 //This is the pin that causes a wakeup from the button Pin 5
#define PWM_PIN 8 //The pin which we will control the high voltage module with Pin 3
#define RELAY_PIN 9 //The pin we trigger the relay to discharge into the sinner. Pin 7
                                //ACTIVE LOW
void sleepForSeconds(int seconds);

void initialize() {
    Serial.println("GOING");
    EEPROM.begin(32);
    Serial.println("GOING");
    pinMode(WAKEUP_PIN, INPUT_PULLUP); //The default is 1 because of this
    Serial.println("GOING");
    pinMode(PWM_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    Serial.println("GOING");
    analogWriteFrequency(120000);
    Serial.println("GOING");
    analogWriteResolution(2);
    Serial.println("GOINGf");
    analogWrite(PWM_PIN, 0);
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("GOING");
    //sleepForSeconds(1);
}
void electrocute() {
    Serial.printf("ZAP THEIR ANKLES");
    analogWrite(PWM_PIN, 2);
    delay(1000); //more delay = more ouchy
    digitalWrite(RELAY_PIN, HIGH);
    delay(50);
    digitalWrite(RELAY_PIN, LOW);
    analogWrite(PWM_PIN, 0);
}
void alarmLogic() { //Determining whether to electrocute the poor sap or not
    time_t rtcTime;
    time(&rtcTime);
    long lastRtcTime = EEPROM.readLong(TIMESTORAGE);
    int  amountTime  = EEPROM.readLong(AMOUNTSTORAGE);
    electrocute();
}
void alarmForSecondsLater(int seconds) {
    time_t rtcTime;
    time(&rtcTime);
    EEPROM.writeLong(TIMESTORAGE, rtcTime);
    EEPROM.writeInt(AMOUNTSTORAGE, seconds);
    sleepForSeconds(seconds);
}

void sleepForSeconds(int seconds) {   //Sleep for that number of seconds while
                                      //listening for all pertinent wakeup signals
    time_t rtcTime; 
    time(&rtcTime);
    esp_deep_sleep_enable_gpio_wakeup(WAKEUP_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);
    esp_sleep_enable_timer_wakeup(seconds*1000000);
    Serial.printf("LEMAYO");
    esp_deep_sleep(seconds*1000000);
}