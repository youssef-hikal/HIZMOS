void gpiomenu(){
  handlegpioInput();
  if (!ingpioSubMenu) {
    drawgpioMainMenu();
  }
}

void handlegpioInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (ingpioSubMenu) {
      gpiosubMenuSelection = (gpiosubMenuSelection - 1 + 2) % 2;
    } else {
      gpiocurrentSelection = (gpiocurrentSelection - 1 + gpiomenuSize) % gpiomenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (ingpioSubMenu) {
      gpiosubMenuSelection = (gpiosubMenuSelection + 1) % 2;
    } else {
      gpiocurrentSelection = (gpiocurrentSelection + 1) % gpiomenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (ingpioSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executegpioAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (ingpioSubMenu) {
      ingpioSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawgpioMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < gpiomenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == gpiocurrentSelection) {
      display.fillRect(0, y, 84, 10, BLACK); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(gpiomenuItems[i]);
  }
  display.display();
}

void executegpioAction() {
  if (gpiocurrentSelection == 0) { 
    gpiosubmenu();
  } else {
    ingpioSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (gpiocurrentSelection) {
      case 1: // Write
        display.println("connect sensor...");
        break;
      case 2: // Save
        display.println("connnect :)");
        break;
      case 3: // Emulate
        display.println("connnect :)...");
        break;
    }
    
    display.println("\nPress BACK to return");
    display.display();
    
    // Wait for BACK button press
    while (digitalRead(BTN_BACK) != LOW) {
      delay(50);
    }
    ingpioSubMenu = false;
  }
}

void gpiosubmenu() {
  ingpioSubMenu = true;
  const char *gpiosubMenuItems[] = {"ON", "OFF"};
  
  while (ingpioSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == gpiosubMenuSelection) {
        display.fillRoundRect(0, y, 78, 10,1,1); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(gpiosubMenuItems[i]);
    }
    display.display();
    
    handlegpioInput();
  }
}