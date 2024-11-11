void nfcReadTag() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NFC Read Function");
  display.display();
  delay(2000);
}

void nfcEmulateTag() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NFC Emulate Function");
  display.display();
  delay(2000);
}

void nfcBruteForce() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NFC Brute Force");
  display.display();
  delay(2000);
}

void nfcSaveToSD() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("NFC Save to SD");
  display.display();
  delay(2000);
}
