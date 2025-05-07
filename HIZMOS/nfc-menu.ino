void nfcmenu(){
  handlenfcInput();
  if (!innfcSubMenu) {
    drawMainnfcMenu();
  }
}

void handlenfcInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (innfcSubMenu) {
      nfcsubMenuSelection = (nfcsubMenuSelection - 1 + 2) % 2;
    } else {
      nfccurrentSelection = (nfccurrentSelection - 1 + nfcmenuSize) % nfcmenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (innfcSubMenu) {
      nfcsubMenuSelection = (nfcsubMenuSelection + 1) % 2;
    } else {
      nfccurrentSelection = (nfccurrentSelection + 1) % nfcmenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (innfcSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      nfcexecuteAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (innfcSubMenu) {
      innfcSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawMainnfcMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < nfcmenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == nfccurrentSelection) {
      display.fillRoundRect(0, y, 80, 10,1,1 ); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(nfcmenuItems[i]);
  }
  display.display();
}

void nfcexecuteAction() {
  if (nfccurrentSelection == 0) { // If "Read" is selected
    nfcsubmenu();
  } else {
    innfcSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (nfccurrentSelection) {
      case 1: // Write
        display.println("Writing data...");
        break;
      case 2: // Save
        display.println("Saving...");
        break;
      case 3: // Emulate
        display.println("Emulating...");
        break;
    }
    
    display.println("\nPress BACK to return");
    display.display();
    
    // Wait for BACK button press
    while (digitalRead(BTN_BACK) != LOW) {
      delay(50);
    }
    innfcSubMenu = false;
  }
}

void nfcsubmenu() {
  innfcSubMenu = true;
  const char *nfcsubMenuItems[] = {"ON", "OFF"};
  
  while (innfcSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == nfcsubMenuSelection) {
        display.fillRoundRect(0, y, 70, 10,1,1); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(nfcsubMenuItems[i]);
    }
    display.display();
    
    handlenfcInput();
  }
}
