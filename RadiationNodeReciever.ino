#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// تعريف دبابيس شاشة Nokia 5110
#define RST_PIN D0
#define CE_PIN D1
#define DC_PIN D2
#define DIN_PIN D3
#define CLK_PIN D4

// إنشاء كائن الشاشة
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);

// هيكل البيانات لاستقبال درجة الحرارة
typedef struct analog_messagedata {
  float temperature;
} analog_messagedata;

// إنشاء نسخة من الهيكل
analog_messagedata myanalogData;

// دالة استدعاء عند استقبال البيانات
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myanalogData, incomingData, sizeof(myanalogData));

  // طباعة درجة الحرارة في Serial Monitor
  Serial.print("RADIATION: ");
  Serial.println(myanalogData.temperature);

  // عرض درجة الحرارة على شاشة Nokia 5110
  display.clearDisplay();
  display.setCursor(10, 10);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.println("RADIATION:");
  display.setCursor(20,20);
  display.print(myanalogData.temperature);
  display.println(" SV");
  display.display();
}

void setup() {
  // تهيئة Serial Monitor
  Serial.begin(115200);

  // تهيئة الشاشة
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.display();

  // إعداد الجهاز كـ Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // تهيئة ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // تسجيل دالة استقبال البيانات
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // لا يوجد شيء في حلقة البرنامج
}
