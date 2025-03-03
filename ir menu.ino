#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Display pin definitions
#define RST 14
#define CE   5
#define DC  19
#define DIN 23
#define CLK 18

// Button pin definitions
#define BTN_UP     27
#define BTN_DOWN   26
#define BTN_SELECT 12
#define BTN_BACK   25

// Initialize display
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CE, RST);

// Menu variables
const char *irmenuItems[] = {"universal IR", "learn new IR", "Saved IR"};
const int irmenuSize = sizeof(irmenuItems) / sizeof(irmenuItems[0]);
int ircurrentSelection = 0;
bool inirSubMenu = false;

// Button debounce variables
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

void setup() {
  // Initialize buttons with internal pull-up
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  // Initialize display
  display.begin();
  display.setContrast(40);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.display();
}

void loop() {
  handleInput();
  if (!inirSubMenu) {
    drawirMainMenu();
  }
}

void handleInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    ircurrentSelection = (ircurrentSelection - 1 + irmenuSize) % irmenuSize;
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    ircurrentSelection = (ircurrentSelection + 1) % irmenuSize;
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    irexecuteAction();
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW && inirSubMenu) {
    inirSubMenu = false;
    lastDebounceTime = millis();
  }
}




void drawirMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < irmenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == ircurrentSelection) {
      display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(irmenuItems[i]);
  }
  display.display();
}

void irexecuteAction() {
  inirSubMenu = true;
  display.clearDisplay();
  display.setCursor(0, 0);
  
  switch (ircurrentSelection) {
    case 0: // Read
      display.println("Reading data...");
      break;
    case 1: // Write
      display.println("Writing data...");
      break;
    case 2: // Save
      display.println("Saving...");
      break;
    case 3: // Emulate
      display.println("Emulating...");
      break;
  }
  
  display.println("\nPress BACK to return");
  display.display();
  
  // Wait for BACK button press
  while (digitalRead(BTN_BACK) != LOW) {
    delay(50);
  }
  inirSubMenu = false;
}
