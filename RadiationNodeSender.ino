#include <ESP8266WiFi.h>
#include <espnow.h>

// عنوان MAC الخاص بالجهاز المستقبل
uint8_t broadcastAddress[] = {0xC4, 0x5B, 0xBE, 0xEA, 0x33, 0x83};

// تعريف هيكل البيانات لإرسال درجة الحرارة
typedef struct analog_messagedata {
  float temperature = 0.0;
} analog_messagedata;

// إنشاء نسخة من الهيكل
analog_messagedata myanalogData;

// دالة استدعاء عند إرسال البيانات
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print(". Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println(". Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

// دالة حساب درجة الحرارة بناءً على مقاومة NTC
float calculateTemperature(int analogValue) {
  // تحويل القراءة التناظرية إلى جهد
  float voltage = analogValue * (3.3 / 1023.0);

  // معطيات NTC والمقاومة الثابتة
  const float Rfixed = 10000.0;  // المقاومة الثابتة (10kΩ)
  const float R25 = 10000.0;    // المقاومة عند 25 درجة مئوية (10kΩ)
  const float Beta = 3950.0;    // معامل بيتا
  const float T0 = 298.15;      // درجة الحرارة بالكلفن عند 25°C (298.15 K)

  // حساب مقاومة الـ NTC
  float Rntc = Rfixed * (3.3 / voltage - 1);

  // معادلة Steinhart-Hart لحساب درجة الحرارة بالكلفن
  float temperatureK = 1 / ((1 / T0) + (1 / Beta) * log(Rntc / R25));

  // تحويل الكلفن إلى درجة مئوية
  return temperatureK - 273.15;
}

void setup() {
  // تهيئة منفذ التسلسل
  Serial.begin(115200);

  // إعداد الجهاز كـ Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // تهيئة ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // تسجيل دالة الإرسال
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // تسجيل الجهاز المستقبل
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  delay(500);

  // قراءة القيمة التناظرية من A0
  int analogValue = analogRead(A0);

  // حساب درجة الحرارة
  myanalogData.temperature = calculateTemperature(analogValue);

  // طباعة درجة الحرارة في Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(myanalogData.temperature);

  // إرسال درجة الحرارة عبر ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *)&myanalogData, sizeof(myanalogData));
}
