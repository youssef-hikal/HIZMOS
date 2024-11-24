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
#define BUTTON_BACK  19 // Back button

#define LED_PIN 25

#define LCD_C 0  // Command
#define LCD_D 1  // Data

// Manual and Auto Image Arrays
const char manualImage1[] = { /* Image data here */ };
const char manualImage2[] = { /* Image data here */ };
const char manualImage3[] = { /* Image data here */ };
const char manualImage4[] = { /* Image data here */ };
const char manualImage5[] = { /* Image data here */ };
const char manualImage6[] = { /* Image data here */ };

const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};

// Arrays for menu options
const char* menuOptions[] = {"NFC", "Sub-GHz", "IR", "HID"};

// Mode flags
bool autoMode = true;  // Start in automatic mode
int currentManualImage = 0; // For manual image tracking

unsigned long lastButtonPressTime = 0;
const unsigned long timeoutDuration = 120000;  // 2 minutes (in milliseconds)

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);  // Initialize back button

  LcdInitialise();
  LcdClear();

  // Start in automatic mode
  displayAutoImage(0);
}

void loop() {
  unsigned long currentTime = millis();

  if (autoMode) {
    // Automatic Mode: Show images periodically
    displayAutoImage(currentManualImage); // Display current image
    currentManualImage = (currentManualImage + 1) % 6; // Loop through images

    delay(2000);  // Wait for 2 seconds before changing image

    // Check if any button is pressed to switch to manual mode
    if (digitalRead(BUTTON_UP) == LOW || digitalRead(BUTTON_DOWN) == LOW || digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_BACK) == LOW) {
      autoMode = false;
      lastButtonPressTime = currentTime;
      delay(200);  // Debounce delay
      displayManualImage(currentManualImage);  // Display the current manual image
    }

  } else {
    // Manual Mode: Navigate through menu options
    if (digitalRead(BUTTON_UP) == LOW) {
      currentManualImage = (currentManualImage - 1 + 6) % 6;  // Previous image
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;
      delay(200);
    }

    if (digitalRead(BUTTON_DOWN) == LOW) {
      currentManualImage = (currentManualImage + 1) % 6;  // Next image
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;
      delay(200);
    }

    // If "SELECT" is pressed, show menu options for the selected image
    if (digitalRead(BUTTON_SELECT) == LOW) {
      showMenuOptions();  // Show menu options
      delay(200);  // Debounce delay
    }

    // Check if "BACK" button is pressed to return to automatic mode
    if (digitalRead(BUTTON_BACK) == LOW) {
      autoMode = true;
      delay(200);  // Debounce delay
      displayAutoImage(currentManualImage);  // Go back to automatic mode
    }

    // Timeout: Return to automatic mode after inactivity
    if (currentTime - lastButtonPressTime > timeoutDuration) {
      autoMode = true;
      displayAutoImage(currentManualImage);  // Return to automatic mode
    }
  }
}

// Function to display the current manual image
void displayManualImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(manualImages[index]);
}

// Function to display the automatic image
void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(manualImages[index]);
}

// Function to show menu options
void showMenuOptions() {
  LcdClear();
  for (int i = 0; i < 4; i++) {
    gotoXY(0, i);
    LcdWriteString(menuOptions[i]);
  }
}

// Function to write a string to the LCD
void LcdWriteString(const char* str) {
  while (*str) {
    LcdWrite(LCD_D, *str);  // Send each character to the display
    str++;
  }
}

// LCD initialization and utilities
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
  LcdWrite(LCD_C, 0x80 | x);  // Set column address
  LcdWrite(LCD_C, 0x40 | y);  // Set row address
}

void LcdClear(void) {
  for (int i = 0; i < LCD_X * LCD_Y / 8; i++) {
    LcdWrite(LCD_D, 0x00);  // Clear the display
  }
}

void LcdWrite(byte dc, byte data) {
  digitalWrite(PIN_DC, dc);  // Set data/command mode
  digitalWrite(PIN_SCE, LOW);  // Enable chip
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);  // Send data byte
  digitalWrite(PIN_SCE, HIGH);  // Disable chip
}

void LcdBitmap(const char my_array[]) {
  for (int i = 0; i < (LCD_X * LCD_Y / 8); i++) {
    LcdWrite(LCD_D, my_array[i]);  // Display image byte by byte
  }
}
