#include <esp_now.h>
#include <WiFi.h>

#define X_AXIS_PIN 32
#define Y_AXIS_PIN 33
#define SWITCH_PIN 25
#define MODE_SWITCH_PIN 14 // زرار تبديل الوضع

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t receiverMacAddress[] = {0xAC, 0x67, 0xB2, 0x36, 0x7F, 0x28};

struct PacketData
{
  byte xAxisValue;
  byte yAxisValue;
  byte switchPressed;
  bool automaticMode; // وضع الأوتوماتيك
};
PacketData data;

bool automaticMode = false;

// This function adjusts joystick values and maps them to 0-254 with deadband
int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  if (value >= 2200)
  {
    value = map(value, 2200, 4095, 127, 254);
  }
  else if (value <= 1800)
  {
    value = map(value, 1800, 0, 127, 0);
  }
  else
  {
    value = 127;
  }

  if (reverse)
  {
    value = 254 - value;
  }
  return value;
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t ");
  Serial.println(status);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent" : "Message failed");
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("Success: Initialized ESP-NOW");
  }

  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  else
  {
    Serial.println("Success: Added peer");
  }

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP); // إعداد زر تبديل الوضع
}

void loop()
{
  data.xAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(X_AXIS_PIN), false);
  data.yAxisValue = mapAndAdjustJoystickDeadBandValues(analogRead(Y_AXIS_PIN), false);
  data.switchPressed = (digitalRead(SWITCH_PIN) == LOW);
  
  // تبديل وضع الأوتوماتيك عند الضغط على الزر
  if (digitalRead(MODE_SWITCH_PIN) == LOW)
  {
    delay(200); // منع الاهتزاز
    automaticMode = !automaticMode; // تبديل الوضع
  }
  data.automaticMode = automaticMode;

  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *)&data, sizeof(data));
  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
  }
  else
  {
    Serial.println("Error sending the data");
  }

  delay(50);
}
