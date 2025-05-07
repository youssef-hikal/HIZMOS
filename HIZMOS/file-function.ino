void filefunction(){




while (!SD.begin(HSPI_CS, hspi)) {
        Serial.println("SD card initialization failed!");
        LcdBitmap(nosd1);
        delay(300);
        if(digitalRead(BTN_BACK)==0  ){
          delay(80);
          infilemenu= 0;
       }

        LcdBitmap(nosd2);
        if(digitalRead(BTN_BACK)==0  ){
          delay(80);
          infilemenu= 0;
       }

        delay(300);
        LcdBitmap(nosd3);
        delay(300);
        if(digitalRead(BTN_BACK)==0  ){
          delay(80);
          infilemenu= 0;
       }

        LcdBitmap(nosd4);
        delay(300);
        LcdBitmap(nosd5);
        if(digitalRead(BTN_BACK)==0  ){
          delay(80);
          infilemenu= 0;
       }

        delay(300);
        LcdBitmap(nosd6);
        if(digitalRead(BTN_BACK)==0  ){
          delay(80);
          infilemenu= 0;
       }

        
        return;
    }
    Serial.println("SD card initialized.");
    loadFiles();














    if (viewingFile) {
        handleFileView();
    } else {
        handleFileSelection();
    }
}

void loadFiles() {
    root = SD.open("/");
    fileCount = 0;
    while (true) {
        File entry = root.openNextFile();
        if (!entry) break;
        if (!entry.isDirectory() && fileCount < 10) {
            files[fileCount] = String(entry.name());
            Serial.println("Found: " + files[fileCount]);
            fileCount++;
        }
        entry.close();
    }
    root.close();
}

void handleFileSelection() {
    if (debounceButton(BTN_UP)) {
        currentFileIndex = (currentFileIndex > 0) ? currentFileIndex - 1 : fileCount - 1;
    } else if (debounceButton(BTN_DOWN)) {
        currentFileIndex = (currentFileIndex < fileCount - 1) ? currentFileIndex + 1 : 0;
    } else if (debounceButton(BTN_SELECT)) {
        openFile(files[currentFileIndex]);
        viewingFile = true;
    }
    displayMenu();
}

void displayMenu() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Select File:");
    for (int i = 0; i < fileCount; i++) {
        if (i == currentFileIndex) {
            display.fillRect(0, display.getCursorY(), 84, 8, BLACK);
            display.setTextColor(WHITE, BLACK);
        } else {
            display.setTextColor(BLACK, WHITE);
        }
        display.println(files[i]);
    }
    display.setTextColor(BLACK, WHITE);
    display.display();
}

void openFile(String filename) {
    File file = SD.open("/" + filename);
    if (!file) {
        
        LcdBitmap(nofiles);
        return;
    }
    Serial.println("Opening file: " + filename);
    lineCount = 0;
    while (file.available() && lineCount < 20) {
        fileLines[lineCount] = file.readStringUntil('\n');
        Serial.println(fileLines[lineCount]);
        lineCount++;
    }
    file.close();
    currentLineIndex = 0;
}

void handleFileView() {
    if (debounceButton(BTN_UP)) {
        currentLineIndex = (currentLineIndex > 0) ? currentLineIndex - 1 : lineCount - 1;
    } else if (debounceButton(BTN_DOWN)) {
        currentLineIndex = (currentLineIndex < lineCount - 1) ? currentLineIndex + 1 : 0;
    } else if (debounceButton(BTN_BACK)) {
        viewingFile = false;
        delay(50);
    }
    displayFileContent();
}

void displayFileContent() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("File Content:");
    for (int i = 0; i < 5; i++) {
        int index = currentLineIndex + i;
        if (index < lineCount) {
            if (i == 0) {
                display.fillRect(0, display.getCursorY(), 84, 8, BLACK);
                display.setTextColor(WHITE, BLACK);
            } else {
                display.setTextColor(BLACK, WHITE);
            }
            display.println(fileLines[index]);
        }
    }
    display.setTextColor(BLACK, WHITE);
    display.display();
}

void displayMessage(String msg) {
    display.clearDisplay();
    display.setCursor(10, 20);
    display.setTextSize(2);
    display.println(msg);
    display.display();
    delay(1000);
}

bool debounceButton(int buttonPin) {
    if (digitalRead(buttonPin) == LOW && (millis() - lastButtonPress) > 200) {
        lastButtonPress = millis();
        return true;
    }
    return false;
}
