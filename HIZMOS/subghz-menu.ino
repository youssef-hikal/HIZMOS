void subghzmenu(){
  setColor(0, 0, 0);
  handlesubghzInput();
  if (!insubghzSubMenu) {
    drawsubghzMainMenu();
  }
}

void handlesubghzInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    if (insubghzSubMenu) {
      subghzsubMenuSelection = (subghzsubMenuSelection - 1 + 2) % 2;
    } else {
      subghzcurrentSelection = (subghzcurrentSelection - 1 + subghzmenuSize) % subghzmenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    if (insubghzSubMenu) {
      subghzsubMenuSelection = (subghzsubMenuSelection + 1) % 2;
    } else {
      subghzcurrentSelection = (subghzcurrentSelection + 1) % subghzmenuSize;
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    if (insubghzSubMenu) {
      // Execute submenu action (currently just placeholder)
    } else {
      executesubghzAction();
    }
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW) {
    if (insubghzSubMenu) {
      insubghzSubMenu = false;
    }
    lastDebounceTime = millis();
  }
}

void drawsubghzMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < subghzmenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == subghzcurrentSelection) {
      display.fillRoundRect(0, y, 80, 10,2, 1); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(subghzmenuItems[i]);
  }
  display.display();
}

void executesubghzAction() {
  if (subghzcurrentSelection == 1) { // If "Read" is selected
    subghzsubmenu();
  } else {
    insubghzSubMenu = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (subghzcurrentSelection) {
      case 0: // read
        if (digitalRead(BTN_SELECT)==0 ){
      delay(100);
        inreadingsubghz= 1;
        LcdBitmap(readingsubghz);
     while(inreadingsubghz == 1){
       readsubghz();
        if(digitalRead(BTN_SELECT) ==0 ){

          Serial.println("SENT");
          LcdBitmap(sendsubghz);
          delay(50);
          
          LcdBitmap(sendsubghz2);
          delay(50);
          LcdBitmap(sendsubghz);
          delay(50);
          LcdBitmap(sendsubghz2);
          delay(50);
          LcdBitmap(sendsubghz);
          delay(50);
          
          LcdBitmap(sendsubghz2);
          delay(50);
          LcdBitmap(sendsubghz);
          delay(50);
          LcdBitmap(sendsubghz2);
          delay(50);

          mysender.send(value,bitLength);
          LcdBitmap(sendsubghz);
          mysender.send(value,bitLength);
          

    }
       if(digitalRead(BTN_BACK)==0  ){
          delay(80);
         inreadingsubghz = 0;
       }

     }
     }
        break;
      case 2: // Save
        display.println("JAMMING(433)");
        digitalWrite(6,1);
        setColor(250, 0, 0);
         setColor(0, 0, 0);
         setColor(250, 0, 0);
         setColor(0, 0, 0);
         setColor(250, 0, 0);
        break;
      case 3: // Emulate
        display.println("JAMMING(960)");
         tone(6,9600);
         setColor(250, 0, 0);
         setColor(0, 0, 0);
         setColor(250, 0, 0);
         setColor(0, 0, 0);
         setColor(250, 0, 0);
        break;
    }
    
    display.println("\nPress BACK to return");
    display.display();
    
    // Wait for BACK button press
    while (digitalRead(BTN_BACK) != LOW) {
      delay(50);
    }
    insubghzSubMenu = false;
  }
}

void subghzsubmenu() {
  insubghzSubMenu = true;
  const char *subghzsubMenuItems[] = {"ON", "OFF"};
  
  while (insubghzSubMenu) {
    display.clearDisplay();
    
    for (int i = 0; i < 2; i++) {
      int y = i * 10; // Adjust spacing for submenu items
      if (i == subghzsubMenuSelection) {
        display.fillRoundRect(0, y, 84, 10,2, 1); // Draw black rectangle for selected item
        display.setTextColor(WHITE);
      } else {
        display.setTextColor(BLACK);
      }
      display.setCursor(2, y);
      display.println(subghzsubMenuItems[i]);
    }
    display.display();
    
    handlesubghzInput();
  }
}
void readsubghz(){
  if (mySwitch.available()) {

    setColor(250, 0, 0);
    setColor(0, 0, 0);
    setColor(250, 0, 0);
    setColor(0, 0, 0);
    setColor(250, 0, 0);
    setColor(0, 0, 0);



 
    
   value = mySwitch.getReceivedValue();
   bitLength = mySwitch.getReceivedBitlength();
   bytes = (bitLength + 7) / 8; // Calculate bytes from bits


  
    
    // Clear display and show received data
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Value: ");
    display.println(value);
    display.print("Bytes: ");
    display.println(bytes);
    display.print("Bits: ");
    display.println(bitLength);
    display.setCursor(0,40);
    display.print("back");
    display.println(" save");
    display.setCursor(59,40);
    display.print("send");
  
    display.display();

    
  
    // Also send to serial monitor for debugging
    Serial.print("Received Value: ");
    Serial.print(value);
    Serial.print(" | Bits: ");
    Serial.print(bitLength);
    Serial.print(" | Bytes: ");
    Serial.println(bytes);

    // Reset receiver
    mySwitch.resetAvailable();
    
  }


  
}