// ==== Globals ====
String irmenuItems[] = {"recv ir", "ir list"};
int irMenuIndex = 0;
bool inIrSubmenu = false;
#define MAX_IR_RAW_LEN 1024
char irLastIRRaw[MAX_IR_RAW_LEN];
char irBuffer[256];
String irRawData = "";
char irInfo[6][32];        // كل سطر من البيانات


int irFileIndex = 0;
String irFileList[20];
int irFileCount = 0;


// ==== Show Message ====
void showMessage(String msg) {
  u8g2.clearBuffer();
  u8g2.setCursor(0, 20);
  u8g2.print(msg);
  u8g2.sendBuffer();
}


void recvIR() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(20, 32, "Waiting IR...");
  u8g2.sendBuffer();
  if (digitalRead(BTN_BACK) == LOW) {
        delay(200);
        while(digitalRead(BTN_BACK) == LOW);
        inIrSubmenu = false;
        
        return;
      }

  if (IrReceiver.decode()) {
    uint32_t hex = IrReceiver.decodedIRData.decodedRawData;
    uint16_t addr = IrReceiver.decodedIRData.address;
    uint16_t cmd = IrReceiver.decodedIRData.command;
    uint8_t bits = IrReceiver.decodedIRData.numberOfBits;
    uint8_t rawlen = IrReceiver.decodedIRData.rawDataPtr->rawlen;
    const char* type = IrReceiver.getProtocolString();

    snprintf(irInfo[0], sizeof(irInfo[0]), "HEX: %08lX", hex);
    snprintf(irInfo[1], sizeof(irInfo[1]), "ADDR: %04X", addr);
    snprintf(irInfo[2], sizeof(irInfo[2]), "CMD: %04X", cmd);
    snprintf(irInfo[3], sizeof(irInfo[3]), "BITS: %d", bits);
    snprintf(irInfo[4], sizeof(irInfo[4]), "RAW LEN: %d", rawlen);
    snprintf(irInfo[5], sizeof(irInfo[5]), "TYPE: %s", type);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x7_mf);
    for (int i = 0; i < 6; i++) {
      u8g2.drawStr(0, 10 + i * 9, irInfo[i]);
    }
    u8g2.sendBuffer();

    irRawData = "";
    for (uint8_t i = 1; i < rawlen; i++) {
      irRawData += String(IrReceiver.decodedIRData.rawDataPtr->rawbuf[i] * MICROS_PER_TICK);
      if (i < rawlen - 1) irRawData += ",";
    }

    IrReceiver.resume();

    while (true) {
      if (digitalRead(BTN_BACK) == LOW) return;

      if (digitalRead(BTN_SELECT) == LOW) {
        delay(200);


      if (digitalRead(BTN_BACK) == LOW) {
        delay(200);
        while(digitalRead(BTN_BACK) == LOW);
        inIrSubmenu = false;
        
        return;
      }
      // بقية الأزرار كالسابق...
    

        while (digitalRead(BTN_SELECT) == LOW);
        String name = inputName();
        if (name != "") {
          File f = SD.open("/" + name + ".ir", FILE_WRITE);
          if (f) {
            f.println(irRawData);
            f.close();
            showMessage("Saved!");
          } else {
            showMessage("SD Error!");
          }
        } else {
          showMessage("Invalid Name");
        }
        delay(1000);
        return;
      }

      if (digitalRead(BTN_UP) == LOW) {
        replayRaw(irRawData);
        showMessage("Replayed!");
        delay(1000);
        return;
      }
    }
  }
}


