#include <Adafruit_NeoPixel.h>
#include <Update.h>

#define PIN        48    // تحديد منفذ GPIO 48
#define NUMPIXELS  1     // عدد الـ NeoPixel

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  strip.begin();            // تهيئة الـ NeoPixel
  strip.show();             // تأكيد أن الإضاءة تكون إيقاف في البداية

  // في حالة عادية: اللون الأخضر
  setNormalColor();
}

void loop() {
  // الكود الطبيعي بتاعك هنا (مثلاً ليد يومض أو أي كود آخر)
  
  // لو فيه حاجة تتعامل مع DFU وتحديث:
  if (Serial.available()) {
    handleUpdate();
  }
}

void handleUpdate() {
  Serial.println("بدء التحديث عبر Serial...");
  
  // في وضع التحديث: اللون الأحمر
  setColor(255, 0, 0); // أحمر: بدأ التحديث

  // بدء التحديث
  if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
    Serial.println("فشل بدء التحديث!");
    return;
  }

  size_t written = 0;
  size_t total = 0;

  // نحسب الحجم الإجمالي للبيانات المرسلة
  while (Serial.available()) {
    written = Update.writeStream(Serial);
    total += written;
    if (total > 0) {
      float progress = (float)total / (float)Update.size() * 100;
      Serial.printf("نسبة التحديث: %.2f%%\n", progress);
    }
  }

  // انتهى التحديث
  if (Update.end()) {
    if (Update.isFinished()) {
      Serial.println("التحديث مكتمل، إعادة تشغيل...");
      // بعد التحديث: اللون البنفسجي
      setColor(128, 0, 128); // بنفسجي
      delay(1000);
      ESP.restart(); // إعادة التشغيل بعد التحديث
    } else {
      Serial.println("التحديث لم ينتهي بنجاح!");
    }
  } else {
    Serial.printf("خطأ: %s\n", Update.errorString());
  }
}

// تغيير اللون حسب الوضع
void setColor(int r, int g, int b) {
  strip.setPixelColor(0, strip.Color(r, g, b));
  strip.show();
}

// في حالة عادية: اللون الأخضر
void setNormalColor() {
  setColor(0, 255, 0); // أخضر
}
