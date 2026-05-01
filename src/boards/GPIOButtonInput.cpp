// ============================================================
//  GPIOButtonInput.cpp
// ============================================================
#include "GPIOButtonInput.h"

GPIOButtonInput::GPIOButtonInput(uint8_t upPin, uint8_t selPin)
    : _up(upPin), _sel(selPin) {}

void GPIOButtonInput::init(uint16_t /*displayWidth*/,
                           uint16_t /*displayHeight*/) {
    _up.begin();
    _sel.begin();
}

bool GPIOButtonInput::read(InputEvent& out) {
    bool pressed = false;

    // Check UP button
    if (_up.poll(pressed)) {
        out.type = pressed ? InputEventType::PRESS : InputEventType::RELEASE;
        out.x    = 0;
        out.y    = 0;
        out.id   = 0;
        return true;
    }

    // Check SELECT button
    if (_sel.poll(pressed)) {
        out.type = pressed ? InputEventType::PRESS : InputEventType::RELEASE;
        out.x    = 1;
        out.y    = 0;
        out.id   = 1;
        return true;
    }

    return false;
}
