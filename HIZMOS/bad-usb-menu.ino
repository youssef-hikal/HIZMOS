void badmenu() {
  handlebadInput();
  if (!badInSubMenu) {
    drawbadMainMenu();
  }
}

void handlebadInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (badInSubMenu) {
      badSubMenuSelection = (badSubMenuSelection - 1 + 2) % 2;
    } else {
      badCurrentSelection = (badCurrentSelection - 1 + badMenuSize) % badMenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (badInSubMenu) {
      badSubMenuSelection = (badSubMenuSelection + 1) % 2;
    } else {
      badCurrentSelection = (badCurrentSelection + 1) % badMenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (badInSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executebadAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (badInSubMenu) {
      badInSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawbadMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < badMenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == badCurrentSelection) {
      display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(badMenuItems[i]);
  }
  display.display();
}

void executebadAction() {
  if (badCurrentSelection == 0) { // If "Read" is selected
    badsubmenu();
  } else {
    badInSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (badCurrentSelection) {
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
    badInSubMenu = false;
  }
}

void badsubmenu() {
  badInSubMenu = true;
  const char *subMenuItems[] = {"ON", "OFF"};
  
  while (badInSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == badSubMenuSelection) {
        display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(subMenuItems[i]);
    }
    display.display();
    
    handlebadInput();
  }
}
