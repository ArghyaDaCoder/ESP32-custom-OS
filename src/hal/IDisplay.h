#pragma once
// ============================================================
//  IDisplay.h  –  Hardware Abstraction Layer : Display
//
//  This is the ONLY display header that any OS code above
//  the HAL will ever #include.  It has zero knowledge of
//  ILI9341, GC9A01, TFT_eSPI, or any concrete driver.
//
//  Java equivalent:  public interface IDisplay { ... }
// ============================================================
#include <stdint.h>

class IDisplay {
public:
    // Always add a virtual destructor to abstract base classes.
    // Java does this for you; C++ requires it explicitly.
    virtual ~IDisplay() = default;

    // ── Lifecycle ────────────────────────────────────────────
    // Called once at startup.  Driver does SPI init, reset,
    // sends init command sequence, clears screen.
    virtual void init() = 0;

    // ── Dimensions ───────────────────────────────────────────
    // Return logical width/height AFTER any rotation is applied.
    // e.g. ILI9341 in landscape → width()=320, height()=240
    virtual uint16_t width()  = 0;
    virtual uint16_t height() = 0;

    // ── Drawing primitives ───────────────────────────────────
    // Fill entire screen with one colour.
    virtual void fillScreen(uint16_t colour) = 0;

    // Fill an axis-aligned rectangle.
    virtual void fillRect(int32_t x, int32_t y,
                          int32_t w, int32_t h,
                          uint16_t colour) = 0;

    // Draw a single pixel.
    virtual void drawPixel(int32_t x, int32_t y, uint16_t colour) = 0;

    // ── Bulk pixel transfer (used by LVGL flush callback) ────
    // Copy a w×h block of RGB565 pixels to screen at (x, y).
    // `buf` must contain exactly w*h uint16_t values.
    // This is the HOT PATH – keep it as thin as possible.
    virtual void pushPixels(int32_t x, int32_t y,
                            int32_t w, int32_t h,
                            uint16_t* buf) = 0;

    // ── Backlight ────────────────────────────────────────────
    // brightness: 0 = off, 255 = full.
    // Boards without PWM backlight can ignore the value and
    // treat any non-zero as "on".
    virtual void setBacklight(uint8_t brightness) = 0;

    // ── Colour helper (static, shared by all boards) ─────────
    // Pack 8-bit R,G,B into a 16-bit RGB565 word.
    // Same formula used internally by TFT_eSPI's color565().
    static constexpr uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
        return (uint16_t)(((r & 0xF8u) << 8) |
                          ((g & 0xFCu) << 3) |
                           (b >> 3));
    }

    // Predefined colours (RGB565).  Add more as needed.
    static constexpr uint16_t BLACK   = 0x0000;
    static constexpr uint16_t WHITE   = 0xFFFF;
    static constexpr uint16_t RED     = 0xF800;
    static constexpr uint16_t GREEN   = 0x07E0;
    static constexpr uint16_t BLUE    = 0x001F;
    static constexpr uint16_t YELLOW  = 0xFFE0;
    static constexpr uint16_t CYAN    = 0x07FF;
    static constexpr uint16_t MAGENTA = 0xF81F;
    static constexpr uint16_t ORANGE  = 0xFD20;
    static constexpr uint16_t GRAY    = 0x7BEF;
};
