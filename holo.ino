#define LCD_X     84
#define LCD_Y     48

// Pin definitions
#define PIN_RESET 8
#define PIN_SCE   5
#define PIN_DC    4
#define PIN_SDIN  7
#define PIN_SCLK  6

#define BUTTON_UP 16
#define BUTTON_DOWN 17
#define BUTTON_SELECT 18

// LCD Command/Data Mode
#define LCD_C 0  // Command
#define LCD_D 1  // Data

// Bitmap images (84x48 pixel example images; replace these with actual image data)
const char image1[] = { };
const char image2[] = { };
const char image3[] = { };
const char image4[] = { };
const char image5[] = { };
const char image6[] = { };




// Array of images
const char* images[] = {image1, image2, image3 , image4, image5, image6};
int totalImages = sizeof(images) / sizeof(images[0]);
int currentImage = 0;

void setup() {
  // Initialize display and buttons
  LcdInitialise();
  LcdClear();
  
  pinMode(BUTTON_UP, INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN, INPUT_PULLDOWN);
  pinMode(BUTTON_SELECT, INPUT_PULLDOWN);

  // Display the first image
  displayImage(currentImage);
}

void loop() {
  // Check if the UP button is pressed
  if (digitalRead(BUTTON_UP) == HIGH) {
    currentImage = (currentImage - 1 + totalImages) % totalImages;  // Previous image
    displayImage(currentImage);
    delay(200);  // Debounce delay
  }

  // Check if the DOWN button is pressed
  if (digitalRead(BUTTON_DOWN) == HIGH) {
    currentImage = (currentImage + 1) % totalImages;  // Next image
    displayImage(currentImage);
    delay(200);  // Debounce delay
  }

  // Check if the SELECT button is pressed
  if (digitalRead(BUTTON_SELECT) == HIGH) {
    // Optional: Add functionality for SELECT button

    Serial.print("Selected image: ");
    Serial.println(currentImage);
    delay(200);  // Debounce delay
  }
}

// Display the specified image
void displayImage(int index) {
  LcdClear();
  gotoXY(0, 0);           // Ensure image starts from the top-left corner
  LcdBitmap(images[index]);
}

// LCD Procedures  
void LcdClear(void) {
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++) {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void) {
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite(LCD_C, 0x21);  // LCD Extended Commands
  LcdWrite(LCD_C, 0xB1);  // Set LCD Vop (Contrast)
  LcdWrite(LCD_C, 0x04);  // Set Temp coefficient
  LcdWrite(LCD_C, 0x14);  // LCD bias mode 1:48
  LcdWrite(LCD_C, 0x0C);  // LCD in normal mode
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void gotoXY(int x, int y) {
  LcdWrite(LCD_C, 0x80 | x);  // Column
  LcdWrite(LCD_C, 0x40 | y);  // Row
}

void LcdWrite(byte dc, byte data) {
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

void LcdBitmap(const char my_array[]) {
  for (int index = 0; index < (LCD_X * LCD_Y / 8); index++) {
    LcdWrite(LCD_D, my_array[index]);
  }
}
