#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DHT.h>

// Pins for Nokia 5110 display
#define RST_PIN 3
#define CE_PIN  4
#define DC_PIN  5
#define DIN_PIN 6
#define CLK_PIN 7

// Joystick and button pins
#define JOYSTICK_Y A0
#define BUTTON_PIN 8

// DHT11 sensor pins
#define DHTPIN A1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Laser (LED) pin
#define LED_PIN 9

// Ultrasonic sensor pins
#define TRIG_PIN 14
#define ECHO_PIN 16

// Buzzer pin
#define BUZZER_PIN 10

// Initialize Nokia 5110 display
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);

// Main Menu variables
const char* mainMenuItems[] = {"1-DHT11", "2-Laser", "3-Ultrasonic", "4-Buzzer","5-IR-attack","6-wifi-attack","7-ble-attack"};
int menuIndex = 0;
const int numOfMainItems = 7;
bool buttonPressed = false;

// Laser Sub-Menu variables
const char* laserMenuItems[] = {"On", "Off", "Back"};
int laserMenuIndex = 0;
const int numOfLaserItems = 3;
bool inLaserMenu = false;

// DHT11 Sub-Menu variables
bool inDHTMenu = false;

// Function Prototypes
void displayMainMenu();
void selectMainMenuItem();
void displayLaserMenu();
void selectLaserMenuItem();
void displayDHTMenu();
void displayDHTData();
void displayUltrasonicData();
void controlBuzzer(bool turnOn);
long readUltrasonicDistance();
void displayBuzzerStatus(const char* status);

void setup() {
  // Initialize display
  display.begin();
  display.setContrast(50);
  display.clearDisplay();

  // Initialize DHT11 sensor
  dht.begin();
  
  // Setup joystick, button, LED, ultrasonic, and buzzer pins
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);    // Laser off initially
  digitalWrite(BUZZER_PIN, LOW); // Buzzer off initially

  // Display initial main menu
  displayMainMenu();
}

void loop() {
  int joystickY = analogRead(JOYSTICK_Y);

  if (!inLaserMenu && !inDHTMenu) {
    // Main menu navigation
    if (joystickY < 300 && menuIndex > 0) {
      menuIndex--;
      displayMainMenu();
      delay(200);
    } else if (joystickY > 700 && menuIndex < numOfMainItems - 1) {
      menuIndex++;
      displayMainMenu();
      delay(200);
    }

    // Main menu selection with button press
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
      buttonPressed = true;
      selectMainMenuItem();
      delay(200);
    }
  } else if (inLaserMenu) {
    // Laser menu navigation
    if (joystickY < 300 && laserMenuIndex > 0) {
      laserMenuIndex--;
      displayLaserMenu();
      delay(200);
    } else if (joystickY > 700 && laserMenuIndex < numOfLaserItems - 1) {
      laserMenuIndex++;
      displayLaserMenu();
      delay(200);
    }

    // Laser menu selection with button press
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
      buttonPressed = true;
      selectLaserMenuItem();
      delay(200);
    }
  }

  // Reset button state
  if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = false;
  }
}

// Function to display the main menu
void displayMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  for (int i = 0; i < numOfMainItems; i++) {
    if (i == menuIndex) {
      display.setTextColor(WHITE, BLACK);  // Highlight selected item
    } else {
      display.setTextColor(BLACK, WHITE);  // Normal display for others
    }
    display.setCursor(0, i * 10);
    display.println(mainMenuItems[i]);
  }
  display.display();
}

// Handle main menu selection
void selectMainMenuItem() {
  switch(menuIndex) {
    case 0:  // DHT11
      inDHTMenu = true;
      displayDHTMenu();
      break;
    case 1:  // Laser
      inLaserMenu = true;
      displayLaserMenu();
      break;
    case 2:  // Ultrasonic
      displayUltrasonicData();
      break;
    case 3:  // Buzzer
      controlBuzzer(true);
      delay(1000);
      controlBuzzer(false);
      break;
    default:
      displayMainMenu();
      break;
  }
}

// Laser Sub-Menu
void displayLaserMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  for (int i = 0; i < numOfLaserItems; i++) {
    if (i == laserMenuIndex) {
      display.setTextColor(WHITE, BLACK);  // Highlight selected item
    } else {
      display.setTextColor(BLACK, WHITE);
    }
    display.setCursor(0, i * 10);
    display.println(laserMenuItems[i]);
  }
  display.display();
}

void selectLaserMenuItem() {
  switch(laserMenuIndex) {
    case 0:  // Turn on Laser
      digitalWrite(LED_PIN, HIGH);
      displayLaserMenu();
      break;
    case 1:  // Turn off Laser
      digitalWrite(LED_PIN, LOW);
      displayLaserMenu();
      break;
    case 2:  // Back
      inLaserMenu = false;
      displayMainMenu();
      break;
  }
}

// Display DHT11 Data
void displayDHTMenu() {
  displayDHTData();
  inDHTMenu = false;
  displayMainMenu();
}

void displayDHTData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  if (isnan(humidity) || isnan(temperature)) {
    display.println("Error reading DHT");
  } else {
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temperature);
    display.println(" C");

    display.setCursor(0, 10);
    display.print("Humidity: ");
    display.print(humidity);
    display.println(" %");
  }
  display.display();
  delay(2000);
}

// Ultrasonic Sensor Functions
long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = (duration / 2) * 0.0343;
  return distance;
}

void displayUltrasonicData() {
  long distance = readUltrasonicDistance();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(1, 10);
  display.print("DISTANCE: ");
  display.print(distance);
  display.println("cm");

  display.display();
  delay(2000);
  displayMainMenu();
}

// Buzzer Control Functions
void controlBuzzer(bool turnOn) {
  if (turnOn) {
    digitalWrite(BUZZER_PIN, HIGH);
    displayBuzzerStatus("Buzzer ON");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    displayBuzzerStatus("Buzzer OFF");
  }
}

void displayBuzzerStatus(const char* status) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  display.setCursor(0, 10);
  display.print(status);

  display.display();
  delay(1000);
}
