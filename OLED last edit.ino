#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Bitmap images (replace these with your actual bitmaps)
const unsigned char Image1[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image2[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image3[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image4[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image5[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image6[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image7[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image8[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image9[] PROGMEM = {
  /* Your bitmap data here */
};
const unsigned char Image10[] PROGMEM = {
  /* Your bitmap data here */
};

void setup() {
  // Initialize the display with the I2C address 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Clear the display buffer
  display.clearDisplay();
  display.display();
}

void loop() {
  // Display each image one by one with a delay
  displayImage(Image1);
  displayImage(Image2);
  displayImage(Image3);
  displayImage(Image4);
  displayImage(Image5);
  displayImage(Image6);
  displayImage(Image7);
  displayImage(Image8);
  displayImage(Image9);
  displayImage(Image10);
}

// Function to display an image
void displayImage(const unsigned char *image) {
  display.clearDisplay();
  display.drawBitmap(0, 0, image, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
  delay(800); // Delay between images
}
