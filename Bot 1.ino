#define LCD_X     84
#define LCD_Y     48

// Pin definitions
#define PIN_RESET 8
#define PIN_SCE   5
#define PIN_DC    4
#define PIN_SDIN  7
#define PIN_SCLK  6

#define BUTTON_UP 16
#define BUTTON_DOWN 17
#define BUTTON_SELECT 18
#define BUTTON_BACK  19 // Back button on pin 19

#define LED_PIN 25

#define LCD_C 0  // Command
#define LCD_D 1  // Data

// Arrays of images
const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};
const char* autoImages[] = {autoImage1, autoImage2, autoImage3};

// Image counters
int totalManualImages = sizeof(manualImages) / sizeof(manualImages[0]);
int totalAutoImages = sizeof(autoImages) / sizeof(autoImages[0]);

int currentManualImage = 0;
int currentAutoImage = 0;

bool autoMode = true;  // Start in automatic mode
unsigned long lastButtonPressTime = 0;  // Time of the last button press
const unsigned long timeoutDuration = 120000;  // 2 minutes (in milliseconds)

void setup() {
  LcdInitialise();
  LcdClear();
  pinMode(LED_PIN ,OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);  // Initialize back button

  // Display the first automatic image
  displayAutoImage(currentAutoImage);
}

void loop() {
  unsigned long currentTime = millis();

  if (autoMode) {
    // Automatic mode: Loop through automatic images
    currentAutoImage = (currentAutoImage + 1) % totalAutoImages;  // Next image
    displayAutoImage(currentAutoImage);
    delay(1000);  // Wait 1 second

    // Check if any button is pressed to exit auto mode
    if (digitalRead(BUTTON_UP) == LOW || digitalRead(BUTTON_DOWN) == LOW || digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_BACK) == LOW) {
      autoMode = false;  // Switch to manual mode
      lastButtonPressTime = currentTime;  // Update last button press time
      delay(200);        // Debounce delay
      displayManualImage(currentManualImage);  // Display the current manual image
    }
  } else {
    // Manual mode: Control with buttons
    if (digitalRead(BUTTON_UP) == LOW) {
      currentManualImage = (currentManualImage - 1 + totalManualImages) % totalManualImages;  // Previous image
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;  // Update last button press time
      delay(200);  // Debounce delay
    }
    if (digitalRead(BUTTON_DOWN) == LOW) {
      currentManualImage = (currentManualImage + 1) % totalManualImages;  // Next image
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;  // Update last button press time
      delay(200);  // Debounce delay
    }

    // If an image is selected, show the additional menu
    if (digitalRead(BUTTON_SELECT) == LOW) {
      displayManualMenu(currentManualImage);
      delay(200);  // Debounce delay
    }

    // Back button to switch to automatic mode
    if (digitalRead(BUTTON_BACK) == LOW) {
      autoMode = true;  // Back button pressed, return to automatic mode
      delay(200);       // Debounce delay
      displayAutoImage(currentAutoImage);  // Display the current automatic image
    }

    // Timeout: if no button pressed for timeout duration, go back to auto mode
    if (currentTime - lastButtonPressTime > timeoutDuration) {
      autoMode = true;  // Return to automatic mode
      displayAutoImage(currentAutoImage);  // Display the current automatic image
    }
  }
}
// Display a manual image
void displayManualImage(int index) {
  LcdClear();
  gotoXY(0, 0);  // Ensure image starts from the top-left corner
  LcdBitmap(manualImages[index]);
}

// Display the manual menu after an image is selected
void displayManualMenu(int index) {
  LcdClear();
  gotoXY(0, 0);
  
  // Display options for each image (NFC, HID, etc.)
  switch (index) {
    case 0:
      displayMenuOptions("NFC", "HID", "Sub-GHz", "Infrared");
      break;
    case 1:
      displayMenuOptions("NFC", "Wi-Fi", "Bluetooth", "SD Card");
      break;
    case 2:
      displayMenuOptions("HID", "Sub-GHz", "Infrared", "BLE");
      break;
    case 3:
      displayMenuOptions("Wi-Fi", "NFC", "HID", "Sub-GHz");
      break;
    case 4:
      displayMenuOptions("Bluetooth", "Sub-GHz", "NFC", "Infrared");
      break;
    case 5:
      displayMenuOptions("Infrared", "Wi-Fi", "BLE", "Sub-GHz");
      break;
  }
}

// Display a list of options
void displayMenuOptions(const char* option1, const char* option2, const char* option3, const char* option4) {
  LcdClear();
  gotoXY(0, 0);
  LcdWrite(LCD_D, option1);
  gotoXY(0, 1);
  LcdWrite(LCD_D, option2);
  gotoXY(0, 2);
  LcdWrite(LCD_D, option3);
  gotoXY(0, 3);
  LcdWrite(LCD_D, option4);
}

// Display an automatic image
void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);  // Ensure image starts from the top-left corner
  LcdBitmap(autoImages[index]);
}

// LCD Procedures  
void LcdClear(void) {
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++) {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void) {
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite(LCD_C, 0x21);  // LCD Extended Commands
  LcdWrite(LCD_C, 0xB1);  // Set LCD Vop (Contrast)
  LcdWrite(LCD_C, 0x04);  // Set Temp coefficient
  LcdWrite(LCD_C, 0x14);  // LCD bias mode 1:48
  LcdWrite(LCD_C, 0x0C);  // LCD in normal mode
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void gotoXY(int x, int y) {
  LcdWrite(LCD_C, 0x80 | x);  // Column
  LcdWrite(LCD_C, 0x40 | y);  // Row
}

void LcdWrite(byte dc, byte data) {
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

void LcdBitmap(const char my_array[]) {
  for (int index = 0; index < (LCD_X * LCD_Y / 8); index++) {
    LcdWrite(LCD_D, my_array[index]);
  }
}
