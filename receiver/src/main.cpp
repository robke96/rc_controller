#include <SPI.h>
#include <RF24.h>
#include "controller.h"
#include <Servo.h>

RF24 radio(9, 10); // cns ce
const byte address[6] = {0xA3, 0xC4, 0xD2, 0xE5, 0xF6};

const int led = 5;
const int servoPin = 2;

Servo servomotor;

void setup() {
    Serial.begin(9600);
    servomotor.attach(servoPin);
    servomotor.write(0);

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_HIGH);
    radio.startListening();
}

void loop() {
    if (radio.available()) {
        ControllerData data;
        radio.read(&data, sizeof(data));

        // buttons
        if (data.btnLeft == LOW) {
            digitalWrite(led, HIGH);
        } else {
            digitalWrite(led, LOW);
        }

        // analogs
        if (data.joystickLY >= 0) {
            int servoAngle = map(data.joystickLY, 0, 1024, 0, 180);
            servomotor.write(servoAngle);    
        }
    }
}