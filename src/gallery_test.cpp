#include <SPI.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SD.h>
#include <JPEGDecoder.h>

#define SD_CS 5 // SD card CS pin
TFT_eSPI tft = TFT_eSPI();

// Declare the function prototype
void displayImage(const char *filename, int xpos, int ypos);

void setup() {
  Serial.begin(115200);

  // Initialize TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("Failed to initialize SD card!");
    tft.setCursor(10, 10);
    tft.println("SD Card Init Failed!");
    while (true);
  }
  Serial.println("SD card initialized.");

  // Call the displayImage function
  displayImage("/1.jpg", 0, 0);
}

void loop() {
  // Do nothing
}

// Define the displayImage function
void displayImage(const char *filename, int xpos, int ypos) {
  Serial.print("Attempting to display: ");
  Serial.println(filename);

  // Open the file
  File file = SD.open(filename);
  if (!file) {
    Serial.println("Failed to open file!");
    tft.setCursor(10, 10);
    tft.println("File Open Failed!");
    return;
  }

  // Decode the JPEG image
  if (!JpegDec.decodeSdFile(file)) {
    Serial.println("JPEG decode failed!");
    file.close();
    return;
  }

  Serial.print("Image size: ");
  Serial.print(JpegDec.width);
  Serial.print("x");
  Serial.println(JpegDec.height);

  // Render the JPEG image
  while (JpegDec.read()) {
    uint16_t *pImg = JpegDec.pImage;
    int mcu_x = JpegDec.MCUx * JpegDec.MCUWidth;
    int mcu_y = JpegDec.MCUy * JpegDec.MCUHeight;
    tft.pushImage(xpos + mcu_x, ypos + mcu_y, JpegDec.MCUWidth, JpegDec.MCUHeight, pImg);
  }

  file.close();
}