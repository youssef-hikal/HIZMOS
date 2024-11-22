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

// Manual images (replace these with actual image data)
const char manualImage1[] = { /* Image data here */ };
const char manualImage2[] = { /* Image data here */ };
const char manualImage3[] = { /* Image data here */ };
const char manualImage4[] = { /* Image data here */ };
const char manualImage5[] = { /* Image data here */ };
const char manualImage6[] = { /* Image data here */ };

// Automatic images (replace these with actual image data)
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

bool autoMode = true;  // Start in automatic mode
unsigned long lastButtonPressTime = 0;  // Time of the last button press
const unsigned long timeoutDuration = 120000;  // 2 minutes (in milliseconds)

// Color options
#define OPTION_ORANGE 0
#define OPTION_GREEN  1
#define OPTION_BLUE   2
#define OPTION_YELLOW 3

int selectedColorOption = -1;  // No option selected initially

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
  } else {
    // Manual mode: Control with buttons
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
      autoMode = true;  // Return to automatic mode
      delay(200);       // Debounce delay
      displayAutoImage(currentAutoImage);  // Display the current automatic image
    }
    if (digitalRead(BUTTON_BACK) == HIGH) {
      autoMode = true;  // Back button pressed, return to automatic mode
      delay(200);       // Debounce delay
      displayAutoImage(currentAutoImage);  // Display the current automatic image
    }

    // Check if no button has been pressed for the timeout duration
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

  if (index == 0) {  // If it's the first image
    selectedColorOption = -1;  // Reset selected option
    displayColorOptions();  // Display the color options
    while (selectedColorOption == -1) {
      handleColorSelection();  // Handle color selection
    }
  }
}

// Display an automatic image
void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);  // Ensure image starts from the top-left corner
  LcdBitmap(autoImages[index]);
}

// Display color options
void displayColorOptions() {
  LcdClear();
  gotoXY(0, 0);
  LcdWrite(LCD_D, '1');
  LcdWrite(LCD_D, '.');
  LcdWrite(LCD_D, ' ');
  LcdWrite(LCD_D, 'O');
  LcdWrite(LCD_D, 'r');
  LcdWrite(LCD_D, 'a');
  LcdWrite(LCD_D, 'n');
  LcdWrite(LCD_D, 'g');
  LcdWrite(LCD_D, 'e');
  
  gotoXY(0, 1);
  LcdWrite(LCD_D, '2');
  LcdWrite(LCD_D, '.');
  LcdWrite(LCD_D, ' ');
  LcdWrite(LCD_D, 'G');
  LcdWrite(LCD_D, 'r');
  LcdWrite(LCD_D, 'e');
  LcdWrite(LCD_D, 'e');
  LcdWrite(LCD_D, 'n');
  
  gotoXY(0, 2);
  LcdWrite(LCD_D, '3');
  LcdWrite(LCD_D, '.');
  LcdWrite(LCD_D, ' ');
  LcdWrite(LCD_D, 'B');
  LcdWrite(LCD_D, 'l');
  LcdWrite(LCD_D, 'u');
  LcdWrite(LCD_D, 'e');
  
  gotoXY(0, 3);
  LcdWrite(LCD_D, '4');
  LcdWrite(LCD_D, '.');
  LcdWrite(LCD_D, ' ');
  LcdWrite(LCD_D, 'Y');
  LcdWrite(LCD_D, 'e');
  LcdWrite(LCD_D, 'l');
  LcdWrite(LCD_D, 'l');
  LcdWrite(LCD_D, 'o');
  LcdWrite(LCD_D, 'w');
}

// Handle color selection
void handleColorSelection() {
  if (digitalRead(BUTTON_UP) == HIGH) {
    selectedColorOption = (selectedColorOption - 1 + 4) % 4;  // Previous color
    delay(200);  // Debounce
  }
  if (digitalRead(BUTTON_DOWN) == HIGH) {
    selectedColorOption = (selectedColorOption + 1) % 4;  // Next color
    delay(200);  // Debounce
  }
  if (digitalRead(BUTTON_SELECT) == HIGH && selectedColorOption != -1) {
    switch (selectedColorOption) {
      case OPTION_ORANGE:
        LcdWrite(LCD_D, 'O');
        LcdWrite(LCD_D, 'r');
        LcdWrite(LCD_D, 'a');
        LcdWrite(LCD_D, 'n');
        LcdWrite(LCD_D, 'g');
        LcdWrite(LCD_D, 'e');
        break;
      case OPTION_GREEN:
        LcdWrite(LCD_D, 'G');
        LcdWrite(LCD_D, 'r');
        LcdWrite(LCD_D, 'e');
        LcdWrite(LCD_D, 'e');
        LcdWrite(LCD_D, 'n');
        break;
      case OPTION_BLUE:
        LcdWrite(LCD_D, 'B');
        LcdWrite(LCD_D, 'l');
        LcdWrite(LCD_D, 'u');
        LcdWrite(LCD_D, 'e');
        break;
      case OPTION_YELLOW:
        LcdWrite(LCD_D, 'Y');
        LcdWrite(LCD_D, 'e');
        LcdWrite(LCD_D, 'l');
        LcdWrite(LCD_D, 'l');
        LcdWrite(LCD_D, 'o');
        LcdWrite(LCD_D, 'w');
        break;
    }
    delay(200);  // Debounce after selection
  }
}

// Initialize the LCD
void LcdInitialise() {
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  LcdClear();
  // Initialization commands for LCD
  digitalWrite(PIN_SCE, HIGH);
  digitalWrite(PIN_RESET, LOW);
  delay(10);
  digitalWrite(PIN_RESET, HIGH);
  delay(10);
  LcdWrite(LCD_C, 0x21);  // Extended command set
  LcdWrite(LCD_C, 0xB8);  // Set contrast
  LcdWrite(LCD_C, 0x04);  // Temperature coefficient
  LcdWrite(LCD_C, 0x14);  // Bias system
  LcdWrite(LCD_C, 0x20);  // Basic command set
  LcdWrite(LCD_C, 0x0C);  // Normal display mode
}

// Clear the LCD screen
void LcdClear() {
  for (int i = 0; i < (LCD_X * LCD_Y / 8); i++) {
    LcdWrite(LCD_D, 0x00);  // Fill screen with zeros (clear)
  }
}

// Write data or command to LCD
void LcdWrite(byte dc, byte value) {
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, value);
  digitalWrite(PIN_SCE, HIGH);
}

// Set cursor position on LCD
void gotoXY(byte x, byte y) {
  LcdWrite(LCD_C, 0x80 | x);  // Set column
  LcdWrite(LCD_C, 0x40 | y);  // Set row
}

// Display a bitmap (image) on LCD
void LcdBitmap(const char* bitmap) {
  for (int i = 0; i < (LCD_X * LCD_Y / 8); i++) {
    LcdWrite(LCD_D, bitmap[i]);
  }
}
