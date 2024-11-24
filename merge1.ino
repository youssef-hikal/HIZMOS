#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Pins for the Nokia 5110 display
#define RST_PIN 8
#define CE_PIN  5
#define DC_PIN  4
#define DIN_PIN 7
#define CLK_PIN 6

// Navigation buttons
#define BUTTON_UP 16
#define BUTTON_DOWN 17
#define BUTTON_SELECT 18
#define BUTTON_BACK 19

// LED pin for Laser control
#define LED_PIN 9

// Initialize Nokia 5110 display
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);

// Menu variables
const char* mainMenuItems[] = {"1-HID", "2-NFC", "3-Ultrasonic", "4-Buzzer"};
int menuIndex = 0;
int numOfMainItems = 4;
bool buttonPressed = false;

// Laser sub-menu variables
const char* laserMenuItems[] = {"On", "Off", "Back"};
int laserMenuIndex = 0;
int numOfLaserItems = 3;
bool inLaserMenu = false;

void setup() {
  // Initialize display
  display.begin();
  display.setContrast(50);
  display.clearDisplay();

  // Setup buttons and LED
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Turn off the laser initially

  // Display initial main menu
  displayMainMenu();
}

void loop() {
  // Check button presses
  if (digitalRead(BUTTON_UP) == LOW && !buttonPressed) {
    buttonPressed = true;
    if (!inLaserMenu && menuIndex > 0) {
      menuIndex--;
      displayMainMenu();
    } else if (inLaserMenu && laserMenuIndex > 0) {
      laserMenuIndex--;
      displayLaserMenu();
    }
    delay(200);  // Debounce
  }

  if (digitalRead(BUTTON_DOWN) == LOW && !buttonPressed) {
    buttonPressed = true;
    if (!inLaserMenu && menuIndex < numOfMainItems - 1) {
      menuIndex++;
      displayMainMenu();
    } else if (inLaserMenu && laserMenuIndex < numOfLaserItems - 1) {
      laserMenuIndex++;
      displayLaserMenu();
    }
    delay(200);  // Debounce
  }

  if (digitalRead(BUTTON_SELECT) == LOW && !buttonPressed) {
    buttonPressed = true;
    if (!inLaserMenu) {
      selectMainMenuItem();
    } else {
      selectLaserMenuItem();
    }
    delay(200);  // Debounce
  }

  if (digitalRead(BUTTON_BACK) == LOW && !buttonPressed) {
    buttonPressed = true;
    if (inLaserMenu) {
      inLaserMenu = false;
      displayMainMenu();
    }
    delay(200);  // Debounce
  }

  // Reset button state when released
  if (digitalRead(BUTTON_UP) == HIGH &&
      digitalRead(BUTTON_DOWN) == HIGH &&
      digitalRead(BUTTON_SELECT) == HIGH &&
      digitalRead(BUTTON_BACK) == HIGH) {
    buttonPressed = false;
  }
}

// Function to display the main menu
void displayMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  // Print each main menu item and highlight the current one
  for (int i = 0; i < numOfMainItems; i++) {
    if (i == menuIndex) {
      display.setTextColor(WHITE, BLACK);  // Highlight selected item
    } else {
      display.setTextColor(BLACK, WHITE);  // Normal display for other items
    }
    display.setCursor(0, i * 10);  // Set position of each item
    display.println(mainMenuItems[i]);
  }
  display.display();
}

// Function to handle main menu item selection
void selectMainMenuItem() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  display.setCursor(0, 10);
  display.print("Selected: ");
  display.println(mainMenuItems[menuIndex]);

  display.display();
  delay(1000);  // Show selection for 1 second
  // If Laser is selected, go to the Laser menu
  if (menuIndex == 1) {
    inLaserMenu = true;
    displayLaserMenu();
  } else {
    displayMainMenu();  // Return to the main menu after selection
  }
}

// Function to display the Laser control menu
void displayLaserMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  // Print each Laser menu item and highlight the current one
  for (int i = 0; i < numOfLaserItems; i++) {
    if (i == laserMenuIndex) {
      display.setTextColor(WHITE, BLACK);  // Highlight selected item
    } else {
      display.setTextColor(BLACK, WHITE);  // Normal display for other items
    }
    display.setCursor(0, i * 10);  // Set position of each item
    display.println(laserMenuItems[i]);
  }
  display.display();
}

// Function to handle Laser menu item selection
void selectLaserMenuItem() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(0, 10);
  display.print("Laser: ");
  display.println(laserMenuItems[laserMenuIndex]);

  display.display();
  delay(1000);  // Show selection for 1 second

  // Control the LED based on Laser menu selection
  if (laserMenuIndex == 0) {
    digitalWrite(LED_PIN, HIGH);  // Turn on Laser (LED)
  } else if (laserMenuIndex == 1) {
    digitalWrite(LED_PIN, LOW);   // Turn off Laser (LED)
  } else if (laserMenuIndex == 2) {
    inLaserMenu = false;  // Go back to main menu
    displayMainMenu();
  }

  // Return to the Laser menu after selection unless "Back" is chosen
  if (laserMenuIndex != 2) {
    displayLaserMenu();
  }
}
