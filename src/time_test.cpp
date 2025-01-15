// #include <WiFi.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #include <TFT_eSPI.h>
// #include <XPT2046_Touchscreen.h>

// // Wi-Fi credentials
// const char* ssid = ""; // Add Wi-Fi name
// const char* password = "";// Add Wi-Fi password

// // NTP settings
// const char* ntpServer = "pool.0ntp.org";
// const long gmtOffset_sec = -5 * 3600;
// const int daylightOffset_sec = 3600;

// // TFT setup
// TFT_eSPI tft = TFT_eSPI();

// // Function prototypes
// void connectToWiFi(); // Declare the function here

// // NTP client
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, 60000);

// void setup() {
//   Serial.begin(115200);

//   // Initialize TFT display
//   tft.init();
//   tft.setRotation(1);
//   tft.fillScreen(TFT_WHITE);
//   tft.setTextColor(TFT_BLACK, TFT_WHITE);
//   tft.setTextSize(2);

//   // Connect to Wi-Fi
//   connectToWiFi();

//   // Initialize NTP client
//   timeClient.begin();
// }

// void loop() {
//   timeClient.update();
//   String formattedTime = timeClient.getFormattedTime();

//   tft.fillScreen(TFT_WHITE);
//   tft.setCursor(10, 50);
//   tft.setTextSize(3);
//   tft.setTextColor(TFT_BLACK, TFT_WHITE);
//   tft.println("Current Time:");
//   tft.setCursor(10, 100);
//   tft.println(formattedTime);

//   delay(1000);
// }

// void connectToWiFi() {
//   tft.setCursor(10, 20);
//   tft.print("Connecting to Wi-Fi...");
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     tft.print(".");
//     Serial.print(".");
//   }

//   tft.fillScreen(TFT_WHITE);
//   tft.setCursor(10, 20);
//   tft.print("Wi-Fi Connected!");
//   Serial.println("\nWi-Fi Connected!");
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());
// }