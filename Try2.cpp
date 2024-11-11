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
int currentSelection = 0;

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
    switch (currentSelection) {
      case 0:
        currentMenu = NFC_MENU;
        showNFCMenu();
        break;
      case 1:
        currentMenu = SUB_GHZ_MENU;
        showSubGHzMenu();
        break;
      case 2:
        currentMenu = IR_MENU;
        showIRMenu();
        break;
      case 3:
        currentMenu = SD_LIST_MENU;
        showSDListMenu();
        break;
      case 4:
        currentMenu = HID_MENU;
        showHIDMenu();
        break;
    }
    delay(200);
  }
}

void showNFCMenu() {
  // Example of showing NFC sub-menu
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("NFC Menu:");
  display.setCursor(0, 10);
  display.print("1. Read");
  display.setCursor(0, 20);
  display.print("2. Emulate");
  display.setCursor(0, 30);
  display.print("3. Brute Force");
  display.setCursor(0, 40);
  display.print("4. Save to SD");
  display.setCursor(0, 50);
  display.print("5. Exit");
  display.display();
}

// Define similar functions for the other sub-menus, e.g., showSubGHzMenu, showIRMenu, etc.
void showSubGHzMenu() { /* ... */ }
void showIRMenu() { /* ... */ }
void showSDListMenu() { /* ... */ }
void showHIDMenu() { /* ... */ }
