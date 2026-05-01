#pragma once
// ============================================================
//  IInput.h  –  Hardware Abstraction Layer : Input
//
//  Abstracts any pointing / button device behind a single
//  event struct.  OS code never knows if it is talking to
//  a resistive touch panel, capacitive touch, or GPIO buttons.
//
//  Java equivalent:  public interface IInput { ... }
// ============================================================
#include <stdint.h>

// ── Input event types ────────────────────────────────────────
enum class InputEventType : uint8_t {
    NONE    = 0,  // no event (read() returned false)
    PRESS   = 1,  // finger / button down
    RELEASE = 2,  // finger / button up
    MOVE    = 3,  // touch dragged while pressed
};

// Single event produced by any input device.
// x/y are in display pixel coordinates (after rotation).
// For physical buttons that have no position, x=0, y=0.
struct InputEvent {
    InputEventType type = InputEventType::NONE;
    int16_t        x    = 0;
    int16_t        y    = 0;
    uint8_t        id   = 0;  // finger/button index (multitouch future use)
};

// ── Abstract interface ────────────────────────────────────────
class IInput {
public:
    virtual ~IInput() = default;

    // Called once at startup after IDisplay::init().
    // For touch drivers this may need the display dimensions
    // to map raw ADC values to screen coordinates.
    virtual void init(uint16_t displayWidth, uint16_t displayHeight) = 0;

    // Poll the device and fill `out` if an event is available.
    // Returns true  → `out` contains a valid event.
    // Returns false → no event; `out` is unchanged.
    // Must be non-blocking (no delay() inside).
    virtual bool read(InputEvent& out) = 0;
};
