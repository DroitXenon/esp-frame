#include <SPI.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SD.h>
#include <JPEGDecoder.h>

// Define CS pin for SD card
#define SD_CS 5

// Initialize TFT
TFT_eSPI tft = TFT_eSPI();

// Function prototypes
void displayImage(const char *filename, int xpos, int ypos);
void renderJPEG(int xpos, int ypos);

void setup() {
  Serial.begin(115200);

  // Initialize TFT display
  tft.init();
  tft.setRotation(1); // Adjust rotation for landscape
  tft.fillScreen(TFT_BLACK);

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("Failed to initialize SD card!");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("SD Card Init Failed!");
    while (true); // Halt execution
  }
  Serial.println("SD card initialized successfully.");

  // Display images in sequence
  displayImage("/1.jpg", 0, 0);
  delay(2000); // Display each image for 2 seconds
  displayImage("/2.jpg", 0, 0);
  delay(2000);
  displayImage("/3.jpg", 0, 0);
}

void loop() {
  // Loop through images continuously
  displayImage("/1.jpg", 0, 0);
  delay(2000);
  displayImage("/2.jpg", 0, 0);
  delay(2000);
  displayImage("/3.jpg", 0, 0);
  delay(2000);
}

// Function to display JPEG image
void displayImage(const char *filename, int xpos, int ypos) {
  Serial.print("Displaying image: ");
  Serial.println(filename);

  // Open the image file
  File file = SD.open(filename);
  if (!file) {
    Serial.println("Failed to open file!");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("Failed to open file!");
    return;
  }

  // Decode and render the image
  if (JpegDec.decodeSdFile(file)) {
    renderJPEG(xpos, ypos);
  } else {
    Serial.println("JPEG decode failed!");
  }

  file.close();
}

// Function to render decoded JPEG
void renderJPEG(int xpos, int ypos) {
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint16_t max_x = JpegDec.width;
  uint16_t max_y = JpegDec.height;

  while (JpegDec.read()) {
    pImg = JpegDec.pImage;
    int mcu_x = JpegDec.MCUx * mcu_w;
    int mcu_y = JpegDec.MCUy * mcu_h;

    if (mcu_x + mcu_w <= max_x && mcu_y + mcu_h <= max_y) {
      tft.pushImage(xpos + mcu_x, ypos + mcu_y, mcu_w, mcu_h, pImg);
    } else if (mcu_x + mcu_w > max_x || mcu_y + mcu_h > max_y) {
      int clipped_w = (mcu_x + mcu_w > max_x) ? max_x - mcu_x : mcu_w;
      int clipped_h = (mcu_y + mcu_h > max_y) ? max_y - mcu_y : mcu_h;
      tft.pushImage(xpos + mcu_x, ypos + mcu_y, clipped_w, clipped_h, pImg);
    }
  }
}