#include <PCD8544.h>
#include <SPI.h>

// Pin definitions for Nokia 5110 LCD
#define PIN_RESET 8
#define PIN_SCE   5
#define PIN_DC    4
#define PIN_SDIN  7
#define PIN_SCLK  6

// Button pin definitions
#define BUTTON_UP 16
#define BUTTON_DOWN 17
#define BUTTON_SELECT 18
#define BUTTON_BACK 19  // Back button on pin 19

PCD8544 lcd(PIN_SCE, PIN_RESET, PIN_DC, PIN_SDIN, PIN_SCLK); // Initialize the LCD object

// Define options for the menu
#define OPTION_ORANGE 1
#define OPTION_GREEN 2
#define OPTION_BLUE 3
#define OPTION_YELLOW 4

int currentOption = OPTION_ORANGE;  // Start with the first option
unsigned long lastButtonPressTime = 0;  // Time of the last button press
const unsigned long timeoutDuration = 120000;  // 2 minutes (in milliseconds)

void setup() {
  lcd.begin();
  lcd.setContrast(50);  // Set contrast level
  lcd.clear();
  
  pinMode(BUTTON_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT, INPUT_PULLDOWN);
  pinMode(BUTTON_BACK, INPUT_PULLDOWN);  // Initialize back button

  // Display the first menu option
  displayMenu(currentOption);
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check for button presses
  if (digitalRead(BUTTON_UP) == HIGH) {
    currentOption = (currentOption - 1) < OPTION_ORANGE ? OPTION_YELLOW : currentOption - 1;
    displayMenu(currentOption);
    delay(200);  // Debounce delay
    lastButtonPressTime = currentTime;
  }
  
  if (digitalRead(BUTTON_DOWN) == HIGH) {
    currentOption = (currentOption + 1) > OPTION_YELLOW ? OPTION_ORANGE : currentOption + 1;
    displayMenu(currentOption);
    delay(200);  // Debounce delay
    lastButtonPressTime = currentTime;
  }

  if (digitalRead(BUTTON_SELECT) == HIGH) {
    selectOption(currentOption);
    delay(200);  // Debounce delay
    lastButtonPressTime = currentTime;
  }

  if (digitalRead(BUTTON_BACK) == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Back to auto mode");
    delay(1000);  // Show message for 1 second
    displayMenu(currentOption);  // Go back to menu after back
    delay(200);  // Debounce delay
  }

  // Auto mode timeout
  if (currentTime - lastButtonPressTime > timeoutDuration) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Auto mode timeout");
    delay(2000);  // Show timeout message for 2 seconds
    displayMenu(currentOption);  // Go back to menu after timeout
  }
}

void displayMenu(int option) {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (option) {
    case OPTION_ORANGE:
      lcd.print("1. Orange");
      break;
    case OPTION_GREEN:
      lcd.print("2. Green");
      break;
    case OPTION_BLUE:
      lcd.print("3. Blue");
      break;
    case OPTION_YELLOW:
      lcd.print("4. Yellow");
      break;
  }
}

void selectOption(int option) {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (option) {
    case OPTION_ORANGE:
      lcd.print("Orange Selected");
      break;
    case OPTION_GREEN:
      lcd.print("Green Selected");
      break;
    case OPTION_BLUE:
      lcd.print("Blue Selected");
      break;
    case OPTION_YELLOW:
      lcd.print("Yellow Selected");
      break;
  }
  delay(1000);  // Show selection for 1 second
  displayMenu(currentOption);  // Go back to the menu
}
