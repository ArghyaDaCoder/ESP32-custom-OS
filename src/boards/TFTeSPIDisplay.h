#pragma once
// ============================================================
//  TFTeSPIDisplay.h  –  IDisplay implemented via TFT_eSPI
//
//  This is the ONE concrete display class for ALL boards.
//  The driver chip difference (ILI9341 vs GC9A01) is handled
//  entirely by TFT_eSPI through the -D flags in platformio.ini.
//  This file never mentions either chip by name.
// ============================================================
#include "../hal/IDisplay.h"
#include <TFT_eSPI.h>

class TFTeSPIDisplay : public IDisplay {
public:
    // backlightPin: GPIO that controls the BL/LED pin.
    // Pass -1 if your board has no software-controlled backlight.
    explicit TFTeSPIDisplay(int8_t backlightPin = -1);

    // ── IDisplay interface ───────────────────────────────────
    void     init()                                       override;
    uint16_t width()                                      override;
    uint16_t height()                                     override;
    void     fillScreen(uint16_t colour)                  override;
    void     fillRect(int32_t x, int32_t y,
                      int32_t w, int32_t h,
                      uint16_t colour)                    override;
    void     drawPixel(int32_t x, int32_t y,
                       uint16_t colour)                   override;
    void     pushPixels(int32_t x, int32_t y,
                        int32_t w, int32_t h,
                        uint16_t* buf)                    override;
    void     setBacklight(uint8_t brightness)             override;

    // ── Direct TFT_eSPI access (for LVGL flush callback) ────
    // LVGL needs a raw TFT_eSPI pointer to call startWrite /
    // endWrite around its DMA transfers.  Only the LVGL glue
    // code should use this; never call it from OS logic.
    TFT_eSPI* raw() { return &_tft; }

private:
    TFT_eSPI _tft;
    int8_t   _backlightPin;
};
