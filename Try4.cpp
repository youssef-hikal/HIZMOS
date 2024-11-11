#include <SPI.h>
#include <SD.h>
#include <Adafruit_PCD8544.h> // Use Nokia 5110 library
#include <Wire.h>

#define BUTTON_UP_PIN 6
#define BUTTON_SELECT_PIN 17
#define BUTTON_DOWN_PIN 18

// Initialize the Nokia 5110 LCD
Adafruit_PCD8544 display = Adafruit_PCD8544(4, 7, 8, 15, 6); // Reset, Data/Command, Chip Enable, Clock, DataIn

enum MenuOption {
  MAIN_MENU,
  NFC_MENU,
  SUB_GHZ_MENU,
  IR_MENU,
  SD_LIST_MENU,
  HID_MENU
};

MenuOption currentMenu = MAIN_MENU;
int currentSelection = 0;

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  
  // Initialize buttons
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  // Initialize the display
  display.begin();
  display.setContrast(50); // Adjust contrast as needed
  
  // Initialize SD card
  if (!SD.begin(14)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("SD Card Error");
    display.display();
    while (true); // Halt execution if SD card is not detected
  }

  // Show the main menu
  displayMainMenu();
}

void loop() {
  // Handle button inputs
  if (digitalRead(BUTTON_UP_PIN) == LOW) {
    currentSelection = (currentSelection - 1 + 5) % 5; // Wrap around menu items
    delay(200); // Debounce delay
  }
  if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
    currentSelection = (currentSelection + 1) % 5; // Wrap around menu items
    delay(200); // Debounce delay
  }
  if (digitalRead(BUTTON_SELECT_PIN) == LOW) {
    selectMenuItem();
    delay(200); // Debounce delay
  }

  // Refresh the display based on the current menu
  displayMenu();
}

void displayMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  switch (currentMenu) {
    case MAIN_MENU:
      displayMainMenu();
      break;
    case NFC_MENU:
      displayNFCMenu();
      break;
    case SUB_GHZ_MENU:
      displaySubGHzMenu();
      break;
    case IR_MENU:
      displayIRMenu();
      break;
    case SD_LIST_MENU:
      displaySDListMenu();
      break;
    case HID_MENU:
      displayHIDMenu();
      break;
  }
}

void displayMainMenu() {
  display.println("Main Menu:");
  displayMenuItem("NFC", 0);
  displayMenuItem("Sub GHz", 1);
  displayMenuItem("Infrared", 2);
  displayMenuItem("SD List", 3);
  displayMenuItem("HID", 4);
  display.display();
}

void displayMenuItem(String label, int index) {
  if (index == currentSelection) {
    display.setInverseDisplay(true);
  } else {
    display.setInverseDisplay(false);
  }
  display.println(label);
}

void selectMenuItem() {
  switch (currentMenu) {
    case MAIN_MENU:
      switch (currentSelection) {
        case 0:
          currentMenu = NFC_MENU;
          break;
        case 1:
          currentMenu = SUB_GHZ_MENU;
          break;
        case 2:
          currentMenu = IR_MENU;
          break;
        case 3:
          currentMenu = SD_LIST_MENU;
          break;
        case 4:
          currentMenu = HID_MENU;
          break;
      }
      break;
    case NFC_MENU:
    case SUB_GHZ_MENU:
    case IR_MENU:
    case HID_MENU:
    case SD_LIST_MENU:
      // Here you can handle specific actions like reading, emulating, etc.
      break;
  }
  currentSelection = 0; // Reset selection after going to a sub-menu
}

void displayNFCMenu() {
  display.println("NFC Menu:");
  displayMenuItem("Read", 0);
  displayMenuItem("Emulate", 1);
  displayMenuItem("Brute Force", 2);
  displayMenuItem("Save to SD", 3);
  displayMenuItem("Exit", 4);
  display.display();
}

void displaySubGHzMenu() {
  display.println("Sub GHz Menu:");
  displayMenuItem("Read", 0);
  displayMenuItem("Emulate", 1);
  displayMenuItem("Save to SD", 2);
  displayMenuItem("Brute Force", 3);
  displayMenuItem("Exit", 4);
  display.display();
}

void displayIRMenu() {
  display.println("IR Menu:");
  displayMenuItem("Read", 0);
  displayMenuItem("Emulate", 1);
  displayMenuItem("Save to SD", 2);
  displayMenuItem("Brute Force", 3);
  displayMenuItem("Exit", 4);
  display.display();
}

void displaySDListMenu() {
  display.println("SD List Menu:");
  displayMenuItem("Saved NFC Tags", 0);
  displayMenuItem("Saved IR Signals", 1);
  displayMenuItem("Saved Sub GHz", 2);
  displayMenuItem("Exit", 3);
  display.display();
}

void displayHIDMenu() {
  display.println("HID Menu:");
  displayMenuItem("Scripts", 0);
  displayMenuItem("Run", 1);
  displayMenuItem("Exit", 2);
  display.display();
}
