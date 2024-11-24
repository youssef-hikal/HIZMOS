// أبعاد شاشة Nokia 5110
#define LCD_X     84
#define LCD_Y     48

// تعريف أرجل الشاشة
#define PIN_RESET 8
#define PIN_SCE   5
#define PIN_DC    4
#define PIN_SDIN  7
#define PIN_SCLK  6

// أزرار التحكم
#define BUTTON_UP 16
#define BUTTON_DOWN 17
#define BUTTON_SELECT 18
#define BUTTON_BACK 19

// الليد المستخدم كـ LED
#define LED_PIN 25

// LCD Modes
#define LCD_C 0  // Command
#define LCD_D 1  // Data

// الصور اليدوية
const char manualImage1[] = { /* بيانات الصورة هنا */ };
const char manualImage2[] = { /* بيانات الصورة هنا */ };
const char manualImage3[] = { /* بيانات الصورة هنا */ };
const char manualImage4[] = { /* بيانات الصورة هنا */ };
const char manualImage5[] = { /* بيانات الصورة هنا */ };
const char manualImage6[] = { /* بيانات الصورة هنا */ };

// الصور التلقائية
const char autoImage1[] = { /* بيانات الصورة هنا */ };
const char autoImage2[] = { /* بيانات الصورة هنا */ };
const char autoImage3[] = { /* بيانات الصورة هنا */ };

// مصفوفة الصور
const char* manualImages[] = {manualImage1, manualImage2, manualImage3, manualImage4, manualImage5, manualImage6};
const char* autoImages[] = {autoImage1, autoImage2, autoImage3};

int totalManualImages = sizeof(manualImages) / sizeof(manualImages[0]);
int totalAutoImages = sizeof(autoImages) / sizeof(autoImages[0]);

int currentManualImage = 0;
int currentAutoImage = 0;

// الوضع الافتراضي
bool autoMode = true;
unsigned long lastButtonPressTime = 0;
const unsigned long timeoutDuration = 120000;  // 2 دقيقة

void setup() {
  // إعداد الشاشة
  LcdInitialise();
  LcdClear();

  // إعداد الأزرار
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);

  // إعداد LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // عرض الصورة التلقائية الأولى
  displayAutoImage(currentAutoImage);
}

void loop() {
  unsigned long currentTime = millis();

  if (autoMode) {
    // وضع التشغيل التلقائي
    currentAutoImage = (currentAutoImage + 1) % totalAutoImages;  // الصورة التالية
    displayAutoImage(currentAutoImage);
    delay(1000);  // الانتظار ثانية واحدة

    // التحقق من الضغط على أي زر للخروج من الوضع التلقائي
    if (digitalRead(BUTTON_UP) == LOW || digitalRead(BUTTON_DOWN) == LOW || 
        digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_BACK) == LOW) {
      autoMode = false;  // التبديل إلى الوضع اليدوي
      lastButtonPressTime = currentTime;
      delay(200);  // تأخير لإزالة الاهتزاز
      displayManualImage(currentManualImage);
    }
  } else {
    // باقي الكود في الجزء الثاني
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

// الدوال المساعدة للشاشة
void LcdClear() {
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++) {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise() {
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite(LCD_C, 0x21);  // أوامر LCD موسعة
  LcdWrite(LCD_C, 0xB1);  // ضبط التباين
  LcdWrite(LCD_C, 0x04);  // ضبط معامل الحرارة
  LcdWrite(LCD_C, 0x14);  // وضع التحيز
  LcdWrite(LCD_C, 0x0C);  // الوضع العادي
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void gotoXY(int x, int y) {
  LcdWrite(LCD_C, 0x80 | x);  // العمود
  LcdWrite(LCD_C, 0x40 | y);  // الصف
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
void loop() {
  unsigned long currentTime = millis();

  if (autoMode) {
    // وضع التشغيل التلقائي
    currentAutoImage = (currentAutoImage + 1) % totalAutoImages;  // الصورة التالية
    displayAutoImage(currentAutoImage);
    delay(1000);  // الانتظار ثانية واحدة

    // التحقق من الضغط على أي زر للخروج من الوضع التلقائي
    if (digitalRead(BUTTON_UP) == LOW || digitalRead(BUTTON_DOWN) == LOW || 
        digitalRead(BUTTON_SELECT) == LOW || digitalRead(BUTTON_BACK) == LOW) {
      autoMode = false;  // التبديل إلى الوضع اليدوي
      lastButtonPressTime = currentTime;
      delay(200);  // تأخير لإزالة الاهتزاز
      displayManualImage(currentManualImage);
    }
  } else {
    // وضع التشغيل اليدوي: التحكم عبر الأزرار
    if (digitalRead(BUTTON_UP) == LOW) {
      currentManualImage = (currentManualImage - 1 + totalManualImages) % totalManualImages;  // الصورة السابقة
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;  // تحديث وقت الضغط الأخير
      delay(200);  // تأخير لإزالة الاهتزاز
    }
    
    if (digitalRead(BUTTON_DOWN) == LOW) {
      currentManualImage = (currentManualImage + 1) % totalManualImages;  // الصورة التالية
      displayManualImage(currentManualImage);
      lastButtonPressTime = currentTime;  // تحديث وقت الضغط الأخير
      delay(200);  // تأخير لإزالة الاهتزاز
    }
    
    if (digitalRead(BUTTON_SELECT) == LOW) {
      // عند اختيار صورة معينة، تنفيذ إجراء
      if (currentManualImage == 0) {
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
      }
      if (currentManualImage == 1) {
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
      }
      if (currentManualImage == 2) {
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
      }
      if (currentManualImage == 3) {
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
      }
      if (currentManualImage == 4) {
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
      }
      if (currentManualImage == 5) {
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
      }
      lastButtonPressTime = currentTime;  // تحديث وقت الضغط الأخير
      delay(200);  // تأخير لإزالة الاهتزاز
    }

    if (digitalRead(BUTTON_BACK) == LOW) {
      autoMode = true;  // العودة إلى الوضع التلقائي
      delay(200);  // تأخير لإزالة الاهتزاز
      displayAutoImage(currentAutoImage);  // عرض الصورة التلقائية الحالية
    }

    // التحقق إذا مر وقت طويل بدون ضغط على الأزرار
    if (currentTime - lastButtonPressTime > timeoutDuration) {
      autoMode = true;  // العودة إلى الوضع التلقائي
      displayAutoImage(currentAutoImage);  // عرض الصورة التلقائية الحالية
    }
  }
}
