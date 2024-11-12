#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <IRremote.h>  // إضافة مكتبة IRremote

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

// IR Receiver pin setup
#define IR_PIN 19  // توصيل جهاز الاستقبال إلى الدبوس 19

// Menu states
enum MenuState { MAIN_MENU, NFC_MENU, SUB_GHZ_MENU, IR_MENU, SD_LIST_MENU, HID_MENU };
MenuState currentMenu = MAIN_MENU;
int currentSelection = 0;

// Initialize display
Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RESET);

// Initialize IR receiver
IRrecv irrecv(IR_PIN);
decode_results results;

void setup() {
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.display();

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  irrecv.enableIRIn();  // تشغيل جهاز الاستقبال IR

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

  // Display options with highlight
  displayOption(0, "NFC");
  displayOption(1, "Sub GHz");
  displayOption(2, "Infrared");
  displayOption(3, "SD List");
  displayOption(4, "HID");

  display.display();
}

void displayOption(int index, const char* text) {
  int yPos = 10 + (index * 10);  // Position for each option

  // Draw rectangle around selected option
  if (currentSelection == index) {
    display.drawRect(0, yPos - 2, 84, 10, BLACK); // Rectangle around the selected option
    display.setCursor(0, yPos);
    display.print("> ");
  } else {
    display.setCursor(0, yPos);
    display.print("  ");
  }
  display.print(text);
}

void handleMenuNavigation() {
  bool upPressed = digitalRead(BUTTON_UP) == LOW;
  bool downPressed = digitalRead(BUTTON_DOWN) == LOW;
  bool selectPressed = digitalRead(BUTTON_SELECT) == LOW;

  if (upPressed) {
    currentSelection = (currentSelection - 1 + 5) % 5; // Wrap around menu items
    showCurrentMenu();
    delay(200);
  }
  if (downPressed) {
    currentSelection = (currentSelection + 1) % 5; // Wrap around menu items
    showCurrentMenu();
    delay(200);
  }
  if (selectPressed) {
    selectMenuOption();
    delay(200);
  }
}

void selectMenuOption() {
  switch (currentMenu) {
    case MAIN_MENU:
      switch (currentSelection) {
        case 0: currentMenu = NFC_MENU; showNFCMenu(); break;
        case 1: currentMenu = SUB_GHZ_MENU; showSubGHzMenu(); break;
        case 2: currentMenu = IR_MENU; showIRMenu(); break;
        case 3: currentMenu = SD_LIST_MENU; showSDListMenu(); break;
        case 4: currentMenu = HID_MENU; showHIDMenu(); break;
      }
      break;
    case IR_MENU:
      if (currentSelection == 0) {  // "Read" option in IR menu
        readIRSignal();
      } else if (currentSelection == 4) {  // Exit option
        currentMenu = MAIN_MENU;
        showMainMenu();
      }
      break;
    // Add other cases for additional menus
  }
}

void showCurrentMenu() {
  switch (currentMenu) {
    case MAIN_MENU: showMainMenu(); break;
    case IR_MENU: showIRMenu(); break;
    // Add cases for other menus
  }
}

void showIRMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("IR Menu:");

  // Display options with highlight
  displayOption(0, "Read");
  displayOption(1, "Emulate");
  displayOption(2, "Save to SD");
  displayOption(3, "Brute Force");
  displayOption(4, "Exit");

  display.display();
}

void readIRSignal() {
  if (irrecv.decode(&results)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("IR Signal:");

    // عرض القيمة المستلمة بتنسيق hexadecimal
    display.setCursor(0, 10);
    display.print("Hex: ");
    display.print(results.value, HEX);

    display.display();
    irrecv.resume();  // Ready to receive the next signal
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("No signal");
    display.display();
  }
}
