#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Nokia 5110 LCD display pin setup
#define PIN_RESET 8
#define PIN_CE 15
#define PIN_DC 4
#define PIN_DIN 7
#define PIN_CLK 6

// Joystick and button pin setup
#define JOYSTICK_Y 28
#define BUTTON_PIN 16

// Menu states
enum MenuState { MAIN_MENU, NFC_MENU, SUB_GHZ_MENU, IR_MENU, SD_LIST_MENU, HID_MENU };
enum SubMenuState { NFC_READ, NFC_EMULATE, NFC_BRUTE_FORCE, NFC_SAVE, NFC_EXIT,
                    SUB_GHZ_READ, SUB_GHZ_EMULATE, SUB_GHZ_SAVE, SUB_GHZ_BRUTE_FORCE, SUB_GHZ_EXIT,
                    IR_READ, IR_EMULATE, IR_SAVE, IR_BRUTE_FORCE, IR_EXIT,
                    SD_LIST_NFC, SD_LIST_IR, SD_LIST_SUB_GHZ, SD_LIST_EXIT,
                    HID_SCRIPTS, HID_RUN, HID_EXIT };

MenuState currentMenu = MAIN_MENU;
int currentSelection = 0;

// Initialize display
Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RESET);

void setup() {
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.display();

  pinMode(JOYSTICK_Y, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

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
  int joystickY = analogRead(JOYSTICK_Y);
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  if (joystickY < 1000) {
    currentSelection = (currentSelection + 1) % 5;
    showMainMenu();
    delay(200);
  } else if (joystickY > 3000) {
    currentSelection = (currentSelection - 1 + 5) % 5;
    showMainMenu();
    delay(200);
  }

  if (buttonPressed) {
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
