#pragma once
// ============================================================
//  WrovrBoard.h  –  Concrete IBoardConfig for ESP32-WROVER
//                   + ILI9341 (240×320)
//
//  Pin assignments match the Wokwi diagram.json below.
//  Change the #define values here if your physical wiring
//  differs – nowhere else.
// ============================================================
#include "../hal/IBoardConfig.h"
#include "TFTeSPIDisplay.h"
#include "GPIOButtonInput.h"

// ── Pin assignments ──────────────────────────────────────────
// SPI is configured by TFT_eSPI via platformio.ini build_flags.
// Only non-SPI pins live here.
#define WROVER_TFT_BL_PIN   -1   // -1 = no software backlight control
#define WROVER_BTN_UP_PIN   32   // GPIO34 – UP button   (Wokwi diagram)
#define WROVER_BTN_SEL_PIN  33   // GPIO35 – SEL button  (Wokwi diagram)

class WrovrBoard : public IBoardConfig {
public:
    WrovrBoard();

    void     init()       override;
    IDisplay* getDisplay() override { return &_display; }
    IInput*   getInput()   override { return &_input;   }
    const char* name()     override { return "ESP32-WROVER / ILI9341"; }

private:
    TFTeSPIDisplay _display;
    GPIOButtonInput _input;
};
