#include <Adafruit_PCD8544.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Nokia 5110 connections
#define RST 14
#define CE   5
#define DC  19
#define DIN 23
#define CLK 18

// GPS Serial configuration
#define GPS_RX 16
#define GPS_TX 17

Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CE, RST);
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // Initialize display
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  // Show initial message
  display.setCursor(0, 0);
  display.println("Waiting for");
  display.println("GPS data...");
  display.display();
  delay(500);
}

void loop() {
  // Read GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayGPSData();
    }
  }

  // If no valid data for 5 seconds, show warning
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("No GPS data");
    display.println("Check wiring!");
    display.display();
    while(true);
  }
}

void displayGPSData() {
  display.clearDisplay();
  display.setCursor(0, 0);

  // Display latitude
  display.print("Lat: ");
  if (gps.location.isValid()) {
    display.println(gps.location.lat(), 6);
  } else {
    display.println("Invalid");
  }

  // Display longitude
  display.print("Lon: ");
  if (gps.location.isValid()) {
    display.println(gps.location.lng(), 6);
  } else {
    display.println("Invalid");
  }

  // Display speed
  display.print("Speed:");
  if (gps.speed.isValid()) {
    display.print(gps.speed.kmph());
    display.println("km/h");
  } else {
    display.println("Invalid");
  }

  // Display satellites
  display.print("Sats: ");
  if (gps.satellites.isValid()) {
    display.println(gps.satellites.value());
  } else {
    display.println("0");
  }

  display.display();
}
