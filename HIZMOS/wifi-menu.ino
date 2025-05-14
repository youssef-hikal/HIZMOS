void wifimenu() {
  handlewifiInput();
  if (!inwifiSubMenu) {
    drawwifiMainMenu();
  }
}

void handlewifiInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (inwifiSubMenu) {
      wifisubMenuSelection = (wifisubMenuSelection - 1 + 2) % 2;
    } else {
      wificurrentSelection = (wificurrentSelection - 1 + wifimenuSize) % wifimenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (inwifiSubMenu) {
      wifisubMenuSelection = (wifisubMenuSelection + 1) % 2;
    } else {
      wificurrentSelection = (wificurrentSelection + 1) % wifimenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (inwifiSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executewifiAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (inwifiSubMenu) {
      inwifiSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawwifiMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < wifimenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == wificurrentSelection) {
      display.fillRoundRect(0, y, 80, 10,2, 1); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(wifimenuItems[i]);
  }
  display.display();
}

void executewifiAction() {
  if (wificurrentSelection == 1) { // If "deauth" is selected
    wifisubmenu();
  } else {
    inwifiSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (wificurrentSelection) {
      case 0: // Write
        if (digitalRead(BTN_SELECT)==0 ){
      delay(100);
        inscanningwifi= 1;
        scanNetworks();
     while(inscanningwifi == 1){
       scanwifi();
       if(digitalRead(BTN_BACK)==0  ){
          delay(80);
          inscanningwifi= 0;
       }

     }
     }
        break;
      case 2: // Save
        display.println(".lol..");
        break;
      case 3: // Emulate
        display.println("beacon<on>...");
        break;
    }
    
    display.println("\nPress BACK to return");
    display.display();
    
    // Wait for BACK button press
    while (digitalRead(BTN_BACK) != LOW) {
      delay(50);
    }
    inwifiSubMenu = false;
  }
}


void scanwifi(){
  
  drawscanMenu();
  checkscanButtons();
  
  delay(100);
}

void scanNetworks() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Scanning...");
  display.display();
  Serial.println("Scanning...");
  setColor(0, 250, 0);
  

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  numNetworks = WiFi.scanNetworks(false, true);
  numNetworks = numNetworks > 20 ? 20 : numNetworks;

  if (numNetworks == 0) {
    display.clearDisplay();
    display.print("No networks!");
    Serial.println("no tetworks");
    display.display();
    delay(1000);
    return;
  }

  for (int i = 0; i < numNetworks; i++) {
    ssidList[i] = WiFi.SSID(i);
    if (ssidList[i] == "") {
      ssidList[i] = "Hidden SSID";
    }
    rssiList[i] = WiFi.RSSI(i);
  }
}

void drawscanMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
   setColor(0, 0, 0);
  if (numNetworks == 0) {
    display.print("No networks");
    setColor(250, 0, 0);
    display.display();
    return;
  }

  int endIdx = min(displayStart + maxDisplayLines, numNetworks);
  
  for (int i = displayStart; i < endIdx; i++) {
    int y = (i - displayStart) * 10;
    
    if (i == selectedNetwork) {
      display.fillRoundRect(0, y, 80, 10,2, 1);
      display.setTextColor(WHITE, BLACK);
    } else {
      display.setTextColor(BLACK);  // Normal text color
    }
    
    display.setCursor(2, y + 2);
    display.print(ssidList[i].substring(0, 12));
  }
  display.display();
}

void checkscanButtons() {
  if (millis() - lastButtonPress < debounceDelay) return;

  // UP Button
  if (digitalRead(BTN_UP) == LOW) {
    handleUp();
    while (digitalRead(BTN_UP) == LOW);
  }

  // DOWN Button
  if (digitalRead(BTN_DOWN) == LOW) {
    handleDown();
    while (digitalRead(BTN_DOWN) == LOW);
  }

  // SELECT Button
  if (digitalRead(BTN_SELECT) == LOW) {
    handleSelect();
    while (digitalRead(BTN_SELECT) == LOW);
  }

  // LEFT Button (Rescan)

}

void handleUp() {
  lastButtonPress = millis();
  if (selectedNetwork > 0) {
    selectedNetwork--;
    if (selectedNetwork < displayStart) {
      displayStart = selectedNetwork;
    }
  }
}

void handleDown() {
  lastButtonPress = millis();
  if (selectedNetwork < numNetworks - 1) {
    selectedNetwork++;
    if (selectedNetwork >= displayStart + maxDisplayLines) {
      displayStart++;
    }
  }
}

void handleSelect() {
  lastButtonPress = millis();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Selected:");
  display.setCursor(0, 10);
  display.print(ssidList[selectedNetwork]);
  display.display();
  delay(2000);
}


void wifisubmenu() {
  inwifiSubMenu = true;
  const char *wifisubMenuItems[] = {"ON", "OFF"};
  
  while (inwifiSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == wifisubMenuSelection) {
        display.fillRoundRect(0, y, 80, 10,2, 1); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(wifisubMenuItems[i]);
    }
    display.display();
    
    handlewifiInput();
  }
}