#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); 

// 1. The OS Variables
int systemUptime = 0;
int simulatedMemoryLoad = 100;

// 2. The Mutex (The Keycard)
// We use this to stop Core 1 from reading variables at the EXACT 
// millisecond Core 0 is writing to them.
SemaphoreHandle_t osDataMutex; 

// ---------------------------------------------------------
// CORE 0: THE BACKEND SYSTEM (Runs in the background)
// ---------------------------------------------------------
void backendTask(void *pvParameters) {
    while(true) {
        // Lock the data, update it, unlock it
        if (xSemaphoreTake(osDataMutex, portMAX_DELAY)) {
            systemUptime++;
            simulatedMemoryLoad = random(100, 800); // Fake memory flux
            xSemaphoreGive(osDataMutex);
        }

        // Print to the serial monitor so we know Core 0 is alive
        Serial.printf("[Core 0] Backend Tick - Uptime: %d\n", systemUptime);
        
        // Sleep for exactly 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }
}

// ---------------------------------------------------------
// SETUP: THE BOOTLOADER
// ---------------------------------------------------------
void setup() {
    Serial.begin(115200);
    
    // Create the Mutex Key
    osDataMutex = xSemaphoreCreateMutex();

    // Boot Graphics
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // Draw Static UI (Things that never change)
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("--- ArghyaOS v1.0 ---");

    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setCursor(10, 60);
    tft.print("CPU0: Backend Logic");
    tft.setCursor(10, 90);
    tft.print("CPU1: GUI Engine");

    // Spawn the Backend Task on Core 0
    xTaskCreatePinnedToCore(
        backendTask,    // The function to run
        "Backend",      // Name for debugging
        4000,           // Stack size (Memory reserved for this task)
        NULL,           // Parameters
        1,              // Priority
        NULL,           // Task Handle
        0               // Pin to Core 0
    );
}

// ---------------------------------------------------------
// CORE 1: THE FRONTEND GUI (Arduino loop() defaults to Core 1)
// ---------------------------------------------------------
void loop() {
    int currentUptime = 0;
    int currentMem = 0;

    // 1. Grab the latest data from the backend safely
    if (xSemaphoreTake(osDataMutex, portMAX_DELAY)) {
        currentUptime = systemUptime;
        currentMem = simulatedMemoryLoad;
        xSemaphoreGive(osDataMutex);
    }

    // 2. Render the changing data to the screen
    // Notice we pass TFT_BLACK as the second color. This erases the old numbers automatically!
    tft.setTextColor(TFT_GREEN, TFT_BLACK); 
    tft.setTextSize(3);
    
    tft.setCursor(10, 150);
    tft.printf("UPTIME: %04d sec", currentUptime);

    tft.setTextColor(TFT_YELLOW, TFT_BLACK); 
    tft.setCursor(10, 200);
    tft.printf("RAM: %04d KB", currentMem);

    // Run the GUI at roughly 20 Frames Per Second (FPS)
    vTaskDelay(50 / portTICK_PERIOD_MS); 
}