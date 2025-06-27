#include <Arduino.h>
#include "config.h"

unsigned long timerStart = 0;
const unsigned long timerDuration = 180000; //3min
bool timerRunning = false;

void deviceTurningOff() {
    Serial.println("Turning off arduino in 3s");
    digitalWrite(LED_WHITE_PIN, LOW);
    delay(3000);

    // kill power
    digitalWrite(POWER_PIN, LOW);
};

void checkTimer(bool activity) {
    if (!timerRunning) {
        timerStart = millis();
        timerRunning = true;
    }

    if (activity) {
        Serial.println("Activity detected");
        timerStart = millis();
    }

    // no interaction == device turn off
    if (millis() - timerStart >= timerDuration) {
        timerRunning = false;

        deviceTurningOff();
    }
  
}
