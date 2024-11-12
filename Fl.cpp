#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <IRremote.h>
#include <RH_ASK.h>

// Pins and objects setup
#define RESET_PIN 8
#define CE_PIN 5
#define DC_PIN 4
#define DIN_PIN 7
#define CLK_PIN 6
Adafruit_PCD8544 display(DC_PIN, CE_PIN, DIN_PIN, CLK_PIN, RESET_PIN);

#define UP_BTN_PIN 28
#define SELECT_BTN_PIN 26
#define DOWN_BTN_PIN 27

#define IR_RECEIVER_PIN 15
#define IR_TRANSMITTER_PIN 14

#define RF_RECEIVER_PIN 12
#define RF_TRANSMITTER_PIN 13
RH_ASK rf_driver;

#define SD_CS_PIN 17

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);
IRrecv irrecv(IR_RECEIVER_PIN);
IRsend irsend(IR_TRANSMITTER_PIN);
decode_results irResults;

File sdFile;
String currentFile;
int menuOption = 0, subMenuOption = 0;
bool inSubMenu = false;

// Function prototypes
void displayMainMenu();
void displaySubMenu();
void handleNFCMenu();
void handleIRMenu();
void handleRFMenu();
void handleHIDMenu();
void readNFC();
void emulateNFC();
void saveNFC();
void readIR();
void emulateIR();
void saveIR();
void readRF();
void emulateRF();
void saveRF();
void runHID();

// Initialization
void setup() {
    display.begin();
    display.setContrast(50);
    display.clearDisplay();
    display.display();
  
    pinMode(UP_BTN_PIN, INPUT_PULLUP);
    pinMode(SELECT_BTN_PIN, INPUT_PULLUP);
    pinMode(DOWN_BTN_PIN, INPUT_PULLUP);
  
    if (!SD.begin(SD_CS_PIN)) {
        display.print("SD Error!");
        display.display();
        while (1);
    }
  
    Wire.begin();
    nfc.begin();
    irrecv.enableIRIn();
    rf_driver.init();
}

// Main loop
void loop() {
    if (inSubMenu) {
        displaySubMenu();
    } else {
        displayMainMenu();
    }
    handleInput();
}

// Display the main menu with highlighted option
void displayMainMenu() {
    display.clearDisplay();
    const char* options[] = {"NFC", "IR", "Sub GHz", "HID"};
    for (int i = 0; i < 4; i++) {
        if (i == menuOption) {
            display.setTextColor(BLACK, WHITE);
        } else {
            display.setTextColor(WHITE, BLACK);
        }
        display.setCursor(0, i * 10);
        display.print(options[i]);
    }
    display.display();
}

// Display the submenu for the selected option
void displaySubMenu() {
    display.clearDisplay();
    const char* nfcOptions[] = {"Read", "Emulate", "Save Tag", "Exit"};
    const char* irOptions[] = {"Read", "Emulate", "Save to SD", "Exit"};
    const char* rfOptions[] = {"Read", "Emulate", "Save to SD", "Exit"};
    const char* hidOptions[] = {"Run", "Exit"};
  
    const char** options;
    int optionCount = 0;
  
    if (menuOption == 0) { // NFC
        options = nfcOptions;
        optionCount = 4;
    } else if (menuOption == 1) { // IR
        options = irOptions;
        optionCount = 4;
    } else if (menuOption == 2) { // Sub GHz
        options = rfOptions;
        optionCount = 4;
    } else if (menuOption == 3) { // HID
        options = hidOptions;
        optionCount = 2;
    }
  
    for (int i = 0; i < optionCount; i++) {
        if (i == subMenuOption) {
            display.setTextColor(BLACK, WHITE);
        } else {
            display.setTextColor(WHITE, BLACK);
        }
        display.setCursor(0, i * 10);
        display.print(options[i]);
    }
    display.display();
}

// Handling button input for menu navigation
void handleInput() {
    if (digitalRead(UP_BTN_PIN) == LOW) {
        if (inSubMenu) {
            subMenuOption = (subMenuOption - 1 + 4) % 4;
        } else {
            menuOption = (menuOption - 1 + 4) % 4;
        }
    } else if (digitalRead(DOWN_BTN_PIN) == LOW) {
        if (inSubMenu) {
            subMenuOption = (subMenuOption + 1) % 4;
        } else {
            menuOption = (menuOption + 1) % 4;
        }
    } else if (digitalRead(SELECT_BTN_PIN) == LOW) {
        if (inSubMenu) {
            executeSubMenu();
        } else {
            inSubMenu = true;
            subMenuOption = 0;
        }
    }
}

// Execute submenu option based on main menu selection
void executeSubMenu() {
    if (menuOption == 0) { // NFC Menu
        handleNFCMenu();
    } else if (menuOption == 1) { // IR Menu
        handleIRMenu();
    } else if (menuOption == 2) { // Sub GHz Menu
        handleRFMenu();
    } else if (menuOption == 3) { // HID Menu
        handleHIDMenu();
    }
}

// Sample NFC, IR, RF, and HID handling functions
void handleNFCMenu() {
    if (subMenuOption == 0) readNFC();
    else if (subMenuOption == 1) emulateNFC();
    else if (subMenuOption == 2) saveNFC();
    else inSubMenu = false;
}

// Similar function structure for IR, RF, and HID menus ...

// NFC read function
void readNFC() {
    display.clearDisplay();
    display.print("Scanning...");
    display.display();
    // Add NFC scanning logic
    delay(2000); // Placeholder delay for demonstration
    display.clearDisplay();
    display.print("UID: ABC123"); // Replace with actual UID
    display.display();
    delay(2000);
    inSubMenu = false;
}

// Add functions for NFC emulate, save, IR read, emulate, save, RF read, emulate, save, and HID run here
