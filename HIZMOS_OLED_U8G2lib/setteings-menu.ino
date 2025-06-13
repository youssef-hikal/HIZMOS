void handlesettingsmenu() {
  const char* menuItems[] = {"show usage", "format sd", "restart", "batt info", "sd info", "about","check sys devices"};
  const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
  const int visibleItems = 3;

  static int selectedItem = 0;
  static int scrollOffset = 0;

  // ===== التعامل مع الأزرار =====
  if (digitalRead(BTN_UP) == LOW) {
    selectedItem--;
    if (selectedItem < 0) selectedItem = menuLength - 1;
    scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    selectedItem++;
    if (selectedItem >= menuLength) selectedItem = 0;
    scrollOffset = constrain(selectedItem - visibleItems + 1, 0, menuLength - visibleItems);
  }

  if (digitalRead(BTN_SELECT) == LOW) {
    switch (selectedItem) {
      case 0:
       runLoop(datausage);
        break;
      case 1:
      if (!SD.begin(SD_CS)) {
            drawBoxMessage("SD Card Error", "Initialization Failed!");
            while (1);
            }

            drawBoxMessage("Do you want to", "format SD card?", "Press SELECT to continue");
        runLoop(sdformater);
       break;
      case 2:
       drawConfirmationUI();
       runLoop(restartesp);
        break;
      case 3: 
      ///emulateNFCTag();
       break;
      case 4:

      if (!SD.begin(SD_CS)) {
    u8g2.clearBuffer();
    u8g2.drawStr(10, 30, "SD Card Failed!");
    u8g2.sendBuffer();
    while (1);
  }

        sdinfo_readStats();
        sdinfo_drawMainScreen();
       runLoop(showsdinfo);
        break;
        case 5:
       runLoop(about);
        break;

        case 6:
        runLoop(checksysdevices);
        break;
        
    }
  }

  // ===== عرض الشاشة =====
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14_tf); // Nice clean font

  for (int i = 0; i < visibleItems; i++) {
    int menuIndex = i + scrollOffset;
    if (menuIndex >= menuLength) break;

    int y = i * 20 + 16;

    if (menuIndex == selectedItem) {
      u8g2.drawRBox(4, y - 12, 120, 16, 4); // Rounded highlight
      u8g2.setDrawColor(0); // black text on white box
      u8g2.drawStr(10, y, menuItems[menuIndex]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(10, y, menuItems[menuIndex]);
    }
  }

  // ===== شريط التمرير =====
  int barX = 124;
  int spacing = 64 / menuLength;

  for (int i = 0; i < menuLength; i++) {
    int dotY = i * spacing + spacing / 2;
    if (i == selectedItem) {
      u8g2.drawBox(barX, dotY - 3, 3, 6);
    } else {
      u8g2.drawPixel(barX + 1, dotY);
    }
  }

  u8g2.sendBuffer();
  delay(150); // Debounce
}