String inputName() {
  const char* keys[] = {
    "<","A","B","C","D","E","F","G",
    "H","I","J","K","L","M","N","O",
    "P","Q","R","S","T","U","V","W",
    "X","Y","Z","0","1","2","3","4",
    "5","6","7","8","9","-","_","*",
    "/","@","#",".",",","ENTER"
  };

  const int totalKeys = sizeof(keys) / sizeof(keys[0]); // 46
  const int cols = 8;
  const int rows = (totalKeys + cols - 1) / cols; // = 6
  const int visibleRows = 3;
  const int maxNameLen = 10;

  int offsetY = 0;
  int cursorX = 0, cursorY = 0;
  String name = "";

  const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  for (int i = 0; i < 6; i++) name += charset[random(strlen(charset))];

  while (true) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tf);

    // Draw name
    u8g2.drawStr(0, 10, "NAME:");
    u8g2.drawStr(42, 10, name.c_str());

    // Draw keys
    for (int y = 0; y < visibleRows; y++) {
      for (int x = 0; x < cols; x++) {
        int keyIndex = (offsetY + y) * cols + x;
        if (keyIndex >= totalKeys) continue;

        int boxX = x * 16;
        int boxY = 14 + y * 16;

        const char* key = keys[keyIndex];
        int boxW = (strcmp(key, "ENTER") == 0) ? 30 : 14;

        if (x == cursorX && y == cursorY) {
          u8g2.drawBox(boxX, boxY, boxW, 14);
          u8g2.setDrawColor(0);
          u8g2.setCursor(boxX + 2, boxY + 11);
          u8g2.print(key);
          u8g2.setDrawColor(1);
        } else {
          u8g2.drawFrame(boxX, boxY, boxW, 14);
          u8g2.setCursor(boxX + 2, boxY + 11);
          u8g2.print(key);
        }
      }
    }

    u8g2.sendBuffer();

    // Controls
    if (digitalRead(BTN_UP) == LOW) {
      if (cursorY == 0 && offsetY > 0) {
        offsetY--;
      } else if (cursorY > 0) {
        cursorY--;
      }
      delay(150);
    }

    if (digitalRead(BTN_DOWN) == LOW) {
      if ((cursorY + offsetY + 1) * cols + cursorX < totalKeys && cursorY < visibleRows - 1) {
        cursorY++;
      } else if (offsetY + visibleRows < rows) {
        offsetY++;
      }
      delay(150);
    }

    if (digitalRead(BTN_LEFT) == LOW) {
      if (cursorX > 0) cursorX--;
      delay(150);
    }

    if (digitalRead(BTN_RIGHT) == LOW) {
      if ((offsetY + cursorY) * cols + cursorX + 1 < totalKeys && cursorX < cols - 1) cursorX++;
      delay(150);
    }

    if (digitalRead(BTN_SELECT) == LOW) {
      int keyIndex = (cursorY + offsetY) * cols + cursorX;
      if (keyIndex < totalKeys) {
        const char* key = keys[keyIndex];
        if (strcmp(key, "<") == 0) {
          if (name.length() > 0) name.remove(name.length() - 1);
        } else if (strcmp(key, "ENTER") == 0) {
          return name;
        } else {
          if (name.length() < maxNameLen) name += key;
        }
      }
      delay(200);
    }

    if (digitalRead(BTN_BACK) == LOW) {
      return ""; // Cancel input
    }
  }
}





void saveRawToFile() {
  String filename = inputName();  // من غير أي باراميتر

  if (filename == "") return;

  String fullpath = "/" + filename + ".ir";
  File f = SD.open(fullpath, FILE_WRITE);
  if (f) {
    f.println(irLastIRRaw);
    f.close();

    u8g2.clearBuffer();
    u8g2.setCursor(0, 30);
    u8g2.print("Saved as:");
    u8g2.setCursor(0, 45);
    u8g2.print(filename);
    u8g2.sendBuffer();
    delay(1000);
  } else {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 30);
    u8g2.print("Save Failed!");
    u8g2.sendBuffer();
    delay(1000);
  }
}




// ==== Replay IR ====
void replayRaw(String data) {
  const int maxLen = 200;
  uint16_t raw[maxLen];
  int index = 0;

  char buf[data.length() + 1];
  data.toCharArray(buf, sizeof(buf));
  char *token = strtok(buf, ",");
  while (token != NULL && index < maxLen) {
    raw[index++] = atoi(token);
    token = strtok(NULL, ",");
  }

  irsend.sendRaw(raw, index, 38);
}




