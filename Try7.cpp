#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Nokia 5110 LCD display pin setup
#define PIN_RESET 8
#define PIN_CE 15
#define PIN_DC 4
#define PIN_DIN 7
#define PIN_CLK 6

// Button pin setup
#define BUTTON_UP 15
#define BUTTON_SELECT 16
#define BUTTON_DOWN 17

// Menu states
enum MenuState { MAIN_MENU, NFC_MENU, SUB_GHZ_MENU, IR_MENU, SD_LIST_MENU, HID_MENU };
MenuState currentMenu = MAIN_MENU;
int currentSelection = 0; // Tracks the current selection in main menu
int currentSubSelection = 0; // Tracks the current selection in sub-menus

// Initialize display
Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RESET);

void setup() {
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.display();

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  showMainMenu();
}

void loop() {
  handleMenuNavigation();
}

void showMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Main Menu:");
  display.setCursor(0, 10);
  display.print(currentSelection == 0 ? "> NFC" : "  NFC");
  display.setCursor(0, 20);
  display.print(currentSelection == 1 ? "> Sub GHz" : "  Sub GHz");
  display.setCursor(0, 30);
  display.print(currentSelection == 2 ? "> Infrared" : "  Infrared");
  display.setCursor(0, 40);
  display.print(currentSelection == 3 ? "> SD List" : "  SD List");
  display.setCursor(0, 50);
  display.print(currentSelection == 4 ? "> HID" : "  HID");
  display.display();
}

void handleMenuNavigation() {
  bool upPressed = digitalRead(BUTTON_UP) == LOW;
  bool downPressed = digitalRead(BUTTON_DOWN) == LOW;
  bool selectPressed = digitalRead(BUTTON_SELECT) == LOW;

  if (upPressed) {
    currentSelection = (currentSelection - 1 + 5) % 5; // Wrap around menu items
    showMainMenu();
    delay(200);
  }
  if (downPressed) {
    currentSelection = (currentSelection + 1) % 5; // Wrap around menu items
    showMainMenu();
    delay(200);
  }
  if (selectPressed) {
    if (currentMenu == MAIN_MENU) {
      switch (currentSelection) {
        case 0:
          currentMenu = NFC_MENU;
          currentSubSelection = 0;
          showNFCMenu();
          break;
        case 1:
          currentMenu = SUB_GHZ_MENU;
          currentSubSelection = 0;
          showSubGHzMenu();
          break;
        case 2:
          currentMenu = IR_MENU;
          currentSubSelection = 0;
          showIRMenu();
          break;
        case 3:
          currentMenu = SD_LIST_MENU;
          currentSubSelection = 0;
          showSDListMenu();
          break;
        case 4:
          currentMenu = HID_MENU;
          currentSubSelection = 0;
          showHIDMenu();
          break;
      }
    } else {
      handleSubMenuSelection();
    }
    delay(200);
  }
}

void handleSubMenuSelection() {
  switch (currentMenu) {
    case NFC_MENU:
      if (currentSubSelection == 4) handleBackToMainMenu(); // Exit to main menu
      else showNFCMenu();
      break;
    case SUB_GHZ_MENU:
      if (currentSubSelection == 4) handleBackToMainMenu(); // Exit to main menu
      else showSubGHzMenu();
      break;
    case IR_MENU:
      if (currentSubSelection == 4) handleBackToMainMenu(); // Exit to main menu
      else showIRMenu();
      break;
    case SD_LIST_MENU:
      if (currentSubSelection == 3) handleBackToMainMenu(); // Exit to main menu
      else showSDListMenu();
      break;
    case HID_MENU:
      if (currentSubSelection == 2) handleBackToMainMenu(); // Exit to main menu
      else showHIDMenu();
      break;
  }
}

void handleBackToMainMenu() {
  currentMenu = MAIN_MENU;
  showMainMenu();
}

void showNFCMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("NFC Menu:");
  display.setCursor(0, 10);
  display.print(currentSubSelection == 0 ? "> Read" : "  Read");
  display.setCursor(0, 20);
  display.print(currentSubSelection == 1 ? "> Emulate" : "  Emulate");
  display.setCursor(0, 30);
  display.print(currentSubSelection == 2 ? "> Brute Force" : "  Brute Force");
  display.setCursor(0, 40);
  display.print(currentSubSelection == 3 ? "> Save to SD" : "  Save to SD");
  display.setCursor(0, 50);
  display.print(currentSubSelection == 4 ? "> Exit" : "  Exit");
  display.display();
}

void showSubGHzMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Sub GHz Menu:");
  display.setCursor(0, 10);
  display.print(currentSubSelection == 0 ? "> Read" : "  Read");
  display.setCursor(0, 20);
  display.print(currentSubSelection == 1 ? "> Emulate" : "  Emulate");
  display.setCursor(0, 30);
  display.print(currentSubSelection == 2 ? "> Save to SD" : "  Save to SD");
  display.setCursor(0, 40);
  display.print(currentSubSelection == 3 ? "> Brute Force" : "  Brute Force");
  display.setCursor(0, 50);
  display.print(currentSubSelection == 4 ? "> Exit" : "  Exit");
  display.display();
}

void showIRMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("IR Menu:");
  display.setCursor(0, 10);
  display.print(currentSubSelection == 0 ? "> Read" : "  Read");
  display.setCursor(0, 20);
  display.print(currentSubSelection == 1 ? "> Emulate" : "  Emulate");
  display.setCursor(0, 30);
  display.print(currentSubSelection == 2 ? "> Save to SD" : "  Save to SD");
  display.setCursor(0, 40);
  display.print(currentSubSelection == 3 ? "> Brute Force" : "  Brute Force");
  display.setCursor(0, 50);
  display.print(currentSubSelection == 4 ? "> Exit" : "  Exit");
  display.display();
}

void showSDListMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("SD List Menu:");
  display.setCursor(0, 10);
  display.print(currentSubSelection == 0 ? "> NFC Tags" : "  NFC Tags");
  display.setCursor(0, 20);
  display.print(currentSubSelection == 1 ? "> IR Signals" : "  IR Signals");
  display.setCursor(0, 30);
  display.print(currentSubSelection == 2 ? "> Sub GHz" : "  Sub GHz");
  display.setCursor(0, 40);
  display.print(currentSubSelection == 3 ? "> Exit" : "  Exit");
  display.display();
}

void showHIDMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("HID Menu:");
  display.setCursor(0, 10);
  display.print(currentSubSelection == 0 ? "> Scripts" : "  Scripts");
  display.setCursor(0, 20);
  display.print(currentSubSelection == 1 ? "> Run" : "  Run");
  display.setCursor(0, 30);
  display.print(currentSubSelection == 2 ? "> Exit" : "  Exit");
  display.display();
}

void handleSubMenuNavigation() {
  bool upPressed = digitalRead(BUTTON_UP) == LOW;
  bool downPressed = digitalRead(BUTTON_DOWN) == LOW;
  bool selectPressed = digitalRead(BUTTON_SELECT) == LOW;

  if (upPressed) {
    currentSubSelection = (currentSubSelection - 1 + 5) % 5; // Wrap around menu items
    showSubMenu();
    delay(200);
  }
  if (downPressed) {
    currentSubSelection = (currentSubSelection + 1) % 5; // Wrap around menu items
    showSubMenu();
    delay(200);
  }
  if (selectPressed) {
    if (currentSubSelection == 4) {
      handleBackToMainMenu(); // Exit to main menu
    }
    // Add specific functionality
