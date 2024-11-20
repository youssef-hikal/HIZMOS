// Define variables and constants
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
#define BUTTON_BACK 19

#define LCD_C 0  // Command
#define LCD_D 1  // Data

const char manualImage1[] = { /* Image data here */ };
const char manualImage2[] = { /* Image data here */ };
const char manualImage3[] = { /* Image data here */ };
const char manualImage4[] = { /* Image data here */ };
const char manualImage5[] = { /* Image data here */ };
const char manualImage6[] = { /* Image data here */ };

const char autoImage1[] = { /* Image data here */ };
const char autoImage2[] = { /* Image data here */ };
const char autoImage3[] = { /* Image data here */ };

// Arrays of images
const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};
const char* autoImages[] = {autoImage1, autoImage2, autoImage3};
int totalManualImages = 6;
int totalAutoImages = 3;

// Image and menu tracking
int currentManualImage = 0;
int currentAutoImage = 0;
int currentSubMenuOption = 0;
bool autoMode = true;     // Start in automatic mode
bool inSubMenu = false;   // Track if in submenu

const char* subMenuOptions[] = {"Read", "Emulate", "Write", "Save"};
int totalSubMenuOptions = 4;

unsigned long lastButtonPressTime = 0;
const unsigned long timeoutDuration = 120000;  // 2 minutes (in milliseconds)

void setup() {
  LcdInitialise();
  LcdClear();
  
  pinMode(BUTTON_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT, INPUT_PULLDOWN);
  pinMode(BUTTON_BACK, INPUT_PULLDOWN);

  // Display the first automatic image
  displayAutoImage(currentAutoImage);
}

void loop() {
  unsigned long currentTime = millis();

  if (autoMode && !inSubMenu) {
    // Automatic mode: Loop through automatic images
    currentAutoImage = (currentAutoImage + 1) % totalAutoImages;  // Next image
    displayAutoImage(currentAutoImage);
    delay(1000);  // Wait 1 second

    // Exit auto mode on button press
    if (digitalRead(BUTTON_UP) == HIGH || digitalRead(BUTTON_DOWN) == HIGH || 
        digitalRead(BUTTON_SELECT) == HIGH || digitalRead(BUTTON_BACK) == HIGH) {
      autoMode = false;
      lastButtonPressTime = currentTime;
      delay(200);  // Debounce delay
      displayManualImage(currentManualImage);
    }
  } else {
    if (!inSubMenu) {
      // Manual mode: Navigate between images
      if (digitalRead(BUTTON_UP) == HIGH) {
        currentManualImage = (currentManualImage - 1 + totalManualImages) % totalManualImages;  // Previous image
        displayManualImage(currentManualImage);
        lastButtonPressTime = currentTime;
        delay(200);  // Debounce delay
      }
      if (digitalRead(BUTTON_DOWN) == HIGH) {
        currentManualImage = (currentManualImage + 1) % totalManualImages;  // Next image
        displayManualImage(currentManualImage);
        lastButtonPressTime = currentTime;
        delay(200);  // Debounce delay
      }
      if (digitalRead(BUTTON_SELECT) == HIGH) {
        inSubMenu = true;  // Enter submenu
        currentSubMenuOption = 0;
        displaySubMenu(currentSubMenuOption);
        delay(200);  // Debounce delay
      }

      // Return to auto mode after timeout
      if (currentTime - lastButtonPressTime > timeoutDuration) {
        autoMode = true;
        displayAutoImage(currentAutoImage);
      }
    } else {
      // Submenu mode: Navigate between options
      if (digitalRead(BUTTON_UP) == HIGH) {
        currentSubMenuOption = (currentSubMenuOption - 1 + totalSubMenuOptions) % totalSubMenuOptions;  // Previous option
        displaySubMenu(currentSubMenuOption);
        delay(200);  // Debounce delay
      }
      if (digitalRead(BUTTON_DOWN) == HIGH) {
        currentSubMenuOption = (currentSubMenuOption + 1) % totalSubMenuOptions;  // Next option
        displaySubMenu(currentSubMenuOption);
        delay(200);  // Debounce delay
      }
      if (digitalRead(BUTTON_SELECT) == HIGH) {
        executeSubMenuOption(currentSubMenuOption);  // Execute selected option
        delay(200);  // Debounce delay
      }
      if (digitalRead(BUTTON_BACK) == HIGH) {
        inSubMenu = false;  // Return to manual mode
        displayManualImage(currentManualImage);
        delay(200);  // Debounce delay
      }
    }
  }
}

// Display a manual image
void displayManualImage(int index) {
  LcdClear();
  gotoXY(0, 0);  // Ensure image starts from the top-left corner
  LcdBitmap(manualImages[index]);
}

// Display an automatic image
void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);  // Ensure image starts from the top-left corner
  LcdBitmap(autoImages[index]);
}

// Display the submenu
void displaySubMenu(int option) {
  LcdClear();
  gotoXY(0, 0);  // Top-left corner
  for (int i = 0; i < totalSubMenuOptions; i++) {
    if (i == option) {
      LcdWrite(LCD_D, '>');  // Highlight current option
    } else {
      LcdWrite(LCD_D, ' ');  // Space for non-highlighted options
    }
    LcdWriteString(subMenuOptions[i]);  // Display option
    gotoXY(0, (i + 1) * 8);  // Move to next line
  }
}

// Execute selected submenu option
void executeSubMenuOption(int option) {
  LcdClear();
  gotoXY(0, 0);
  LcdWriteString("Executing:");
  gotoXY(0, 8);
  LcdWriteString(subMenuOptions[option]);  // Display selected option
  delay(2000);  // Show for 2 seconds
  displaySubMenu(currentSubMenuOption);  // Return to submenu
}

// LCD Functions (Unchanged)
void LcdClear(void) { /* Existing Code */ }
void LcdInitialise(void) { /* Existing Code */ }
void gotoXY(int x, int y) { /* Existing Code */ }
void LcdWrite(byte dc, byte data) { /* Existing Code */ }
void LcdBitmap(const char my_array[]) { /* Existing Code */ }
void LcdWriteString(const char* str) {
  while (*str) {
    LcdWrite(LCD_D, *str++);
  }
}
