#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SD.h>
#include <SPI.h>

// شاشة نوكيا 5110
#define RST 2
#define CE  15
#define DC  0
#define DIN 23
#define CLK 18

// أزرار
#define BTN_UP     33
#define BTN_DOWN   32
#define BTN_SELECT 26
#define BTN_BACK   25

// HSPI Configuration for SD Card
#define HSPI_CLK   14
#define HSPI_MISO  12  
#define HSPI_MOSI  13 
#define HSPI_CS    27 

SPIClass hspi(HSPI);
Adafruit_PCD8544 display(CLK, DIN, DC, CE, RST);

// الأيقونات
const uint8_t ir[] PROGMEM = {
  0x3F, 0x00, 0x40, 0x80, 0x9E, 0x40, 0x21, 0x00, 0x0C, 0x00,
  0x00, 0x00, 0x0C, 0x00, 0x1A, 0x00, 0x1E, 0x00, 0xFF, 0xC0
};

const uint8_t subghz[] PROGMEM = {
  0x40, 0x80, 0x92, 0x40, 0xA1, 0x40, 0xAD, 0x40, 0xAD, 0x40,
  0x80, 0x40, 0x4C, 0x80, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00
};

const uint8_t nfc[] PROGMEM = {
  0x01, 0x00, 0x00, 0x80, 0x44, 0x40, 0xc2, 0x40, 0xa2, 0x40,
  0x92, 0x40, 0x8c, 0x40, 0x44, 0x40, 0x00, 0x80, 0x01, 0x00
};

const uint8_t fileIcon[] PROGMEM = {
  0xfe, 0x00, 0x85, 0x00, 0xb4, 0x80, 0x87, 0x80, 0xb0, 0x80,
  0x80, 0x80, 0xbe, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff, 0x80
};

// الحالة
enum State { MENU, FILE_VIEW };
State currentState = MENU;

// الملفات
#define MAX_FILES 50
String fileNames[MAX_FILES];
int fileCount = 0;
int selectedItem = 0;
int topItem = 0;
String currentFileContent = "";

const uint8_t* getIconForFile(String name) {
  name.toLowerCase();
  if (name.endsWith(".ir")) return ir;
  if (name.endsWith(".nfc")) return nfc;
  if (name.endsWith(".sub") || name.endsWith(".subghz")) return subghz;
  return fileIcon;
}

void listFilesFromSD() {
  fileCount = 0;
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry || fileCount >= MAX_FILES) break;
    if (!entry.isDirectory()) {
      fileNames[fileCount++] = String(entry.name());
    }
    entry.close();
  }
  root.close();
}

void drawMenu() {
  display.clearDisplay();
  int visibleItems = min(5, fileCount - topItem);
  for (int i = 0; i < visibleItems; i++) {
    int index = topItem + i;
    int y = i * 12;

    if (index == selectedItem) {
      display.fillRect(0, y, 84, 12, BLACK);
      display.drawBitmap(2, y + 1, getIconForFile(fileNames[index]), 10, 10, WHITE);
      display.setTextColor(WHITE);
    } else {
      display.drawBitmap(2, y + 1, getIconForFile(fileNames[index]), 10, 10, BLACK);
      display.setTextColor(BLACK);
    }

    display.setCursor(14, y + 2);
    display.print(fileNames[index]);
  }
  display.display();
}

void drawFileContent() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(BLACK);
  display.print(currentFileContent);
  display.display();
}

void openFile(String name) {
  File f = SD.open("/" + name);
  currentFileContent = "";
  if (f) {
    while (f.available()) {
      currentFileContent += (char)f.read();
    }
    f.close();
  } else {
    currentFileContent = "Error opening file";
  }
  currentState = FILE_VIEW;
  drawFileContent();
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  display.begin();
  display.setContrast(50);

  hspi.begin(HSPI_CLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);
  if (!SD.begin(HSPI_CS, hspi)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("SD Fail");
    display.display();
    while (1);
  }

  listFilesFromSD();
  drawMenu();
}

void loop() {
  static unsigned long lastBtn = 0;
  unsigned long now = millis();

  if (now - lastBtn > 200) {
    if (currentState == MENU) {
      if (digitalRead(BTN_UP) == LOW) {
        if (selectedItem > 0) selectedItem--;
        if (selectedItem < topItem) topItem--;
        drawMenu();
        lastBtn = now;
      }

      if (digitalRead(BTN_DOWN) == LOW) {
        if (selectedItem < fileCount - 1) selectedItem++;
        if (selectedItem > topItem + 4) topItem++;
        drawMenu();
        lastBtn = now;
      }

      if (digitalRead(BTN_SELECT) == LOW) {
        openFile(fileNames[selectedItem]);
        lastBtn = now;
      }
    } else if (currentState == FILE_VIEW) {
      if (digitalRead(BTN_BACK) == LOW) {
        currentState = MENU;
        drawMenu();
        lastBtn = now;
      }
    }
  }
}
