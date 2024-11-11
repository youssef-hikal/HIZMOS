#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SD.h>

// Display pins
#define RESET_PIN 8
#define CE_PIN 15
#define DC_PIN 4
#define DIN_PIN 7
#define CLK_PIN 6

// Button pins
#define UP_BUTTON_PIN 15
#define SELECT_BUTTON_PIN 16
#define DOWN_BUTTON_PIN 17

// Display setup
Adafruit_PCD8544 display = Adafruit_PCD8544(CE_PIN, DC_PIN, DIN_PIN, CLK_PIN, RESET_PIN);

// Variables for menu navigation
int currentMenu = 0; // Main menu
int selectedOption = 0;

// Menu names and structure
const char* mainMenu[] = {"NFC", "Sub-GHz", "Infrared", "SD List", "HID"};
const char* nfcMenu[] = {"Read Tag", "Emulate Tag", "Brute Force", "Save to SD", "Back"};
const char* subGHzMenu[] = {"Read", "Emulate", "Save to SD", "Brute Force", "Back"};
const char* infraredMenu[] = {"Read", "Save", "Emulate", "Brute Force", "Back"};
const char* sdMenu[] = {"View Files", "Back"};
const char* hidMenu[] = {"Run Script", "Back"};

int mainMenuSize = sizeof(mainMenu) / sizeof(mainMenu[0]);
int nfcMenuSize = sizeof(nfcMenu) / sizeof(nfcMenu[0]);
int subGHzMenuSize = sizeof(subGHzMenu) / sizeof(subGHzMenu[0]);
int infraredMenuSize = sizeof(infraredMenu) / sizeof(infraredMenu[0]);
int sdMenuSize = sizeof(sdMenu) / sizeof(sdMenu[0]);
int hidMenuSize = sizeof(hidMenu) / sizeof(hidMenu[0]);

void setup() {
  // Initialize display and buttons
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.display();
  
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  
  // Display initial menu
  displayMainMenu();
}

void loop() {
  // Handle button presses for menu navigation
  if (digitalRead(UP_BUTTON_PIN) == LOW) {
    selectedOption = (selectedOption - 1 + getMenuSize()) % getMenuSize();
    delay(200); // Debounce
    updateMenu();
  }
  
  if (digitalRead(DOWN_BUTTON_PIN) == LOW) {
    selectedOption = (selectedOption + 1) % getMenuSize();
    delay(200); // Debounce
    updateMenu();
  }
  
  if (digitalRead(SELECT_BUTTON_PIN) == LOW) {
    selectOption();
    delay(200); // Debounce
  }
}

void displayMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Main Menu:");
  for (int i = 0; i < mainMenuSize; i++) {
    if (i == selectedOption) display.print("> ");
    else display.print("  ");
    display.println(mainMenu[i]);
  }
  display.display();
}

void updateMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  const char** menu;
  int menuSize;

  switch (currentMenu) {
    case 0: menu = mainMenu; menuSize = mainMenuSize; display.setCursor(0, 0); display.println("Main Menu:"); break;
    case 1: menu = nfcMenu; menuSize = nfcMenuSize; display.setCursor(0, 0); display.println("NFC Menu:"); break;
    case 2: menu = subGHzMenu; menuSize = subGHzMenuSize; display.setCursor(0, 0); display.println("Sub-GHz Menu:"); break;
    case 3: menu = infraredMenu; menuSize = infraredMenuSize; display.setCursor(0, 0); display.println("Infrared Menu:"); break;
    case 4: menu = sdMenu; menuSize = sdMenuSize; display.setCursor(0, 0); display.println("SD Menu:"); break;
    case 5: menu = hidMenu; menuSize = hidMenuSize; display.setCursor(0, 0); display.println("HID Menu:"); break;
  }

  for (int i = 0; i < menuSize; i++) {
    if (i == selectedOption) display.print("> ");
    else display.print("  ");
    display.println(menu[i]);
  }
  display.display();
}

void selectOption() {
  switch (currentMenu) {
    case 0: // Main menu
      currentMenu = selectedOption + 1;
      selectedOption = 0;
      updateMenu();
      break;
    default: // Sub-menu
      if (selectedOption == getMenuSize() - 1) { // Back option
        currentMenu = 0;
        selectedOption = 0;
        displayMainMenu();
      } else {
        // Placeholder for sub-menu actions
      }
      break;
  }
}

int getMenuSize() {
  switch (currentMenu) {
    case 0: return mainMenuSize;
    case 1: return nfcMenuSize;
    case 2: return subGHzMenuSize;
    case 3: return infraredMenuSize;
    case 4: return sdMenuSize;
    case 5: return hidMenuSize;
    default: return 0;
  }
}
