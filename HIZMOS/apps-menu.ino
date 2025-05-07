void gpsfunction(){
  // Read GPS data
   if (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayGPSData();
    }
  }

  // If no valid data for 5 seconds, show warning
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("No GPS data");
    display.println("Check wiring!");
    display.display();
    
    while(true);
  }
}

void displayGPSData() {
  display.clearDisplay();
  display.setCursor(0, 0);

  // Display latitude
  display.print("Lat: ");
  if (gps.location.isValid()) {
    display.println(gps.location.lat(), 6);
  } else {
    display.println("Invalid");
  }

  // Display longitude
  display.print("Lon: ");
  if (gps.location.isValid()) {
    display.println(gps.location.lng(), 6);
  } else {
    display.println("Invalid");
  }

  // Display speed
  display.print("Speed:");
  if (gps.speed.isValid()) {
    display.print(gps.speed.kmph());
    display.println("km/h");
  } else {
    display.println("Invalid");
  }

  // Display satellites
  display.print("Sats: ");
  if (gps.satellites.isValid()) {
    display.println(gps.satellites.value());
  } else {
    display.println("0");
  }

  display.display();
}
