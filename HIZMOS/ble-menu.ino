void blemenu(){
  inblehandleInput();
  if (!inSubMenu) {
    drawMainMenu();
  }
}

void displayImage(const uint8_t *image) {
  sendCommand(0x80 | 0);  // X = 0
  sendCommand(0x40 | 0);  // Y = 0

  for (uint16_t i = 0; i < 504; i++) {
    sendData(image[i]);
  }
}

void displayText(const char *text) {
  sendCommand(0x80 | 0);  // X = 0
  sendCommand(0x40 | 2);  // Y = 2 (centered)
  

}

void inblehandleButtons() {
  if (digitalRead(BTN_UP) == LOW) {
    imageIndex = (imageIndex - 1 + totalImages) % totalImages;
    
    delay(150); // Debounce delay
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    imageIndex = (imageIndex + 1) % totalImages;
    ;
    delay(50);
  }

  
}


 void handleBLEKEYBOARD(){
  



}










void blekeyboard(){

 inblehandleButtons();
 displayImage(images[imageIndex]);
  delay(50);
  handleBLEKEYBOARD();

}

void blemouse(){
  
    
  }





void inblehandleInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (inSubMenu) {
      subMenuSelection = (subMenuSelection - 1 + 2) % 2;
    } else {
      currentSelection = (currentSelection - 1 + menuSize) % menuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (inSubMenu) {
      subMenuSelection = (subMenuSelection + 1) % 2;
    } else {
      currentSelection = (currentSelection + 1) % menuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (inSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executeAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (inSubMenu) {
      inSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < menuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == currentSelection) {
      display.fillRoundRect(0, y, 80, 9,2,1); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(menuItems[i]);
  }
  display.display();
}

void executeAction() {
  
    
    switch (currentSelection) {
      case 0:
      inblemouse =1;
         
         while(inblemouse==1){
          blemouse();
          
           if(digitalRead(BTN_BACK) ==0 && digitalRead(BTN_SELECT) ==0  ){
            inblemouse =0;
           
            
  

          }
               }
           
      break;
      case 1: // ble keyboard

            inblekeyboard =1;
           
         while(inblekeyboard==1){
          blekeyboard();
          
           if(digitalRead(BTN_BACK) ==0 ){
            inblekeyboard =0;
            
            }
            }
       
      break;
      case 2: // ble script
        
      break;
     
    }
    
    
    
         // Wait for BACK button press
    while (digitalRead(BTN_BACK) != LOW) {
      delay(50);
    }
    inSubMenu = false;
  }

 