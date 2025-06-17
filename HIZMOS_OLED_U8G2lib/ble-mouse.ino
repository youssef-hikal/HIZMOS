


String mouse_currentStatus = "Waiting...";
String mouse_lastPressed = "None";




void blemouse() {
  // Update connection status
  if (mouse_ble.isConnected()) {
    mouse_currentStatus = "Connected";
  } else {
    mouse_currentStatus = "Disconnected";
  }

  mouse_lastPressed = "None";

  if (mouse_ble.isConnected()) {
    if (digitalRead(BTN_UP) == LOW) {
      mouse_ble.move(0, -20);
      mouse_lastPressed = "UP";
    }
    else if (digitalRead(BTN_DOWN) == LOW) {
      mouse_ble.move(0, 20);
      mouse_lastPressed = "DOWN";
    }
    else if (digitalRead(BTN_LEFT) == LOW) {
      mouse_ble.move(-20, 0);
      mouse_lastPressed = "LEFT";
    }
    else if (digitalRead(BTN_RIGHT) == LOW) {
      mouse_ble.move(20, 0);
      mouse_lastPressed = "RIGHT";
    }
    else if (digitalRead(BTN_SELECT) == LOW) {
      mouse_ble.click(MOUSE_LEFT);
      mouse_lastPressed = "SELECT (L-Click)";
    }
    else if (digitalRead(BTN_BACK) == LOW) {
      mouse_ble.click(MOUSE_RIGHT);
      mouse_lastPressed = "BACK (R-Click)";
    }
  }

  // Update display
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(0, 15, ("Status: " + mouse_currentStatus).c_str());
  u8g2.drawStr(0, 35, ("Pressed: " + mouse_lastPressed).c_str());
  u8g2.sendBuffer();

  delay(100);
}

