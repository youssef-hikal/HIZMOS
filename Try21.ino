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
#define BUTTON_BACK 19  // Back button on pin 19

#define LCD_C 0  // Command
#define LCD_D 1  // Data

// Manual images placeholders (replace with your image data)
const char manualImage1[] = { /* Image data here */ };
const char manualImage2[] = { /* Image data here */ };
const char manualImage3[] = { /* Image data here */ };
const char manualImage4[] = { /* Image data here */ };
const char manualImage5[] = { /* Image data here */ };
const char manualImage6[] = { /* Image data here */ };

// Automatic images placeholders (replace with your image data)
const char autoImage1[] = { /* Image data here */ };
const char autoImage2[] = { /* Image data here */ };
const char autoImage3[] = { /* Image data here */ };

// Image arrays
const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};
const char* autoImages[] = {autoImage1, autoImage2, autoImage3};

// Menu options
const char* submenuOptions[][6] = {
  {"BLE Scan", "BLE Jammer", "BLE Spoof"},             // Image 1
  {"Run", "Stop"},                                    // Image 2
  {"Read", "Emulate", "Save to SD", "Brute Force"},   // Image 3
  {"Read", "Jammer", "Emulate", "Save to SD"},        // Image 4
  {"Read", "Emulate", "Write", "Save to SD"},         // Image 5
  {"Deauth", "Scan", "Beacon"}                        // Image 6
};

int submenuCounts[] = {3, 2, 4, 4, 4, 3};  // Number of options per image

// Counters and states
int currentManualImage = 0;
int currentMenuOption = 0;
int currentAutoImage = 0;
bool autoMode = true;
bool inSubMenu = false;
unsigned long lastInteraction = 0;

void setup() {
  pinMode(BUTTON_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT, INPUT_PULLDOWN);
  pinMode(BUTTON_BACK, INPUT_PULLDOWN);

  LcdInitialise();
  LcdClear();

  // Start in automatic mode
  displayAutoImage(currentAutoImage);
}

void loop() {
  handleInput();

  // Auto mode image cycling
  if (autoMode && millis() - lastInteraction > 2000) {  // Switch image every 2 seconds
    currentAutoImage = (currentAutoImage + 1) % 3;  // Only 3 auto images
    displayAutoImage(currentAutoImage);
  }
}

// === INPUT HANDLING ===
void handleInput() {
  if (digitalRead(BUTTON_UP) == HIGH) {
    if (inSubMenu) {
      currentMenuOption = (currentMenuOption - 1 + submenuCounts[currentManualImage]) % submenuCounts[currentManualImage];
      displaySubMenuOption();
    } else if (!autoMode) {
      currentManualImage = (currentManualImage - 1 + 6) % 6;  // Cycle 6 manual images
      displayManualImage();
    }
    lastInteraction = millis();
    delay(200);  // Debounce
  }

  if (digitalRead(BUTTON_DOWN) == HIGH) {
    if (inSubMenu) {
      currentMenuOption = (currentMenuOption + 1) % submenuCounts[currentManualImage];
      displaySubMenuOption();
    } else if (!autoMode) {
      currentManualImage = (currentManualImage + 1) % 6;
      displayManualImage();
    }
    lastInteraction = millis();
    delay(200);  // Debounce
  }

  if (digitalRead(BUTTON_SELECT) == HIGH) {
    if (inSubMenu) {
      performAction();
    } else if (!autoMode) {
      inSubMenu = true;
      currentMenuOption = 0;
      displaySubMenuOption();
    }
    lastInteraction = millis();
    delay(200);  // Debounce
  }

  if (digitalRead(BUTTON_BACK) == HIGH) {
    if (inSubMenu) {
      inSubMenu = false;
      displayManualImage();
    } else {
      autoMode = true;
      displayAutoImage(currentAutoImage);
    }
    lastInteraction = millis();
    delay(200);  // Debounce
  }
}

// === DISPLAY FUNCTIONS ===
void displayManualImage() {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(manualImages[currentManualImage]);
}

void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(autoImages[index]);
}

void displaySubMenuOption() {
  LcdClear();
  gotoXY(0, 0);
  LcdWriteString(submenuOptions[currentManualImage][currentMenuOption]);
}

void performAction() {
  LcdClear();
  gotoXY(0, 0);
  LcdWriteString("Action: ");
  LcdWriteString(submenuOptions[currentManualImage][currentMenuOption]);
  delay(1000);  // Show action for 1 second
  displaySubMenuOption();
}

// === LCD FUNCTIONS ===
void LcdInitialise() {
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
  LcdWrite(LCD_C, 0x20);  // Standard Commands, Horizontal addressing
  LcdWrite(LCD_C, 0x0C);  // Normal Mode
}

void LcdClear() {
  for (int i = 0; i < LCD_X * LCD_Y / 8; i++) {
    LcdWrite(LCD_D, 0x00);
  }
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

void LcdBitmap(const char* image) {
  for (int i = 0; i < LCD_X * LCD_Y / 8; i++) {
    LcdWrite(LCD_D, image[i]);
  }
}

void LcdWriteString(const char* str) {
  while (*str) {
    LcdWrite(LCD_D, *str++);
  }
}
