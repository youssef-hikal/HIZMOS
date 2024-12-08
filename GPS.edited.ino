#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// GPS setup
SoftwareSerial gpsSerial(8, 9); // RX, TX
TinyGPSPlus gps;

void setup() {
  // Initialize Serial
  gpsSerial.begin(9600);
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) { // Address 0x3C for most displays
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Infinite loop if initialization fails
  }

  // Clear the display
  display.clearDisplay();
  display.display();
  delay(2000); // Pause for 2 seconds
}

void loop() {
  while (gpsSerial.available()) {
    int data = gpsSerial.read();
    if (gps.encode(data)) {
      // Get GPS data
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      float altitude = gps.altitude.meters();
      float speed = gps.speed.kmph();

      // Print to Serial Monitor
      Serial.print("Latitude: ");
      Serial.println(latitude, 6);
      Serial.print("Longitude: ");
      Serial.println(longitude, 6);
      Serial.print("Altitude: ");
      Serial.println(altitude, 2);
      Serial.print("Speed: ");
      Serial.println(speed, 2);

      // Display on OLED
      display.clearDisplay(); // Clear previous data
      display.setTextSize(1); // Normal text size
      display.setTextColor(SSD1306_WHITE); // White text

      display.setCursor(0, 0);
      display.print("GPS Data:");

      display.setCursor(0, 10);
      display.print("Lat: ");
      display.println(latitude, 6);

      display.setCursor(0, 20);
      display.print("Lon: ");
      display.println(longitude, 6);

      display.setCursor(0, 30);
      display.print("Alt: ");
      display.print(altitude, 2);
      display.println(" m");

      display.setCursor(0, 40);
      display.print("Speed: ");
      display.print(speed, 2);
      display.println(" km/h");

      display.display(); // Update the display
    }
  }
}
