#include <Arduino.h>
#include <TFT_eSPI.h> 

// Instantiate the Graphics Engine
TFT_eSPI tft = TFT_eSPI(); 

void setup() {
    Serial.begin(115200);
    Serial.println("Booting Video Driver...");

    // 1. Start the screen hardware
    tft.init();
    
    // 2. Set rotation to landscape (1 or 3). 0 and 2 are portrait.
    tft.setRotation(1); 
    
    // 3. Paint the entire background RED
    tft.fillScreen(TFT_RED);
    
    // 4. Draw our first UI text
    tft.setTextColor(TFT_WHITE, TFT_RED); // White text, Red background
    tft.setTextSize(3);                   // Make it big
    
    // Set the X, Y coordinates and print
    tft.setCursor(40, 100);
    tft.println("OS INITIALIZED!");
    
    Serial.println("Screen successfully painted!");
}

void loop() {
    // The screen holds its own memory (framebuffer). 
    // We don't need to loop anything to keep the image there!
    delay(1000);
}