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
#define BTN_DOWN   16
#define BTN_SELECT 17
#define BTN_BACK   25

// Initialize display
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CE, RST);

// Menu variables
const char *menuItems[] = {"Read", "Write", "Save", "Emulate"};
const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
int currentSelection = 0;
bool inSubMenu = false;
int subMenuSelection = 0;

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
  display.setContrast(60);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.display();
}

void loop() {
  handleInput();
  if (!inSubMenu) {
    drawMainMenu();
  }
}

void handleInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (inSubMenu) {
      subMenuSelection = (subMenuSelection - 1 + 2) % 2;
    } else {
      currentSelection = (currentSelection - 1 + menuSize) % menuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (inSubMenu) {
      subMenuSelection = (subMenuSelection + 1) % 2;
    } else {
      currentSelection = (currentSelection + 1) % menuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (inSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executeAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (inSubMenu) {
      inSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < menuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == currentSelection) {
      display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(menuItems[i]);
  }
  display.display();
}

void executeAction() {
  if (currentSelection == 0) { // If "Read" is selected
    submenu();
  } else {
    inSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (currentSelection) {
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
    inSubMenu = false;
  }
}

void submenu() {
  inSubMenu = true;
  const char *subMenuItems[] = {"ON", "OFF"};
  
  while (inSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == subMenuSelection) {
        display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(subMenuItems[i]);
    }
    display.display();
    
    handleInput();
  }
}
