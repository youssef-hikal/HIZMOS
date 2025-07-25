


String mouse_currentStatus = "Waiting...";
String mouse_lastPressed = "None";




void blemouse() {
  static const unsigned char image_ArrowDownEmpty_bits[] U8X8_PROGMEM = {0xf8,0x07,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x0f,0x3c,0x01,0x20,0x02,0x10,0x04,0x08,0x08,0x04,0x10,0x02,0x20,0x01,0xc0,0x00};
  static const unsigned char image_ArrowleftEmpty_bits[] U8X8_PROGMEM = {0xc0,0x00,0xa0,0x00,0x90,0x00,0x88,0x7f,0x04,0x40,0x02,0x40,0x01,0x40,0x01,0x40,0x02,0x40,0x04,0x40,0x88,0x7f,0x90,0x00,0xa0,0x00,0xc0,0x00};
  static const unsigned char image_ArrowrightEmpty_bits[] U8X8_PROGMEM = {0x80,0x01,0x80,0x02,0x80,0x04,0xff,0x08,0x01,0x10,0x01,0x20,0x01,0x40,0x01,0x40,0x01,0x20,0x01,0x10,0xff,0x08,0x80,0x04,0x80,0x02,0x80,0x01};
  static const unsigned char image_ArrowUpEmpty_bits[] U8X8_PROGMEM = {0xc0,0x00,0x20,0x01,0x10,0x02,0x08,0x04,0x04,0x08,0x02,0x10,0x01,0x20,0x0f,0x3c,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0xf8,0x07};
  static const unsigned char image_Ble_connected_bits[] U8X8_PROGMEM = {0xe0,0x03,0xf8,0x0f,0x7c,0x1f,0x7e,0x3e,0x6e,0x3d,0x5f,0x7b,0x3f,0x7d,0x7f,0x7e,0x3f,0x7d,0x5f,0x7b,0x6e,0x3d,0x7e,0x3e,0x7c,0x1f,0xf8,0x0f,0xe0,0x03};
  static const unsigned char image_Ble_disconnected_bits[] U8X8_PROGMEM = {0xe0,0x03,0x18,0x0c,0x84,0x10,0x82,0x21,0x92,0x22,0xa1,0x44,0xc1,0x42,0x81,0x41,0xc1,0x42,0xa1,0x44,0x92,0x22,0x82,0x21,0x84,0x10,0x18,0x0c,0xe0,0x03};
  static const unsigned char image_mouse_click_bits[] U8X8_PROGMEM = {0xe0,0x03,0x18,0x0c,0x04,0x10,0xc2,0x21,0xf2,0x27,0xf1,0x47,0xf9,0x4f,0xf9,0x4f,0xf9,0x4f,0xf1,0x47,0xf2,0x27,0xc2,0x21,0x04,0x10,0x18,0x0c,0xe0,0x03};

  String mouse_currentStatus = "";
  String mouse_lastPressed = "None";

  // Update connection status
  bool isConnected = mouse_ble.isConnected();
  mouse_currentStatus = isConnected ? "Connected" : "Disconnected";

  if (isConnected) {
    if (digitalRead(BTN_UP) == LOW) {
      mouse_ble.move(0, -40);
      mouse_lastPressed = "UP";
    } else if (digitalRead(BTN_DOWN) == LOW) {
      mouse_ble.move(0, 50);
      mouse_lastPressed = "DOWN";
    } else if (digitalRead(BTN_LEFT) == LOW) {
      mouse_ble.move(-50, 0);
      mouse_lastPressed = "LEFT";
    } else if (digitalRead(BTN_RIGHT) == LOW) {
      mouse_ble.move(50, 0);
      mouse_lastPressed = "RIGHT";
    } else if (digitalRead(BTN_SELECT) == LOW) {
      mouse_ble.click(MOUSE_LEFT);
      mouse_lastPressed = "SELECT";
    } else if (digitalRead(BTN_BACK) == LOW) {
      mouse_ble.click(MOUSE_RIGHT);
      mouse_lastPressed = "BACK";
    }
  }

  // Display with icons
  u8g2.clearBuffer();

  // BLE status icon
  if (isConnected) {
    u8g2.drawXBMP(108, 3, 15, 15, image_Ble_connected_bits);
  } else {
    u8g2.drawXBMP(108, 3, 15, 15, image_Ble_disconnected_bits);
  }

  // Direction icons layout
  u8g2.drawXBMP(61, 9, 14, 15, image_ArrowUpEmpty_bits);     // UP
  u8g2.drawXBMP(61, 41, 14, 15, image_ArrowDownEmpty_bits);   // DOWN
  u8g2.drawXBMP(44, 25, 15, 14, image_ArrowleftEmpty_bits);   // LEFT
  u8g2.drawXBMP(76, 25, 15, 14, image_ArrowrightEmpty_bits);  // RIGHT
  u8g2.drawXBMP(60, 25, 15, 16, image_mouse_click_bits);      // CENTER (select)

  // Highlight the pressed button (optional: draw rectangle)
  if (mouse_lastPressed == "UP") u8g2.drawFrame(60, 8, 16, 17);
  else if (mouse_lastPressed == "DOWN") u8g2.drawFrame(60, 40, 16, 17);
  else if (mouse_lastPressed == "LEFT") u8g2.drawFrame(43, 24, 17, 15);
  else if (mouse_lastPressed == "RIGHT") u8g2.drawFrame(75, 24, 17, 15);
  else if (mouse_lastPressed == "SELECT") u8g2.drawFrame(59, 24, 17, 17);
  else if (mouse_lastPressed == "BACK") {
    u8g2.setCursor(0, 64);
    u8g2.print("Right Click");
  }

  u8g2.sendBuffer();
  delay(50);
}


