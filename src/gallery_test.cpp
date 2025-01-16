#include <TFT_eSPI.h>
#include <SD.h>
#include <JPEGDEC.h>

// SD card CS pin
#define SD_CS 5

// TFT display and JPEG decoder
TFT_eSPI tft = TFT_eSPI();
JPEGDEC jpeg;

// SD card and file management
File root;
File jpgFile;
int16_t currentIndex = 0;
uint16_t fileCount = 0;

// Timer and button management
uint32_t timer;
volatile bool buttonPressed = false;

// Button interrupt handler
void IRAM_ATTR buttonInt() {
    buttonPressed = true;
}

// JPEG file access functions
void *myOpen(const char *filename, int32_t *size) {
    jpgFile = SD.open(filename);
    if (!jpgFile) {
        Serial.println("Failed to open file!");
        return nullptr;
    }
    *size = jpgFile.size();
    return &jpgFile;
}

void myClose(void *handle) {
    if (jpgFile) jpgFile.close();
}

int32_t myRead(JPEGFILE *handle, uint8_t *buffer, int32_t length) {
    if (!jpgFile) return 0;
    return jpgFile.read(buffer, length);
}

int32_t mySeek(JPEGFILE *handle, int32_t position) {
    if (!jpgFile) return 0;
    return jpgFile.seek(position);
}

// Draw the JPEG image on the TFT display
int JPEGDraw(JPEGDRAW *pDraw) {
    tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
    return 1;
}

// Decode and display a JPEG image
void decodeJpeg(const char *name) {
    char fullPath[100];
    snprintf(fullPath, sizeof(fullPath), "/%s", name); // Add "/" prefix
    if (!jpeg.open(fullPath, myOpen, myClose, myRead, mySeek, JPEGDraw)) {
        Serial.println("Failed to open JPEG file!");
        return;
    }

    // Clear the background if the image doesn't fill the screen
    if (jpeg.getWidth() < tft.width() || jpeg.getHeight() < tft.height()) {
        tft.fillScreen(TFT_BLACK);
    }

    jpeg.decode((tft.width() - jpeg.getWidth()) / 2, (tft.height() - jpeg.getHeight()) / 2, 0);
    jpeg.close();
}

// Load a JPEG image by index
void loadImage(uint16_t targetIndex) {
    if (targetIndex >= fileCount) targetIndex = 0;

    root = SD.open("/");
    if (!root) {
        Serial.println("Failed to open root directory!");
        return;
    }

    uint16_t index = 0;
    File entry;

    while ((entry = root.openNextFile())) {
        if (!entry.isDirectory()) {
            String name = entry.name();
            if (name.endsWith(".jpg") || name.endsWith(".JPG")) {
                if (index == targetIndex) {
                    Serial.print("Displaying: ");
                    Serial.println(name);
                    decodeJpeg(name.c_str());
                    entry.close();
                    return;
                }
                index++;
            }
        }
        entry.close();
    }

    Serial.println("Could not load file.");
}

// Handle errors by displaying them on the TFT
void error(const char *msg) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println(msg);
    while (true) delay(1000);
}

// Setup function
void setup() {
    Serial.begin(115200);

    // Initialize the button
    pinMode(0, INPUT_PULLUP);
    attachInterrupt(0, buttonInt, FALLING);

    // Initialize the TFT display
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    // Initialize the SD card
    if (!SD.begin(SD_CS)) {
        error("SD card initialization failed!");
    }

    root = SD.open("/");
    if (!root) {
        error("Failed to open root directory!");
    }

    // Count the number of JPEG files
    File entry;
    while ((entry = root.openNextFile())) {
        if (!entry.isDirectory()) {
            String name = entry.name();
            if (name.endsWith(".jpg") || name.endsWith(".JPG")) {
                fileCount++;
            }
        }
        entry.close();
    }

    if (fileCount == 0) {
        error("No JPEG files found!");
    }

    Serial.printf("Found %d JPEG files.\n", fileCount);

    // Display the first image
    loadImage(currentIndex);
    timer = millis();
}

// Main loop
void loop() {
    if (millis() - timer > 10000 || buttonPressed) {
        buttonPressed = false;
        currentIndex = (currentIndex + 1) % fileCount;
        loadImage(currentIndex);
        timer = millis();
    }
}