// ============================================================
//  TFTeSPIDisplay.cpp
// ============================================================
#include "TFTeSPIDisplay.h"
#include <Arduino.h>   // pinMode, analogWrite, digitalWrite

TFTeSPIDisplay::TFTeSPIDisplay(int8_t backlightPin)
    : _backlightPin(backlightPin) {}

void TFTeSPIDisplay::init() {
    _tft.init();
    _tft.setRotation(0);          // portrait; change to 1 for landscape
    _tft.fillScreen(TFT_BLACK);

    if (_backlightPin >= 0) {
        pinMode(_backlightPin, OUTPUT);
        setBacklight(255);        // full brightness by default
    }
}

uint16_t TFTeSPIDisplay::width()  { return (uint16_t)_tft.width();  }
uint16_t TFTeSPIDisplay::height() { return (uint16_t)_tft.height(); }

void TFTeSPIDisplay::fillScreen(uint16_t colour) {
    _tft.fillScreen(colour);
}

void TFTeSPIDisplay::fillRect(int32_t x, int32_t y,
                               int32_t w, int32_t h,
                               uint16_t colour) {
    _tft.fillRect(x, y, w, h, colour);
}

void TFTeSPIDisplay::drawPixel(int32_t x, int32_t y, uint16_t colour) {
    _tft.drawPixel(x, y, colour);
}

void TFTeSPIDisplay::pushPixels(int32_t x, int32_t y,
                                 int32_t w, int32_t h,
                                 uint16_t* buf) {
    // pushImage handles the window-set + bulk write internally.
    // This is what LVGL will call in its flush callback.
    _tft.pushImage(x, y, w, h, buf);
}

void TFTeSPIDisplay::setBacklight(uint8_t brightness) {
    if (_backlightPin < 0) return;

    if (brightness == 0) {
        digitalWrite(_backlightPin, LOW);
    } else if (brightness == 255) {
        digitalWrite(_backlightPin, HIGH);
    } else {
        // PWM: analogWrite works on most ESP32 GPIO via Arduino framework.
        analogWrite(_backlightPin, brightness);
    }
}
