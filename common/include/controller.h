#pragma once

const byte address[6] = {0xA3, 0xC4, 0xD2, 0xE5, 0xF6};

struct ControllerData {
    bool btnLeft;
    bool btnTopRight;

    int joystickLX;
    int joystickLY;
    int joystickRX;
    int joystickRY;

    int wheelR;
    int wheelL;
};