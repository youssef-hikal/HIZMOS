#include <Arduino.h>
#include <Adafruit_PCD8544.h>

// Pin definitions for Nokia 5110 and buttons
#define RESET_PIN 8
#define CE_PIN 5
#define DC_PIN 4
#define DIN_PIN 7
#define CLK_PIN 6
#define UP_BUTTON_PIN 28
#define SELECT_BUTTON_PIN 26
#define DOWN_BUTTON_PIN 27

// Initialize display
Adafruit_PCD8544 display(CE_PIN, DC_PIN, DIN_PIN, CLK_PIN, RESET_PIN);

// Menu options
enum Menu { MAIN, NFC, IR, SUB_GHZ, HID };
enum SubMenu { NFC_READ, NFC_EMULATE, NFC_SAVE, NFC_EXIT, IR_READ, IR_EMULATE, IR_SAVE, IR_EXIT, SUB_READ, SUB_EMULATE, SUB_SAVE, SUB_EXIT, HID_RUN, HID_EXIT };

Menu currentMenu = MAIN;
SubMenu currentSubMenu = NFC_READ;
int selectedOption = 0;
bool inSubMenu = false;

void setup() {
  // Initialize display and buttons
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.display();

  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Handle button input
  if (digitalRead(UP_BUTTON_PIN) == LOW) {
    selectedOption = (selectedOption - 1 + getMenuOptionCount()) % getMenuOptionCount();
    delay(200); // Debounce
  } else if (digitalRead(DOWN_BUTTON_PIN) == LOW) {
    selectedOption = (selectedOption + 1) % getMenuOptionCount();
    delay(200); // Debounce
  } else if (digitalRead(SELECT_BUTTON_PIN) == LOW) {
    handleSelect();
    delay(200); // Debounce
  }

  // Display menu
  displayMenu();
}

// Function to get the number of options in the current menu
int getMenuOptionCount() {
  switch (currentMenu) {
    case MAIN: return 4; // Main menu has 4 options
    case NFC: return 4;  // NFC sub-menu has 4 options
    case IR: return 4;   // IR sub-menu has 4 options
    case SUB_GHZ: return 4; // Sub-GHz sub-menu has 4 options
    case HID: return 2;  // HID sub-menu has 2 options
    default: return 0;
  }
}

// Display the current menu
void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  switch (currentMenu) {
    case MAIN:
      display.print(selectedOption == 0 ? "> NFC" : "  NFC");
      display.setCursor(0, 10);
      display.print(selectedOption == 1 ? "> IR" : "  IR");
      display.setCursor(0, 20);
      display.print(selectedOption == 2 ? "> SUB GHZ" : "  SUB GHZ");
      display.setCursor(0, 30);
      display.print(selectedOption == 3 ? "> HID" : "  HID");
      break;
    case NFC:
      display.print(selectedOption == 0 ? "> Read" : "  Read");
      display.setCursor(0, 10);
      display.print(selectedOption == 1 ? "> Emulate" : "  Emulate");
      display.setCursor(0, 20);
      display.print(selectedOption == 2 ? "> Save Tag" : "  Save Tag");
      display.setCursor(0, 30);
      display.print(selectedOption == 3 ? "> Exit" : "  Exit");
      break;
    case IR:
      display.print(selectedOption == 0 ? "> Read" : "  Read");
      display.setCursor(0, 10);
      display.print(selectedOption == 1 ? "> Emulate" : "  Emulate");
      display.setCursor(0, 20);
      display.print(selectedOption == 2 ? "> Save to SD" : "  Save to SD");
      display.setCursor(0, 30);
      display.print(selectedOption == 3 ? "> Exit" : "  Exit");
      break;
    case SUB_GHZ:
      display.print(selectedOption == 0 ? "> Read" : "  Read");
      display.setCursor(0, 10);
      display.print(selectedOption == 1 ? "> Emulate" : "  Emulate");
      display.setCursor(0, 20);
      display.print(selectedOption == 2 ? "> Save to SD" : "  Save to SD");
      display.setCursor(0, 30);
      display.print(selectedOption == 3 ? "> Exit" : "  Exit");
      break;
    case HID:
      display.print(selectedOption == 0 ? "> Run" : "  Run");
      display.setCursor(0, 10);
      display.print(selectedOption == 1 ? "> Exit" : "  Exit");
      break;
  }

  display.display();
}

// Handle selection in the menu
void handleSelect() {
  display.clearDisplay();
  display.setCursor(0, 0);

  if (!inSubMenu) {
    // Main menu selection
    switch (selectedOption) {
      case 0: currentMenu = NFC; display.print("NFC selected"); break;
      case 1: currentMenu = IR; display.print("IR selected"); break;
      case 2: currentMenu = SUB_GHZ; display.print("SUB GHZ selected"); break;
      case 3: currentMenu = HID; display.print("HID selected"); break;
    }
  } else {
    // Sub-menu selection
    switch (currentMenu) {
      case NFC:
        if (selectedOption == 0) display.print("Read NFC selected");
        else if (selectedOption == 1) display.print("Emulate NFC selected");
        else if (selectedOption == 2) display.print("Save Tag NFC selected");
        else if (selectedOption == 3) currentMenu = MAIN; // Exit to main menu
        break;
      case IR:
        if (selectedOption == 0) display.print("Read IR selected");
        else if (selectedOption == 1) display.print("Emulate IR selected");
        else if (selectedOption == 2) display.print("Save IR to SD selected");
        else if (selectedOption == 3) currentMenu = MAIN; // Exit to main menu
        break;
      case SUB_GHZ:
        if (selectedOption == 0) display.print("Read RF selected");
        else if (selectedOption == 1) display.print("Emulate RF selected");
        else if (selectedOption == 2) display.print("Save RF to SD selected");
        else if (selectedOption == 3) currentMenu = MAIN; // Exit to main menu
        break;
      case HID:
        if (selectedOption == 0) display.print("Run HID selected");
        else if (selectedOption == 1) currentMenu = MAIN; // Exit to main menu
        break;
    }
  }

  display.display();
  delay(2000); // Show selection for 2 seconds
}
