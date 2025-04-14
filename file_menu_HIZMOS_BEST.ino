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

// SD Card على HSPI
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

const uint8_t badusb[] PROGMEM = {
  0x00, 0x00, 0x1f, 0x00, 0x3f, 0x80, 0x2e, 0x80, 0x24, 0x80,
  0x3f, 0x80, 0x3f, 0x80, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t folder[] PROGMEM = {
  0x00, 0x00, 0xfc, 0x00, 0x82, 0x00, 0xff, 0xc0, 0x80, 0x40,
  0x80, 0x40, 0x80, 0x40, 0x80, 0x40, 0x7f, 0x80, 0x00, 0x00
};

// الحالة
enum State { MENU, FILE_VIEW };
State currentState = MENU;

#define MAX_FILES 50
String fileNames[MAX_FILES];
int fileCount = 0;
int selectedItem = 0;
int topItem = 0;
String currentFileContent = "";

const uint8_t* getIconForFile(String name) {
  name.toLowerCase();

  if (name.endsWith("/")) return folder; // ← فولدر
  if (name.endsWith(".bad")) return badusb;
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

    String name = String(entry.name());

    // تخطي system volume information
    if (name.equalsIgnoreCase("System Volume Information")) {
      entry.close();
      continue;
    }

    if (entry.isDirectory()) {
      name += "/"; // ← نضيف / في آخر اسم الفولدر
    }

    fileNames[fileCount++] = name;
    entry.close();
  }
  root.close();
}


void drawScrollDots() {
  int totalDots = fileCount;
  int maxDots = 48;
  float step = (float)maxDots / totalDots;

  for (int i = 0; i < totalDots; i++) {
    int y = i * step;

    if (i == selectedItem) {
      display.fillRect(81, y, 3, step > 5 ? 5 : step, BLACK);
    } else {
      display.drawPixel(83, y + 1, BLACK);
    }
  }
}

void drawMenu() {
  display.clearDisplay();
  int visibleItems = min(5, fileCount - topItem);
  for (int i = 0; i < visibleItems; i++) {
    int index = topItem + i;
    int y = i * 12;

    String displayName = fileNames[index];

    // إزالة الامتداد من العرض
    int dotIndex = displayName.lastIndexOf('.');
    if (dotIndex > 0 && !displayName.endsWith("/")) {
      displayName = displayName.substring(0, dotIndex);
    }

    if (index == selectedItem) {
      display.fillRoundRect(0, y, 80, 12, 2, 1);
      display.drawBitmap(2, y + 1, getIconForFile(fileNames[index]), 10, 10, WHITE);
      display.setTextColor(WHITE);
    } else {
      display.drawBitmap(2, y + 1, getIconForFile(fileNames[index]), 10, 10, BLACK);
      display.setTextColor(BLACK);
    }

    display.setCursor(14, y + 2);
    display.print(displayName);
  }

  drawScrollDots();
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

