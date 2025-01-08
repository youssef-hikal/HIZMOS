// تعريف أبعاد الشاشة
#define LCD_X     84
#define LCD_Y     48

// تعريف Pins
#define PIN_RESET PB3
#define PIN_SCE   PB4
#define PIN_DC    PB5
#define PIN_SDIN  PB6
#define PIN_SCLK  PB7

// تعريف أزرار التحكم
#define BUTTON_UP PB12
#define BUTTON_DOWN PB13
#define BUTTON_SELECT PB14
#define BUTTON_SELECT_MODE PB15

// أوضاع LCD
#define LCD_C 0  // Command
#define LCD_D 1  // Data

// تعريف الصور
const char manualImage1[] = {};  // بيانات الصورة (يتم استبدالها)
const char manualImage2[] = {};
const char manualImage3[] = {};
const char manualImage4[] = {};
const char manualImage5[] = {};
const char manualImage6[] = {};

const char autoImage1[] = {};
const char autoImage2[] = {};
const char autoImage3[] = {};
const char autoImage4[] = {};
const char autoImage5[] = {};
const char autoImage6[] = {};

const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};
const char* autoImages[] = {autoImage1, autoImage2, autoImage3, autoImage4, autoImage5, autoImage6};

// المتغيرات الرئيسية
int currentManualImage = 0;
int currentAutoImage = 0;
bool autoMode = true;
unsigned long lastButtonPressTime = 0;
const unsigned long timeoutDuration = 60000; // دقيقة واحدة (بالمللي ثانية)

// إعداد الشاشة
void setupLCD() {
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);

  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite(LCD_C, 0x21); // إعداد الشاشة
  LcdWrite(LCD_C, 0xB1);
  LcdWrite(LCD_C, 0x04);
  LcdWrite(LCD_C, 0x14);
  LcdWrite(LCD_C, 0x0C);
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);

  LcdClear();
}

// تنظيف الشاشة
void LcdClear() {
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++) {
    LcdWrite(LCD_D, 0x00);
  }
}

// الكتابة على الشاشة
void LcdWrite(byte dc, byte data) {
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

// تحديد موقع الكتابة
void gotoXY(int x, int y) {
  LcdWrite(LCD_C, 0x80 | x);
  LcdWrite(LCD_C, 0x40 | y);
}

// عرض الصورة على الشاشة
void LcdBitmap(const char my_array[]) {
  for (int index = 0; index < (LCD_X * LCD_Y / 8); index++) {
    LcdWrite(LCD_D, my_array[index]);
  }
}

// عرض صورة تلقائية
void displayAutoImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(autoImages[index]);
}

// عرض صورة يدوية
void displayManualImage(int index) {
  LcdClear();
  gotoXY(0, 0);
  LcdBitmap(manualImages[index]);
}

// التبديل للوضع التلقائي
void handleAutoMode() {
  currentAutoImage = (currentAutoImage + 1) % (sizeof(autoImages) / sizeof(autoImages[0]));
  displayAutoImage(currentAutoImage);
  delay(800);

  if (isButtonPressed()) {
    autoMode = false;
    lastButtonPressTime = millis();
    delay(200);
    displayManualImage(currentManualImage);
  }
}

// التبديل للوضع اليدوي
void handleManualMode() {
  unsigned long currentTime = millis();

  if (digitalRead(BUTTON_UP) == HIGH) {
    currentManualImage = (currentManualImage - 1 + (sizeof(manualImages) / sizeof(manualImages[0]))) % (sizeof(manualImages) / sizeof(manualImages[0]));
    displayManualImage(currentManualImage);
    lastButtonPressTime = currentTime;
    delay(200);
  }
  if (digitalRead(BUTTON_DOWN) == HIGH) {
    currentManualImage = (currentManualImage + 1) % (sizeof(manualImages) / sizeof(manualImages[0]));
    displayManualImage(currentManualImage);
    lastButtonPressTime = currentTime;
    delay(200);
  }
  if (digitalRead(BUTTON_SELECT) == HIGH) {
    autoMode = true;
    delay(200);
    displayAutoImage(currentAutoImage);
  }

  if (currentTime - lastButtonPressTime > timeoutDuration) {
    autoMode = true;
    displayAutoImage(currentAutoImage);
  }
}

// التحقق من الضغط على أي زر
bool isButtonPressed() {
  return digitalRead(BUTTON_UP) == HIGH || digitalRead(BUTTON_DOWN) == HIGH || digitalRead(BUTTON_SELECT) == HIGH || digitalRead(BUTTON_SELECT_MODE) == HIGH;
}

// إعداد الأزرار
void setupButtons() {
  pinMode(BUTTON_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT_MODE, INPUT_PULLDOWN);
}

// تهيئة البرنامج
void setup() {
  setupLCD();
  setupButtons();
  displayAutoImage(currentAutoImage);
}

// الحلقة الرئيسية
void loop() {
  if (autoMode) {
    handleAutoMode();
  } else {
    handleManualMode();
  }
}
