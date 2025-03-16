#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// HSPI Configuration for SD Card
#define HSPI_CLK   14
#define HSPI_MISO  12  
#define HSPI_MOSI  13 
#define HSPI_CS    27 
SPIClass hspi(HSPI);

// Nokia 5110 display pins
#define RST 2
#define CE  15
#define DC  22
#define DIN 23
#define CLK 18

// Button pins
#define BTN_UP    32 
#define BTN_DOWN  26
#define BTN_SELECT 25
#define BTN_BACK   33

// Initialize display
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CE, RST);

File root;
String files[10]; // Store up to 10 file names
int fileCount = 0;
int currentFileIndex = 0;
int currentLineIndex = 0;
String fileLines[20]; // Store up to 20 lines per file
int lineCount = 0;
bool viewingFile = false;
unsigned long lastButtonPress = 0;

void setup() {
    Serial.begin(115200);
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_BACK, INPUT_PULLUP);
    
    // Initialize display
    display.begin();
    display.setContrast(60);
    display.clearDisplay();
    display.display();
    
    // Initialize HSPI for SD card
    hspi.begin(HSPI_CLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);
    if (!SD.begin(HSPI_CS, hspi)) {
        Serial.println("SD card initialization failed!");
        displayMessage("SD Fail");
        return;
    }
    Serial.println("SD card initialized.");
    loadFiles();
}

void loop() {
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
        displayMessage("File Err");
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
