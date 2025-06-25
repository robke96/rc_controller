#include "config.h"
#include <RF24.h>
#include <SPI.h>
#include <controller.h>

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = {0xA3, 0xC4, 0xD2, 0xE5, 0xF6};

void initRadio() {
    if (!radio.begin()) {
        Serial.println(F("radio hardware is not responding!!"));
        while (1) {}  // hold in infinite loop
    }

    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_HIGH);
    radio.stopListening();
}

void sendControllerData(ControllerData &c) {
    radio.write(&c, sizeof(c));
}

