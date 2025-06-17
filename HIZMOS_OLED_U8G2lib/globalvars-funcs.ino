const uint8_t* nosdFrames[] = {
  nosd1, nosd2, nosd3, nosd4, nosd5, nosd6
};


void drawnosdcard() {
  static uint8_t currentFrame = 0;
  static unsigned long lastUpdate = 0;
  const unsigned long frameDuration = 200; // ms per frame

  if (millis() - lastUpdate >= frameDuration) {
    lastUpdate = millis();
    currentFrame = (currentFrame + 1) % 6;  // loop through 0 to 5
  }

  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, 128, 64, nosdFrames[currentFrame]);
  u8g2.sendBuffer();
}