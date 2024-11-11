#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h> // For the Nokia 5110 display
#include <SPI.h>
#include <SD.h>
#include <PN532_I2C.h> // For NFC
#include <PN532.h>
#include <WiFi.h>      // For Wi-Fi
#include <BluetoothSerial.h> // For BLE

// Define pins
#define JOY_X_PIN 34
#define JOY_Y_PIN 35
#define BUTTON_PIN 32

// Initialize components
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 12, 14, 27); // Adjust pins to your setup
PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

// Variables for menu navigation
int menuIndex = 0;
int subMenuIndex = 0;
bool inSubMenu = false;
String menuItems[] = {"NFC", "Sub-GHz", "Infrared", "Wi-Fi", "BLE"};
String nfcOptions[] = {"Read", "Emulate", "Brute Force", "Save to SD"};
String subGHzOptions[] = {"Read", "Emulate", "Save to SD", "Brute Force"};
String irOptions[] = {"Read", "Emulate", "Save to SD", "Brute Force"};
String wifiOptions[] = {"Wi-Fi Portal", "Deauth", "Scan Networks"};
String bleOptions[] = {"BLE Jammer", "BLE Spoof"};

void setup() {
  Serial.begin(115200);
  
  // Initialize display
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  
  // Set up SD card
  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
  }
  
  // Set up NFC
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN53x NFC module not found");
  }
  nfc.SAMConfig();
  
  // Joystick and button pins
  pinMode(JOY_X_PIN, INPUT);
  pinMode(JOY_Y_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  showMainMenu();
}

void loop() {
  int xVal = analogRead(JOY_X_PIN);
  int yVal = analogRead(JOY_Y_PIN);
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  // Joystick navigation
  if (!inSubMenu) {
    if (yVal < 1000) {
      menuIndex = (menuIndex + 1) % 5;
      showMainMenu();
      delay(200); // Debounce
    } else if (yVal > 3000) {
      menuIndex = (menuIndex - 1 + 5) % 5;
      showMainMenu();
      delay(200);
    } else if (buttonPressed) {
      inSubMenu = true;
      subMenuIndex = 0;
      showSubMenu(menuIndex);
      delay(200);
    }
  } else {
    if (yVal < 1000) {
      subMenuIndex++;
      showSubMenu(menuIndex);
      delay(200);
    } else if (yVal > 3000) {
      subMenuIndex--;
      showSubMenu(menuIndex);
      delay(200);
    } else if (buttonPressed) {
      // Call the function corresponding to the selected sub-menu item
      executeSubMenuFunction(menuIndex, subMenuIndex);
      delay(200);
    }
  }
}

// Function to display the main menu
void showMainMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Main Menu");
  for (int i = 0; i < 5; i++) {
    if (i == menuIndex) display.print("> ");
    else display.print("  ");
    display.println(menuItems[i]);
  }
  display.display();
}

// Function to display the sub-menu
void showSubMenu(int index) {
  display.clearDisplay();
  display.setCursor(0, 0);
  switch (index) {
    case 0:
      display.println("NFC");
      for (int i = 0; i < 4; i++) {
        if (i == subMenuIndex) display.print("> ");
        else display.print("  ");
        display.println(nfcOptions[i]);
      }
      break;
    case 1:
      display.println("Sub-GHz");
      for (int i = 0; i < 4; i++) {
        if (i == subMenuIndex) display.print("> ");
        else display.print("  ");
        display.println(subGHzOptions[i]);
      }
      break;
    case 2:
      display.println("Infrared");
      for (int i = 0; i < 4; i++) {
        if (i == subMenuIndex) display.print("> ");
        else display.print("  ");
        display.println(irOptions[i]);
      }
      break;
    case 3:
      display.println("Wi-Fi");
      for (int i = 0; i < 3; i++) {
        if (i == subMenuIndex) display.print("> ");
        else display.print("  ");
        display.println(wifiOptions[i]);
      }
      break;
    case 4:
      display.println("BLE");
      for (int i = 0; i < 2; i++) {
        if (i == subMenuIndex) display.print("> ");
        else display.print("  ");
        display.println(bleOptions[i]);
      }
      break;
  }
  display.display();
}

// Placeholder function to execute submenu functions
void executeSubMenuFunction(int mainIndex, int subIndex) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Executing: ");
  switch (mainIndex) {
    case 0:
      display.println(nfcOptions[subIndex]);
      break;
    case 1:
      display.println(subGHzOptions[subIndex]);
      break;
    case 2:
      display.println(irOptions[subIndex]);
      break;
    case 3:
      display.println(wifiOptions[subIndex]);
      break;
    case 4:
      display.println(bleOptions[subIndex]);
      break;
  }
  display.display();
  delay(1000); // Temporary pause
  inSubMenu = false;
  showMainMenu();
}
