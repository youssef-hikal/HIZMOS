void apps_menu(){
handleAppsInput();
  if (!appsInSubMenu) {
    drawAppsMainMenu();
  }

}


void handleAppsInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (appsInSubMenu) {
      appsSubMenuSelection = (appsSubMenuSelection - 1 + 2) % 2;
    } else {
      appsCurrentSelection = (appsCurrentSelection - 1 + appsMenuSize) % appsMenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (appsInSubMenu) {
      appsSubMenuSelection = (appsSubMenuSelection + 1) % 2;
    } else {
      appsCurrentSelection = (appsCurrentSelection + 1) % appsMenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (appsInSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executeAppsAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (appsInSubMenu) {
      appsInSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawAppsMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < appsMenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == appsCurrentSelection) {
      display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(appsMenuItems[i]);
  }
  display.display();
}

void executeAppsAction() {
  if (appsCurrentSelection == 0) { // If "SHOW RAM" is selected
    appsSubmenu();
  } else {
    appsInSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (appsCurrentSelection) {
      case 1: // متنساش تمسح ال داله ههه 
        display.println("app one logic");  /// حط هنا داله ال loop
        break;
      case 2: // // متنساش تمسح ال داله ههه 
        display.println("app two logic."); /// حط هنا داله ال loop
        break;
      case 3: //// متنساش تمسح ال داله ههه 
        display.println("app three logic");/// حط هنا داله ال loop
        break;
      case 4: // // متنساش تمسح ال داله ههه 
        display.println("app four logic ");/// حط هنا داله ال loop
        break;
    }
    ///////////////////اسمع الكلام يا عمور علشان انا خلقي ضيق 
    display.println("\nPress BACK to return");  // delete this if you need
    display.display();
    
    // Wait for BACK button press
    while (digitalRead(BTN_BACK) != LOW) {
      delay(50);
    }
    appsInSubMenu = false;
  }
}

void appsSubmenu() {
  appsInSubMenu = true;
  const char *subMenuItems[] = {"ON", "OFF"};
  
  while (appsInSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == appsSubMenuSelection) {
        display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(subMenuItems[i]);
    }
    display.display();
    
    handleAppsInput();
  }
}

