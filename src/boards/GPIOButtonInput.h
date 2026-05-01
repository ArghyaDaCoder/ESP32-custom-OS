#pragma once
// ============================================================
//  GPIOButtonInput.h  –  IInput implemented via GPIO buttons
//
//  Simulates a two-button "controller" (UP / SELECT) that the
//  OS shell can use for navigation when no touch screen is
//  available.  Generates InputEvents with x=0/y=0 for UP and
//  x=1/y=0 for SELECT so the shell can distinguish them.
//
//  Wokwi users: add two pushbuttons to diagram.json wired to
//  the pins defined in the board config.
// ============================================================
#include "../hal/IInput.h"
#include <Arduino.h>

// Simple single-button tracker with software debounce.
struct ButtonTracker {
    uint8_t  pin;
    uint32_t lastChangeMs  = 0;
    bool     lastState     = HIGH;  // active-low buttons assumed
    bool     reportedState = HIGH;

    explicit ButtonTracker(uint8_t p) : pin(p) {}

    void begin() { pinMode(pin, INPUT_PULLUP); }

    // Returns true on state change after debounce period.
    // `isPressed` will be true on press, false on release.
    bool poll(bool& isPressed, uint32_t debounceMs = 30) {
        bool current = digitalRead(pin);
        uint32_t now = millis();

        if (current != lastState) {
            lastState     = current;
            lastChangeMs  = now;
        }

        if ((now - lastChangeMs) > debounceMs && current != reportedState) {
            reportedState = current;
            isPressed     = (current == LOW);  // active-low
            return true;
        }
        return false;
    }
};

class GPIOButtonInput : public IInput {
public:
    // upPin    → maps to InputEvent x=0, y=0  (navigate / scroll)
    // selPin   → maps to InputEvent x=1, y=0  (confirm / tap)
    GPIOButtonInput(uint8_t upPin, uint8_t selPin);

    void init(uint16_t displayWidth, uint16_t displayHeight) override;
    bool read(InputEvent& out)                               override;

private:
    ButtonTracker _up;
    ButtonTracker _sel;
};
