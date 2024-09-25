#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DHT.h>

// Pins for the Nokia 5110 display
#define RST_PIN 3
#define CE_PIN  4
#define DC_PIN  5
#define DIN_PIN 6
#define CLK_PIN 7

// Joystick and button pins
#define JOYSTICK_Y A0
#define BUTTON_PIN 8

// DHT11 sensor
#define DHTPIN A1  // DHT11 connected to A1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LED pin for Laser control
#define LED_PIN 9

// Initialize Nokia 5110 display
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);

// Menu variables
const char* mainMenuItems[] = {"1-DHT11", "2-Laser", "3-Ultrasonic", "4-Buzzer"};
int menuIndex = 0;
int numOfMainItems = 4;
bool buttonPressed = false;

// Laser sub-menu variables
const char* laserMenuItems[] = {"On", "Off", "Back"};
int laserMenuIndex = 0;
int numOfLaserItems = 3;
bool inLaserMenu = false;

// Flags for different menus
bool inDHT11Menu = false;

void setup() {
  // Initialize display
  display.begin();
  display.setContrast(50);
  display.clearDisplay();

  // Initialize DHT11 sensor
  dht.begin();
  
  // Setup joystick, button, and LED
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Turn off the laser initially

  // Display initial main menu
  displayMainMenu();
}

void loop() {
  int joystickY = analogRead(JOYSTICK_Y);

  if (!inLaserMenu && !inDHT11Menu) {
    // Main menu navigation
    if (joystickY < 300) {
      if (menuIndex > 0) {
        menuIndex--;
        displayMainMenu();
        delay(200);  // Debounce
      }
    } else if (joystickY > 700) {
      if (menuIndex < numOfMainItems - 1) {
        menuIndex++;
        displayMainMenu();
        delay(200);  // Debounce
      }
    }

    // Check button press for main menu
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
      buttonPressed = true;
      selectMainMenuItem();
      delay(200);  // Debounce
    }
  } else if (inLaserMenu) {
    // Laser menu navigation
    if (joystickY < 300) {
      if (laserMenuIndex > 0) {
        laserMenuIndex--;
        displayLaserMenu();
        delay(200);  // Debounce
      }
    } else if (joystickY > 700) {
      if (laserMenuIndex < numOfLaserItems - 1) {
        laserMenuIndex++;
        displayLaserMenu();
        delay(200);  // Debounce
      }
    }

    // Check button press for Laser menu
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
      buttonPressed = true;
      selectLaserMenuItem();
      delay(200);  // Debounce
    }
  } else if (inDHT11Menu) {
    // Check button press for DHT11 menu to go back
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
      buttonPressed = true;
      inDHT11Menu = false;  // Go back to the main menu
      displayMainMenu();
      delay(200);  // Debounce
    }

    // Display temperature and humidity continuously
    displayTemperatureHumidity();
  }

  // Reset button state when released
  if (digitalRead(BUTTON_PIN) == HIGH) {
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
  }
  // If DHT11 is selected, display temperature and humidity
  else if (menuIndex == 0) {
    inDHT11Menu = true;
    displayTemperatureHumidity();
  }
  // Handle other menu selections here
  else {
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

// Function to display temperature and humidity from DHT11
void displayTemperatureHumidity() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 10);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 30);
  display.println("back");
  display.println("to menu");

  display.display();
}
