#pragma once
// ============================================================
//  IBoardConfig.h  –  Hardware Abstraction Layer : Board
//
//  A "board config" owns one IDisplay and one IInput and
//  exposes them through getters.  main.cpp calls
//  Board::create() which returns the right concrete board
//  selected at compile time via the -D BOARD_xxx flag.
//
//  Adding a new board = adding one .cpp file and one case
//  in Board::create().  Nothing else changes.
// ============================================================
#include "IDisplay.h"
#include "IInput.h"

class IBoardConfig {
public:
    virtual ~IBoardConfig() = default;

    // One-time hardware initialisation.
    // Calls display->init() and input->init() internally.
    virtual void init() = 0;

    // Accessors – return raw pointers; lifetime is the
    // program lifetime, so no ownership transfer.
    virtual IDisplay* getDisplay() = 0;
    virtual IInput*   getInput()   = 0;

    // Human-readable board name for debug output.
    virtual const char* name() = 0;
};

// ── Factory ───────────────────────────────────────────────────
// Implemented in boards/BoardFactory.cpp
// Returns a heap-allocated IBoardConfig selected at compile
// time by the -D BOARD_xxx flag in platformio.ini.
namespace Board {
    IBoardConfig* create();
}
