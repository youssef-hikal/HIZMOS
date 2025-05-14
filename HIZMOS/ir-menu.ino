void irmenu(){
  scanning= 1;
  handleInput();
  if (!inirSubMenu) {
    drawirMainMenu();
  }
}

void handleInput() {
  if ((millis() - lastDebounceTime) < debounceDelay) return;

  if (digitalRead(BTN_UP) == LOW) {
    ircurrentSelection = (ircurrentSelection - 1 + irmenuSize) % irmenuSize;
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    ircurrentSelection = (ircurrentSelection + 1) % irmenuSize;
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_SELECT) == LOW) {
    irexecuteAction();
    lastDebounceTime = millis();
  }
  else if (digitalRead(BTN_BACK) == LOW && inirSubMenu) {
    inirSubMenu = false;
    lastDebounceTime = millis();
  }
}



void drawirMainMenu() {
  display.clearDisplay();
  
  for (int i = 0; i < irmenuSize; i++) {
    int y = i * 10; // Adjust spacing for each menu item
    if (i == ircurrentSelection) {
      display.fillRoundRect(0, y, 80, 9,2,1); // Draw black rectangle for selected item
      display.setTextColor(WHITE);
    } else {
      display.setTextColor(BLACK);
    }
    display.setCursor(2, y);
    display.println(irmenuItems[i]);
  }
  display.display();
}

void irexecuteAction() {
  inirSubMenu = true;
  display.clearDisplay();
  display.setCursor(0, 0);
  
  switch (ircurrentSelection) {
    case 0: // Read
      display.display();
      LcdBitmap(send1);
      delay(100);
    LcdBitmap(send2);
    delay(300);
    LcdBitmap(send3);
    delay(300);
    LcdBitmap(send3);
    if(digitalRead(BTN_BACK)==0){
          delay(80);
          inirmenu= 0;
       }

      break;
    case 1: // read ir 
       
      display.display();
       if (digitalRead(BTN_SELECT)==0 ){
      delay(100);
        scanning= 1;
        LcdBitmap(readsignal);
     while(scanning == 1){
         recvIR();
       if(digitalRead(BTN_BACK)==0  ){
          delay(80);
         scanning = 0;
       }

     }
     }
      
      
      
      
      break;
    case 2: // Saved ir
    if (digitalRead(BTN_SELECT)==0 ){
      delay(100);
        scanning= 1;
        
     while(scanning == 1){
         listIRFiles();
       if(digitalRead(BTN_BACK)==0  ){
          delay(80);
         scanning = 0;
       }

     }
     }
     
    
      display.display();
      break;
  
  }
  
  
  // Wait for BACK button press
  while (digitalRead(BTN_BACK) != LOW) {
    delay(50);
  }
  inirSubMenu = false;
}



