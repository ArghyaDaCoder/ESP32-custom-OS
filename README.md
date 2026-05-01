# ESP32 Runtime OS — Layer 0: Hardware Abstraction Layer

## Project structure

```
esp32-runtime-os/
├── platformio.ini          ← build environments (one per board)
├── wokwi.toml              ← Wokwi VS Code simulator config
├── diagram.json            ← Wokwi circuit diagram
└── src/
    ├── main.cpp            ← OS entry point / HAL smoke test
    ├── hal/
    │   ├── IDisplay.h      ← pure abstract display interface
    │   ├── IInput.h        ← pure abstract input interface
    │   └── IBoardConfig.h  ← board factory interface
    └── boards/
        ├── TFTeSPIDisplay.h/.cpp   ← IDisplay via TFT_eSPI (all boards)
        ├── GPIOButtonInput.h/.cpp  ← IInput via GPIO buttons
        ├── WrovrBoard.h/.cpp       ← board config: WROVER + ILI9341
        └── BoardFactory.cpp        ← compile-time board selector
```

## How to build & simulate

```bash
# Build for the WROVER + ILI9341 environment
pio run -e wrover_ili9341

# Then in VS Code: F1 → "Wokwi: Start Simulator"
```

## How to add a new board (e.g. ESP32-S3 + GC9A01)

1. Copy `WrovrBoard.h/.cpp` → `S3MiniBoard.h/.cpp`
2. Change the pin `#define` values in `S3MiniBoard.h`
3. Add the new env to `platformio.ini` (template is already there, commented out)
4. Uncomment the `#elif` block in `BoardFactory.cpp`
5. Done. **Zero changes to `main.cpp` or any HAL header.**

## Wiring (matches diagram.json)

| ESP32 GPIO | ILI9341 pin | Function       |
|-----------|-------------|----------------|
| 23        | SDI (MOSI)  | SPI data       |
| 18        | SCK         | SPI clock      |
| 5         | CS          | Chip select    |
| 2         | DC          | Data/Command   |
| 4         | RST         | Reset          |
| 3V3       | LED/VCC     | Power          |
| GND       | GND         | Ground         |
| 34        | —           | UP button      |
| 35        | —           | SEL button     |

## What to see

When simulation starts you should see:
- Four coloured quadrants (red, green, blue, yellow) on the display
- A white crosshair in the centre
- Serial output: board name, screen size, PSRAM status

Pressing the UP button (GPIO34) flashes a cyan square top-left.
Pressing the SEL button (GPIO35) flashes a magenta square.

## Next step

Layer 1: System Services — LittleFS filesystem mount + PSRAM heap manager.
