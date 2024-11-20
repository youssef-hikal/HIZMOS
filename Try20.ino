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

// Manual images
const char manualImage1[] = { /* Image data here */ };
const char manualImage2[] = { /* Image data here */ };
const char manualImage3[] = { /* Image data here */ };
const char manualImage4[] = { /* Image data here */ };
const char manualImage5[] = { /* Image data here */ };
const char manualImage6[] = { /* Image data here */ };

// Automatic images
const char autoImage1[] = { /* Image data here */ };
const char autoImage2[] = { /* Image data here */ };
const char autoImage3[] = { /* Image data here */ };

// Arrays of images
const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};
const char* autoImages[] = {autoImage1, autoImage2, autoImage3};

// Image counters
int totalManualImages = sizeof(manualImages) / sizeof(manualImages[0]);
int totalAutoImages = sizeof(autoImages) / sizeof(autoImages[0]);
int currentManualImage = 0;
int currentAutoImage = 0;
int currentMenuOption = 0;

bool autoMode = true;  // Start in automatic mode
bool inSubMenu = false;  // Track if we're in a submenu
unsigned long lastButtonPressTime = 0;  // Time of the last button press
const unsigned long timeoutDuration = 120000;  // 2 minutes (in milliseconds)

// Submenu options for each manual image
const char* submenuOptions[][6] = {
  {"BLE Scan", "BLE Jammer", "BLE Spoof"},  // Image 1
  {"Run", "Stop"},                         // Image 2
  {"Read", "Emulate", "Save to SD", "Brute Force"},  // Image 3
  {"Read", "Jammer", "Emulate", "Save to SD"},       // Image 4
  {"Read", "Emulate", "Write", "Save to SD"},        // Image 5
  {"Deauth", "Scan", "Beacon"}                       // Image 6
};

int submenuCounts[] = {3, 2, 4, 4, 4, 3};  // Number of options per submenu

void setup() {
  LcdInitialise();
  LcdClear();
  
  pinMode(BUTTON_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT, INPUT_PULLDOWN);
  pinMode(BUTTON_BACK, INPUT_PULLDOWN);  // Initialize back button

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
    if (digitalRead(BUTTON_UP) == HIGH || digitalRead(BUTTON_DOWN) == HIGH || digitalRead(BUTTON_SELECT) == HIGH || digitalRead(BUTTON_BACK) == HIGH) {
      autoMode = false;  // Switch to manual mode
      lastButtonPressTime = currentTime;  // Update last button press time
      delay(200);        // Debounce delay
      displayManualImage(currentManualImage);  // Display the current manual image
    }
  } else if (!inSubMenu) {
    // Manual mode: Control with buttons to select images
    if (digitalRead(BUTTON_UP) == HIGH) {
      currentManualImage = (currentManualImage - 1 + totalManualImages) % totalManualImages;  // Previous image
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;  // Update last button press time
      delay(200);  // Debounce delay
    }
    if (digitalRead(BUTTON_DOWN) == HIGH) {
      currentManualImage = (currentManualImage + 1) % totalManualImages;  // Next image
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;  // Update last button press time
      delay(200);  // Debounce delay
    }
    if (digitalRead(BUTTON_SELECT) == HIGH) {
      inSubMenu = true;  // Enter submenu
      currentMenuOption = 0;  // Reset to first option
      displaySubMenu(currentManualImage, currentMenuOption);
      delay(200);  // Debounce delay
    }
  } else {
    // Submenu: Navigate through options
    if (digitalRead(BUTTON_UP) == HIGH) {
      currentMenuOption = (currentMenuOption - 1 + submenuCounts[currentManualImage]) % submenuCounts[currentManualImage];  // Previous option
      displaySubMenu(currentManualImage, currentMenuOption);
      delay(200);  // Debounce delay
    }
    if (digitalRead(BUTTON_DOWN) == HIGH) {
      currentMenuOption = (currentMenuOption + 1) % submenuCounts[currentManualImage];  // Next option
      displaySubMenu(currentManualImage, currentMenuOption);
      delay(200);  // Debounce delay
    }
    if (digitalRead(BUTTON_SELECT) == HIGH) {
      // Action for the selected submenu option
      displayAction(submenuOptions[currentManualImage][currentMenuOption]);
      delay(500);  // Show the action briefly
      displaySubMenu(currentManualImage, currentMenuOption);  // Return to submenu
    }
    if (digitalRead(BUTTON_BACK) == HIGH) {
      inSubMenu = false;  // Exit submenu
      displayManualImage(currentManualImage);
      delay(200);  // Debounce delay
    }
  }

  // Return to auto mode if no activity
  if (!autoMode && (currentTime - lastButtonPressTime > timeoutDuration)) {
    autoMode = true;
    displayAutoImage(currentAutoImage);
  }
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

void displayManualImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(manualImages[index]);
}

void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(autoImages[index]);
}

void displaySubMenu(int imageIndex, int optionIndex) {
  LcdClear();
  gotoXY(0, 0);
  LcdWrite(LCD_D, "Option: ");  // Title
  L