// ==== Receive IR ====
void recvIR() {
  LcdBitmap(readsignal);
  if (IrReceiver.decode()) {

      setColor(250, 0, 0);
      setColor(0, 0, 0);
      setColor(250, 0, 0);
      setColor(0, 0, 0);


    lastIRRaw = "";
    String hexCode = String(IrReceiver.decodedIRData.decodedRawData, HEX);

    uint8_t length = IrReceiver.decodedIRData.rawDataPtr->rawlen;
    for (uint8_t i = 1; i < length; i++) {
      lastIRRaw += String(IrReceiver.decodedIRData.rawDataPtr->rawbuf[i] * MICROS_PER_TICK);
      if (i < length - 1)
        lastIRRaw += ",";
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("IR Recived!");
    display.setCursor(0, 10);
    display.print("Hex:");
    display.println(hexCode);
    display.setCursor(0, 20);
    display.print("Type:");
    display.setCursor(30, 20);
    display.print(getProtocolName(IrReceiver.decodedIRData.protocol));
    display.setCursor(0,40);
    display.print("back");
    display.println(" save");
    display.setCursor(59,40);
    display.print("send");
    display.display();
    if(getProtocolName(IrReceiver.decodedIRData.protocol)=="NEC"){

          LcdBitmap(recivednecir);
        } 

        if(getProtocolName(IrReceiver.decodedIRData.protocol)=="UNKNOWN"){

          LcdBitmap(recivedunknownir);
        } 
    

    IrReceiver.resume();

    while (true) {
      if (digitalRead(BTN_BACK) == LOW) { inirSubmenu = false;  break; }
      if (digitalRead(BTN_SELECT) == LOW) {
        saveRawToFile();
        showMessage("Saved!");
        delay(1000);
        inirSubmenu = false;  break;
      }
      if (digitalRead(BTN_UP) == LOW) {
        replayRaw(lastIRRaw);
        LcdBitmap(replayedir);
        
        delay(1000);
        inirSubmenu = false;  break;
      }
    }
  }
}






String inputName() {
  const char charSet[] = "*ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_ ";
  const uint8_t charSetSize = sizeof(charSet) - 1;

  char name[7] = {'*', '*', '*', '*', '*', '*'};
  uint8_t pos = 0;
  bool editing = true;

  unsigned long backPressStart = 0;

  while (editing) {
    // UI
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Name IR Signal:");
    display.setCursor(0, 12);
    for (uint8_t i = 0; i < 6; i++) {
      if (i == pos) {
        display.print("[");
        display.print(name[i]);
        display.print("]");
      } else {
        display.print(" ");
        display.print(name[i]);
        display.print(" ");
      }
    }
    display.setCursor(0, 36);
    display.print("S:Next  B:Back");
    display.display();

    // Scroll UP
    if (digitalRead(BTN_UP) == LOW) {
      for (uint8_t j = 0; j < charSetSize; j++) {
        if (name[pos] == charSet[j]) {
          name[pos] = charSet[(j + 1) % charSetSize];
          break;
        }
      }
      delay(150);
    }

    // Scroll DOWN
    if (digitalRead(BTN_DOWN) == LOW) {
      for (uint8_t j = 0; j < charSetSize; j++) {
        if (name[pos] == charSet[j]) {
          name[pos] = charSet[(j + charSetSize - 1) % charSetSize];
          break;
        }
      }
      delay(150);
    }

    // NEXT CHAR
    if (digitalRead(BTN_SELECT) == LOW) {
      if (pos < 5) {
        pos++;
      } else {
        editing = false; // Done naming
      }
      delay(200);
    }

    // BACK (Short = go back, Long = cancel)
    if (digitalRead(BTN_BACK) == LOW) {
      backPressStart = millis();
      while (digitalRead(BTN_BACK) == LOW) {
        if (millis() - backPressStart > 800) {
          // Long press = cancel input
          return "";
        }
      }
      // Short press = move back
      if (pos > 0) pos--;
      delay(200);
    }
  }

  name[6] = '\0';
  String finalName = String(name);

  // Check for empty or invalid name
  if (finalName.indexOf("      ") >= 0 || finalName == "") {
    showMessage("Invalid Name!");
    delay(1000);
    return "";
  }

  return finalName;
}


// ==== Save IR to SD ====
void saveRawToFile() {
  String name = inputName();
  if (name == "") return;

  String filename = "/" + name + ".txt";
  File f = SD.open(filename, FILE_WRITE);
  if (f) {
    f.println(lastIRRaw);
    f.close();
  }
}


// ==== Replay Raw IR ====
void replayRaw(String data) {
  const int maxLen = 200;
  uint16_t raw[maxLen];
  int index = 0;

  char *token = strtok((char*)data.c_str(), ",");
  while (token != NULL && index < maxLen) {
    raw[index++] = atoi(token);
    token = strtok(NULL, ",");
  }

  IrSender.sendRaw(raw, index, 38); // 38kHz typical
}

// ==== IR File List ====
void listIRFiles() {
  File dir = SD.open("/");
  fileCount = 0;
  fileIndex = 0;

  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    if (!entry.isDirectory()) {
      fileList[fileCount++] = String(entry.name());
    }
    entry.close();
  }

  while (true) {
    display.clearDisplay();
    for (int i = 0; i < 5 && i + fileIndex < fileCount; i++) {
      if (i == 0)
        display.fillRect(0, i * 10, 84, 10, BLACK);
      display.setCursor(2, i * 10);
      display.setTextColor(i == 0 ? WHITE : BLACK);
      display.print(fileList[i + fileIndex]);
    }
    display.display();

    if (digitalRead(BTN_BACK) == LOW && digitalRead(BTN_UP) == LOW ) {
  String toDelete = "/" + fileList[fileIndex];
  SD.remove(toDelete);
  showMessage("Deleted");
  delay(1000);
  inirSubmenu = false;
  
  break;
}

    if (digitalRead(BTN_UP) == LOW) { fileIndex = max(0, fileIndex - 1); delay(200); }
    if (digitalRead(BTN_DOWN) == LOW) {
      if (fileIndex < fileCount - 1) fileIndex++;
      delay(200);
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      String path = "/" + fileList[fileIndex];
      File f = SD.open(path);
      if (f) {
        String raw = f.readStringUntil('\n');
        f.close();
        replayRaw(raw);
        LcdBitmap(replayedir);
        
      }
      delay(1000);
    }
  }
}

// ==== Helper ====
void showMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(BLACK);
  display.println(msg);
  display.display();
}





















void irreadvalue(){
     
     if(IrReceiver.decode() ) {
      LcdClear();
      setColor(250, 0, 0);
      setColor(0, 0, 0);
      setColor(250, 0, 0);
        
          uint32_t irvalue = IrReceiver.decodedIRData.decodedRawData;
        

        Serial.print("Received: 0x");
        Serial.println(irvalue, HEX);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Signal Recvd");
        display.setCursor(0, 10);
        display.print("HEX:");
        display.setCursor(30, 10);
        display.print(irvalue, HEX);

        display.setCursor(0, 20);
        display.print("Type:");
        display.setCursor(30, 20);
        display.print(getProtocolName(IrReceiver.decodedIRData.protocol));
        display.setCursor(0,40);
        display.print("back");
        display.println(" save");
        display.setCursor(59,40);
        display.print("send");
        
  
          
        display.display();

       
         
        IrReceiver.resume(); // Receive next signal

        

       
        
        






        
    }
     
    
    
}

String getProtocolName(decode_type_t protocol) {
    switch (protocol) {
        case NEC: return "NEC" ;
        case SONY: return "SONY";
        case RC5: return "RC5";
        case RC6: return "RC6";
        case JVC: return "JVC";
        case PANASONIC: return "PANASONIC";
        case LG: return "LG";
        case SAMSUNG: return "SAMSUNG";
        case WHYNTER: return "WHYNTER";
        case DENON: return "DENON";
        case UNKNOWN: return "UNKNOWN";
        default: return "OTHER";
    }
}








