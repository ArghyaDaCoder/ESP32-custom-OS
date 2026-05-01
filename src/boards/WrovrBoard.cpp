// ============================================================
//  WrovrBoard.cpp
// ============================================================
#include "WrovrBoard.h"
#include <Arduino.h>

WrovrBoard::WrovrBoard()
    : _display(WROVER_TFT_BL_PIN),
      _input(WROVER_BTN_UP_PIN, WROVER_BTN_SEL_PIN) {}

void WrovrBoard::init() {
    _display.init();
    _input.init(_display.width(), _display.height());
}
