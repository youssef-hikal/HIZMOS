#define LCD_X     84
#define LCD_Y     48

// Pin definitions
#define PIN_RESET PB3
#define PIN_SCE   PB4
#define PIN_DC    PB5
#define PIN_SDIN  PB6
#define PIN_SCLK  PB7

#define BUTTON_UP PB12
#define BUTTON_DOWN PB13
#define BUTTON_SELECT PB14
#define BUTTON_SELECT_MODE PB15

#define LCD_C 0  // Command
#define LCD_D 1  // Data

// Manual images (replace these with actual image data)
const char manualImage1[] = {};  // Replace with actual data
const char manualImage2[] = {};  // Replace with actual data
const char manualImage3[] = {};  // Replace with actual data
const char manualImage4[] = {};  // Replace with actual data
const char manualImage5[] = {};  // Replace with actual data
const char manualImage6[] = {};  // Replace with actual data

// Sub-images for manual mode
const char manualSubImage1[] = {};  // Replace with actual data
const char manualSubImage2[] = {};  // Replace with actual data
const char manualSubImage3[] = {};  // Replace with actual data
const char manualSubImage4[] = {};  // Replace with actual data
const char manualSubImage5[] = {};  // Replace with actual data
const char manualSubImage6[] = {};  // Replace with actual data

const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};
const char* manualSubImages[] = {manualSubImage1, manualSubImage2, manualSubImage3, manualSubImage4, manualSubImage5, manualSubImage6};

// Automatic images (replace these with actual image data)
const char autoImage1[] = {};  // Replace with actual data
const char autoImage2[] = {};  // Replace with actual data
const char autoImage3[] = {};  // Replace with actual data
const char autoImage4[] = {};  // Replace with actual data
const char autoImage5[] = {};  // Replace with actual data
const char autoImage6[] = {};  // Replace with actual data

const char* autoImages[] = {autoImage1, autoImage2, autoImage3, autoImage4, autoImage5, autoImage6};

int totalManualImages = sizeof(manualImages) / sizeof(manualImages[0]);
int totalManualSubImages = sizeof(manualSubImages) / sizeof(manualSubImages[0]);
int totalAutoImages = sizeof(autoImages) / sizeof(autoImages[0]);

int currentManualImage = 0;
int currentManualSubImage = 0;
int currentAutoImage = 0;
bool autoMode = true;  // Start in automatic mode
unsigned long lastButtonPressTime = 0;  // Time of the last button press
const unsigned long timeoutDuration = 100000;  // 1 minute (in milliseconds)

bool displaySubImage = false;  // Tracks if sub-images are being navigated

void setup() {
  LcdInitialise();
  LcdClear();

  pinMode(BUTTON_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT_MODE, INPUT_PULLDOWN);

  displayAutoImage(currentAutoImage);
}

void loop() {
  unsigned long currentTime = millis();

  if (autoMode) {
    currentAutoImage = (currentAutoImage + 1) % totalAutoImages;
    displayAutoImage(currentAutoImage);
    delay(800);

    if (digitalRead(BUTTON_UP) == HIGH || digitalRead(BUTTON_DOWN) == HIGH || digitalRead(BUTTON_SELECT) == HIGH || digitalRead(BUTTON_SELECT_MODE) == HIGH) {
      autoMode = false;
      lastButtonPressTime = currentTime;
      delay(200);
      displayManualImage(currentManualImage);
    }
  } else {
    if (displaySubImage) {
      if (digitalRead(BUTTON_UP) == HIGH) {
        currentManualSubImage = (currentManualSubImage - 1 + totalManualSubImages) % totalManualSubImages;
        displayManualSubImage(currentManualSubImage);
        lastButtonPressTime = currentTime;
        delay(200);
      }
      if (digitalRead(BUTTON_DOWN) == HIGH) {
        currentManualSubImage = (currentManualSubImage + 1) % totalManualSubImages;
        displayManualSubImage(currentManualSubImage);
        lastButtonPressTime = currentTime;
        delay(200);
      }
      if (digitalRead(BUTTON_SELECT_MODE) == HIGH) {
        displaySubImage = false;  // Exit sub-image navigation
        delay(200);
        displayManualImage(currentManualImage);
      }
    } else {
      if (digitalRead(BUTTON_UP) == HIGH) {
        currentManualImage = (currentManualImage - 1 + totalManualImages) % totalManualImages;
        displayManualImage(currentManualImage);
        lastButtonPressTime = currentTime;
        delay(200);
      }
      if (digitalRead(BUTTON_DOWN) == HIGH) {
        currentManualImage = (currentManualImage + 1) % totalManualImages;
        displayManualImage(currentManualImage);
        lastButtonPressTime = currentTime;
        delay(200);
      }
      if (digitalRead(BUTTON_SELECT) == HIGH) {
        autoMode = true;
        delay(200);
        displayAutoImage(currentAutoImage);
      }
      if (digitalRead(BUTTON_SELECT_MODE) == HIGH) {
        displaySubImage = true;  // Enter sub-image navigation
        currentManualSubImage = currentManualImage;  // Start with corresponding sub-image
        delay(200);
        displayManualSubImage(currentManualSubImage);
      }

      if (currentTime - lastButtonPressTime > timeoutDuration) {
        autoMode = true;
        displayAutoImage(currentAutoImage);
      }
    }
  }
}

void displayManualImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(manualImages[index]);
}

void displayManualSubImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(manualSubImages[index]);
}

void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);
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

  LcdWrite(LCD_C, 0x21);
  LcdWrite(LCD_C, 0xB1);
  LcdWrite(LCD_C, 0x04);
  LcdWrite(LCD_C, 0x14);
  LcdWrite(LCD_C, 0x0C);
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void gotoXY(int x, int y) {
  LcdWrite(LCD_C, 0x80 | x);
  LcdWrite(LCD_C, 0x40 | y);
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
