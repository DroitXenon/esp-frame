#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TFT_eSPI.h> 

// Wi-Fi credentials
const char* ssid = "Droit's iPhone 14 Pro";
const char* password = "imr15336996002";

// NTP settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0; // Adjust based on your timezone (e.g., -5 * 3600 for EST)
const int daylightOffset_sec = 3600; // Adjust for DST if applicable

// TFT setup
TFT_eSPI tft = TFT_eSPI(); 

// NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, 60000);

void setup() {
  Serial.begin(115200);

  // Initialize TFT
  tft.init();
  tft.setRotation(1); // Adjust rotation based on your preference
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  // Connect to Wi-Fi
  connectToWiFi();

  // Initialize NTP client
  timeClient.begin();
  timeClient.update();
}

void loop() {
  // Update time from NTP server
  timeClient.update();

  // Get the current time
  String formattedTime = timeClient.getFormattedTime();

  // Display time on the TFT screen
  tft.fillScreen(TFT_BLACK); // Clear the screen
  tft.setCursor(10, 50);     // Set position
  tft.print("Time: ");
  tft.println(formattedTime);

  delay(1000); // Update every second
}

void connectToWiFi() {
  tft.setCursor(10, 20);
  tft.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
    Serial.print(".");
  }

  tft.fillScreen(TFT_BLACK); // Clear the screen after connection
  tft.setCursor(10, 20);
  tft.print("Wi-Fi Connected!");
  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}