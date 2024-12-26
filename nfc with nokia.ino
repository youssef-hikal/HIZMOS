// Pin definitions for Nokia 5110 LCD
#define RST_PIN 8
#define CE_PIN 5
#define DC_PIN 4
#define DIN_PIN 7
#define CLK_PIN 6

// Pins for PN532 (NFC reader)
#define SDA_PIN 2
#define SCL_PIN 3

// RGB LED pins
#define RED_LED 10
#define GREEN_LED 16
#define BLUE_LED 14

// Joystick and button pins
#define JOYSTICK_PIN 36
#define BUTTON_PIN 15

// Menu structure and state variables
#define MAIN_MENU_ITEMS 4
String mainMenu[MAIN_MENU_ITEMS] = {"Read Tag", "Save Tag", "Emulate Tag", "Saved Tags"};
int mainMenuIndex = 0;
bool inSubMenu = false;
String savedTags[10];  // Array to store saved UIDs
int savedTagCount = 0;
int savedMenuIndex = 0;

// Initialize LCD and PN532
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup() {
  Serial.begin(115200);
  
  // Initialize Nokia LCD
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  
  // Initialize PN532
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN53x board");
    while (1);
  }
  nfc.SAMConfig();
  
  // Joystick and button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // RGB LED setup
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  randomSeed(analogRead(0));
  
  // Initial menu display
  showMainMenu();
}

void loop() {
  int joystickVal = analogRead(JOYSTICK_PIN);
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  // Navigate main or sub-menu
  if (joystickVal < 300) {  // Joystick up
    if (inSubMenu) {
      savedMenuIndex = (savedMenuIndex > 0) ? savedMenuIndex - 1 : savedTagCount - 1;
    } else {
      mainMenuIndex = (mainMenuIndex > 0) ? mainMenuIndex - 1 : MAIN_MENU_ITEMS - 1;
    }
    showMenu();
    delay(200);  // Debounce delay
  } else if (joystickVal > 700) {  // Joystick down
    if (inSubMenu) {
      savedMenuIndex = (savedMenuIndex < savedTagCount - 1) ? savedMenuIndex + 1 : 0;
    } else {
      mainMenuIndex = (mainMenuIndex < MAIN_MENU_ITEMS - 1) ? mainMenuIndex + 1 : 0;
    }
    showMenu();
    delay(200);  // Debounce delay
  }

  // Execute selected action
  if (buttonPressed) {
    if (inSubMenu) {
      // Exit sub-menu
      inSubMenu = false;
      showMainMenu();
    } else {
      executeMainMenuAction(mainMenuIndex);
    }
    delay(200);  // Debounce delay
  }
}

// Show main menu
void showMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  for (int i = 0; i < MAIN_MENU_ITEMS; i++) {
    if (i == mainMenuIndex) {
      display.fillRect(0, i * 10, 84, 10, BLACK);  // Highlight selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(0, i * 10);
    display.print(mainMenu[i]);
  }
  display.display();
}

// Show current menu (main or sub-menu)
void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);

  if (inSubMenu) {  // Show saved tags sub-menu
    display.setCursor(0, 0);
    display.print("Saved Tags:");
    for (int i = 0; i < savedTagCount; i++) {
      display.setCursor(0, (i + 1) * 10);
      display.print(savedTags[i]);
    }
  } else {  // Show main menu
    showMainMenu();
  }
  display.display();
}

// Execute selected action from main menu
void executeMainMenuAction(int action) {
  switch (action) {
    case 0:  // "Read Tag"
      digitalWrite(RED_LED, HIGH);
      readTag();
      digitalWrite(RED_LED, LOW);
      break;
    case 1:  // "Save Tag"
      digitalWrite(GREEN_LED, HIGH);
      saveTag();
      digitalWrite(GREEN_LED, LOW);
      break;
    case 2:  // "Emulate Tag"
      digitalWrite(BLUE_LED, HIGH);
      emulateTag();
      digitalWrite(BLUE_LED, LOW);
      break;
    case 3:  // "Saved Tags" submenu
      inSubMenu = true;
      showMenu();
      break;
  }
}

// Read and display tag UID
void readTag() {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Scanning...");
  display.display();

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      display.print(uid[i], HEX);
      display.print(" ");
    }
    display.display();
    delay(2000);
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("No tag found");
    display.display();
    delay(2000);
  }
  showMainMenu();
}

// Save tag UID to memory
void saveTag() {
  uint8_t uid[7];
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    String tagUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      tagUID += String(uid[i], HEX);
    }
    if (savedTagCount < 10) {
      savedTags[savedTagCount++] = tagUID;
    } else {
      Serial.println("Max saved tags reached");
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Tag Saved!");
    display.display();
    delay(1000);
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("No tag found");
    display.display();
    delay(2000);
  }
  showMainMenu();
}

// Emulate the selected tag UID
void emulateTag() {
  if (savedTagCount == 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("No tags saved");
    display.display();
    delay(2000);
    return;
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Emulating UID:");
  display.setCursor(0, 10);
  display.print(savedTags[savedMenuIndex]);
  display.display();
  delay(2000);  // Emulate delay
  showMainMenu();
}
