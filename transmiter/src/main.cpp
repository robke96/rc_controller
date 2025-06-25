#include <Arduino.h>
#include "config.h"
#include "timer.h"
#include "controller.h"
#include "nrf.h"

constexpr int JOYSTICK_CENTER { 500 };
constexpr uint8_t JOYSTICK_DEADZONE { 30 }; 
constexpr uint8_t WHEELS_DEADZONE { 50 }; 

bool checkInteraction() {
    bool activityDetected = false;

    int btn1 = digitalRead(BTN_LEFT_PIN);  
    int btn2 = digitalRead(BTN_LEFT_PIN);

    if (btn1 == LOW || btn2 == LOW) {
        activityDetected = true;
    }

    int joystickPins[] = {JOYSTICK_LEFT_X, JOYSTICK_LEFT_Y, JOYSTICK_RIGHT_X, JOYSTICK_RIGHT_Y, LWHEEL, RWHEEL};
    int numPins = sizeof(joystickPins) / sizeof(joystickPins[0]);

    for (int i = 0; i < numPins; i++) {
        int reading = analogRead(joystickPins[i]);
        int deadzone = (i < 4) ? JOYSTICK_DEADZONE : WHEELS_DEADZONE; 
        if (abs(reading - JOYSTICK_CENTER) > deadzone) {
            activityDetected = true;
            break; 
        }
    }

    return activityDetected;
}

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Arduino is on!");

    // power btn
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, HIGH);

    // buttons
    pinMode(BTN_LEFT_PIN, INPUT);
    pinMode(BTN_TOPR_PIN, INPUT);
    
    // led
    pinMode(LED_WHITE_PIN, OUTPUT);
    digitalWrite(LED_WHITE_PIN, HIGH);
    
    // analog
    pinMode(JOYSTICK_LEFT_X, INPUT);
    pinMode(JOYSTICK_LEFT_Y, INPUT);
    pinMode(JOYSTICK_RIGHT_X, INPUT);
    pinMode(JOYSTICK_RIGHT_Y, INPUT);
    pinMode(JOYSTICK_RIGHT_Y, INPUT);
    pinMode(LWHEEL, INPUT);
    pinMode(RWHEEL, INPUT);

    initRadio();
}

bool stopSend = false;

void loop() {
    bool activity = checkInteraction();
    checkTimer(activity);

    struct ControllerData myController = {
        digitalRead(BTN_LEFT_PIN),  
        digitalRead(BTN_TOPR_PIN),
        analogRead(JOYSTICK_LEFT_X),  
        analogRead(JOYSTICK_LEFT_Y),
        analogRead(JOYSTICK_RIGHT_X),  
        analogRead(JOYSTICK_RIGHT_Y),  
        analogRead(RWHEEL),  
        analogRead(LWHEEL),  
    };

    if (activity) {
        sendControllerData(myController);
        stopSend = false;
    } else {
        if (!stopSend) {
            for (int i = 0; i < 3; i++) {
                sendControllerData(myController);
                delay(50);
            }
            stopSend = true;
        }
    }
}
