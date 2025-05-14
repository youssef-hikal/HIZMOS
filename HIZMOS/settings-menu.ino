void settings_menu() {
handleSettingsInput();
  if (!settingsInSubMenu) {
    drawSettingsMainMenu();
  }

}


void handleSettingsInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (settingsInSubMenu) {
      settingsSubMenuSelection = (settingsSubMenuSelection - 1 + 2) % 2;
    } else {
      settingsCurrentSelection = (settingsCurrentSelection - 1 + settingsMenuSize) % settingsMenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (settingsInSubMenu) {
      settingsSubMenuSelection = (settingsSubMenuSelection + 1) % 2;
    } else {
      settingsCurrentSelection = (settingsCurrentSelection + 1) % settingsMenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (settingsInSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executeSettingsAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (settingsInSubMenu) {
      settingsInSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawSettingsMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < settingsMenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == settingsCurrentSelection) {
      display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(settingsMenuItems[i]);
  }
  display.display();
}

void executeSettingsAction() {
  if (settingsCurrentSelection == 0) { // If "SHOW RAM" is selected
    settingsSubmenu();
  } else {
    settingsInSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (settingsCurrentSelection) {
      case 1: // FORMAT SD
        display.println("Formatting SD...");
        break;
      case 2: // RESTART
        display.println("Restarting...");
        break;
      case 3: // BAT INFO
        display.println("Reading battery info...");
        break;
      case 4: // SD INFO
        display.println("Reading SD info...");
        break;
    }
    
    display.println("\nPress BACK to return");
    display.display();
    
    // Wait for BACK button press
    while (digitalRead(BTN_BACK) != LOW) {
      delay(50);
    }
    settingsInSubMenu = false;
  }
}

void settingsSubmenu() {
  settingsInSubMenu = true;
  const char *subMenuItems[] = {"ON", "OFF"};
  
  while (settingsInSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == settingsSubMenuSelection) {
        display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(subMenuItems[i]);
    }
    display.display();
    
    handleSettingsInput();
  }
}