void fileOptions(String filename) {
  const String opts[] = {"Run", "Rename", "Delete", "Info"};
  int optCount = 4, optIndex = 0;

  while (true) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, filename.c_str());

    // عرض القايمة بخط أوضح وحجم أكبر
    for (int i = 0; i < optCount; i++) {
      int y = 22 + i * 12;
      if (i == optIndex) {
        u8g2.drawBox(0, y - 10, 128, 12);
        u8g2.setDrawColor(0);
      } else {
        u8g2.setDrawColor(1);
      }
      u8g2.setCursor(4, y);
      u8g2.print(opts[i]);
    }
    u8g2.setDrawColor(1);
    u8g2.sendBuffer();

    // التنقل
    if (digitalRead(BTN_UP) == LOW) {
      optIndex = (optIndex + optCount - 1) % optCount;
      delay(200);
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      optIndex = (optIndex + 1) % optCount;
      delay(200);
    }

    // تنفيذ الاختيار
    if (digitalRead(BTN_SELECT) == LOW) {
      delay(200); while (digitalRead(BTN_SELECT) == LOW);
      String selected = opts[optIndex];

      if (selected == "Run") {
        File f = SD.open("/" + filename);
        if (f) {
          String raw = f.readStringUntil('\n'); f.close();
          replayRaw(raw.c_str());
          showMessage("Replayed");
        }
      }

      else if (selected == "Rename") {
        String newname = inputName();
        if (newname != "") {
          SD.rename("/" + filename, "/" + newname + ".ir");
          showMessage("Renamed");
          filename = newname + ".ir";
        } else showMessage("Invalid");
      }

      else if (selected == "Delete") {
        SD.remove("/" + filename);
        showMessage("Deleted");
        delay(800);
        break;
      }

      else if (selected == "Info") {
        File f = SD.open("/" + filename);
        if (!f) {
          showMessage("Can't open");
          continue;
        }

        std::vector<String> lines;
        while (f.available()) {
          String line = f.readStringUntil('\n');
          line.trim();
          lines.push_back(line);
        }
        f.close();

        int scroll = 0;
        const int visibleLines = 5;
        int totalLines = lines.size();

        while (true) {
          u8g2.clearBuffer();
          u8g2.setFont(u8g2_font_6x10_tf);
          u8g2.drawStr(0, 10, "Content:");

          // عرض السطور
          for (int i = 0; i < visibleLines; i++) {
            int index = scroll + i;
            if (index < totalLines) {
              u8g2.drawStr(0, 22 + i * 10, lines[index].c_str());
            }
          }

          // Scroll bar
          if (totalLines > visibleLines) {
            int scrollbarHeight = 40;
            int barY = 22;
            int barX = 124;
            int barH = (visibleLines * scrollbarHeight) / totalLines;
            int barPos = (scroll * scrollbarHeight) / totalLines;
            u8g2.drawFrame(barX, barY, 4, scrollbarHeight);
            u8g2.drawBox(barX, barY + barPos, 4, barH);
          }

          u8g2.sendBuffer();

          if (digitalRead(BTN_DOWN) == LOW && scroll + visibleLines < totalLines) {
            scroll++;
            delay(150);
          }
          if (digitalRead(BTN_UP) == LOW && scroll > 0) {
            scroll--;
            delay(150);
          }
          if (digitalRead(BTN_BACK) == LOW) {
            delay(200); while (digitalRead(BTN_BACK) == LOW);
            break;
          }
        }
      }
    }

    if (digitalRead(BTN_BACK) == LOW) {
      delay(200); while (digitalRead(BTN_BACK) == LOW);
      break;
    }
  }

  listIRFiles();  // عرض الملفات بعد الخروج
}



void listIRFiles() {
  String irFileList[20];
  int irFileCount = 0;
  int selectedIndex = 0, viewOffset = 0;

  showMessage("Loading...");
  delay(500);

  File dir = SD.open("/");
  File entry;
  while ((entry = dir.openNextFile())) {
    if (!entry.isDirectory()) {
      String name = String(entry.name());
      if (name.endsWith(".ir") && irFileCount < 20) {
        irFileList[irFileCount++] = name;
      }
    }
    entry.close();
  }
  dir.close();

  if (irFileCount == 0) {
    showMessage("No files");
    delay(1000);
    inIrSubmenu = false;
    
    return;
  }

  const int itemHeight = 14;
  const int visibleItems = 4;

  while (true) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);

    for (int i = 0; i < visibleItems; i++) {
      int fi = viewOffset + i;
      if (fi >= irFileCount) break;

      int y = i * itemHeight + 14;

      if (fi == selectedIndex) {
        u8g2.setDrawColor(1);
        u8g2.drawRBox(0, y - 11, 128, itemHeight, 3);  // شكل احترافي
        u8g2.setDrawColor(0);
      } else {
        u8g2.setDrawColor(1);
      }

      u8g2.setCursor(6, y - 2);
      u8g2.print(irFileList[fi]);
    }

    u8g2.sendBuffer();

    if (digitalRead(BTN_SELECT) == LOW) {
      delay(200); while(digitalRead(BTN_SELECT)==LOW);
      fileOptions(irFileList[selectedIndex]);
      break;
    }

    if (digitalRead(BTN_BACK) == LOW) {
      delay(200); inIrSubmenu = false;  break;
    }

    if (digitalRead(BTN_UP) == LOW) {
      if (selectedIndex > 0) selectedIndex--;
      if (selectedIndex < viewOffset) viewOffset--;
      delay(150);
    }

    if (digitalRead(BTN_DOWN) == LOW) {
      if (selectedIndex < irFileCount - 1) selectedIndex++;
      if (selectedIndex > viewOffset + visibleItems - 1) viewOffset++;
      delay(150);
    }
  }
}



