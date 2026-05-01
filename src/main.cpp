// ============================================================
//  main.cpp  –  ESP32 Runtime OS  –  Entry point
//
//  RIGHT NOW this is a HAL smoke test.  It:
//    1. Boots the board (display + input)
//    2. Draws coloured rectangles using only IDisplay*
//    3. Polls IInput* and prints events to Serial
//
//  Notice: NOT ONE LINE in this file mentions ILI9341,
//  TFT_eSPI, or any GPIO pin number.  All of that lives
//  in boards/.  This file will never change as you add
//  hardware support.
// ============================================================
#include <Arduino.h>
#include "hal/IBoardConfig.h"   // the only HAL header main needs

// The board pointer – global, lives for the entire program.
static IBoardConfig* board   = nullptr;
static IDisplay*     display = nullptr;
static IInput*       input   = nullptr;

// ── Helper: draw a simple test pattern ──────────────────────
static void drawTestPattern() {
    uint16_t w = display->width();
    uint16_t h = display->height();

    display->fillScreen(IDisplay::BLACK);

    // Four coloured quadrants
    display->fillRect(0,     0,     w/2, h/2, IDisplay::RED);
    display->fillRect(w/2,   0,     w/2, h/2, IDisplay::GREEN);
    display->fillRect(0,     h/2,   w/2, h/2, IDisplay::BLUE);
    display->fillRect(w/2,   h/2,   w/2, h/2, IDisplay::YELLOW);

    // White cross-hair in the centre
    display->fillRect(w/2 - 1, 0,     2, h, IDisplay::WHITE);
    display->fillRect(0,       h/2-1, w, 2, IDisplay::WHITE);

    Serial.printf("[HAL] Test pattern drawn.  Screen: %u x %u\n", w, h);
}

// ── Arduino setup ────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(300);  // let the serial monitor connect in Wokwi

    Serial.println("\n========================================");
    Serial.println("  ESP32 Runtime OS  –  HAL smoke test");
    Serial.println("========================================");

    // ── Create and initialise the board ──────────────────────
    // Board::create() is in BoardFactory.cpp.
    // It returns the right concrete board based on the
    // -D BOARD_xxx flag you set in platformio.ini.
    board   = Board::create();
    display = board->getDisplay();
    input   = board->getInput();

    board->init();

    Serial.printf("[HAL] Board ready: %s\n", board->name());
    Serial.printf("[HAL] Display: %u x %u px\n",
                  display->width(), display->height());

    // PSRAM sanity check (WROVER only)
#if defined(BOARD_HAS_PSRAM)
    if (psramFound()) {
        Serial.printf("[HAL] PSRAM: %u KB free\n",
                      esp_get_free_heap_size() / 1024);
    } else {
        Serial.println("[HAL] WARNING: PSRAM not detected!");
    }
#endif

    drawTestPattern();

    Serial.println("[HAL] Press UP or SEL button to test input.");
}

// ── Arduino loop ─────────────────────────────────────────────
void loop() {
    InputEvent evt;
    if (input->read(evt)) {
        const char* typeName = "UNKNOWN";
        switch (evt.type) {
            case InputEventType::PRESS:   typeName = "PRESS";   break;
            case InputEventType::RELEASE: typeName = "RELEASE"; break;
            case InputEventType::MOVE:    typeName = "MOVE";    break;
            default: break;
        }
        Serial.printf("[INPUT] %s  id=%u  x=%d  y=%d\n",
                      typeName, evt.id, evt.x, evt.y);

        // Visual feedback: invert a small square on button press
        if (evt.type == InputEventType::PRESS) {
            uint16_t colour = (evt.id == 0) ? IDisplay::CYAN
                                             : IDisplay::MAGENTA;
            display->fillRect(10, 10, 30, 30, colour);
        }
    }

    delay(10);  // yield – keeps Wokwi simulation smooth
}
