#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <DHT.h>
#include <Adafruit_BMP280.h> // For barometric pressure sensor

// Wi-Fi credentials
const char* ssid = "gradproject";
const char* password = "12345678";

// Web server
WebServer server(80);

// MPU6050 I2C address
const int MPU6050_ADDR = 0x68;

// DHT11 pin
#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// GPS
#define GPSTX 16
#define GPSRX 17
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

// MQ sensors pins
const int mq2Pin = 34;
const int mq7Pin = 35;
const int mq8Pin = 36;
const int mq135Pin = 37;

// Barometric pressure sensor
Adafruit_BMP280 bmp; // BMP280 connected to I2C (SCL = 22, SDA = 21)

// LDR pin
#define LDR_PIN 6

// Rain sensor pin
#define RAIN_PIN 7

// Function prototypes
void handleRoot();
void readMPU6050(float &ax, float &ay, float &az);
void readGPS(float &lat, float &lon, float &alt, float &speed, int &satellites);
void readDHT(float &temp, float &humidity);
void readMQ(int &mq2, int &mq7, int &mq8, int &mq135);
void readBarometricPressure(float &pressure);
String readLDRState();
String readRainState();

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize I2C for MPU6050 and BMP280
  Wire.begin(21, 22); // SDA, SCL
  if (!bmp.begin(0x76)) { // BMP280 I2C address (0x76 or 0x77)
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }

  // Initialize DHT11
  dht.begin();

  // Initialize GPS
  gpsSerial.begin(9600, SERIAL_8N1, GPSRX, GPSTX);

  // Initialize LDR and Rain Sensor pins
  pinMode(LDR_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);

  // Start Wi-Fi in AP mode
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Variables to store sensor data
  float ax, ay, az; // MPU6050 accelerometer data
  float lat, lon, alt, speed; // GPS data
  int satellites; // GPS satellites
  float temp, humidity; // DHT11 data
  int mq2, mq7, mq8, mq135; // MQ sensors data
  float pressure; // Barometric pressure data
  String ldrState = readLDRState(); // LDR state
  String rainState = readRainState(); // Rain sensor state

  // Read sensor data
  readMPU6050(ax, ay, az);
  readGPS(lat, lon, alt, speed, satellites);
  readDHT(temp, humidity);
  readMQ(mq2, mq7, mq8, mq135);
  readBarometricPressure(pressure);

  // Create HTML response with modern CSS styling and auto-refresh
  String html = R"(
    <html>
    <head>
      <title>Grad Project Sensor Data</title>
      <meta http-equiv="refresh" content="5"> <!-- Auto-refresh every 5 seconds -->
      <style>
        body {
          font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
          background-color: #f4f4f9;
          margin: 0;
          padding: 20px;
        }
        h1 {
          color: #2c3e50;
          text-align: center;
          margin-bottom: 20px;
        }
        table {
          width: 100%;
          border-collapse: collapse;
          margin-bottom: 20px;
          box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
          border-radius: 10px;
          overflow: hidden;
        }
        th, td {
          padding: 12px;
          text-align: left;
        }
        th {
          background: linear-gradient(135deg, #6a11cb, #2575fc);
          color: white;
          font-weight: bold;
        }
        tr:nth-child(even) {
          background-color: #f9f9f9;
        }
        tr:nth-child(odd) {
          background-color: #ffffff;
        }
        tr:hover {
          background-color: #f1f1f1;
        }
        .status {
          font-weight: bold;
        }
        .dark {
          color: #e74c3c;
        }
        .light {
          color: #2ecc71;
        }
        .dry {
          color: #3498db;
        }
        .wet {
          color: #e67e22;
        }
      </style>
    </head>
    <body>
      <h1>Grad Project Sensor Data</h1>
      <table>
        <tr>
          <th>Sensor</th>
          <th>Parameter</th>
          <th>Value</th>
        </tr>
        <tr>
          <td rowspan="3">MPU6050</td>
          <td>X</td>
          <td>)" + String(ax) + R"(</td>
        </tr>
        <tr>
          <td>Y</td>
          <td>)" + String(ay) + R"(</td>
        </tr>
        <tr>
          <td>Z</td>
          <td>)" + String(az) + R"(</td>
        </tr>
        <tr>
          <td rowspan="5">GPS</td>
          <td>Latitude</td>
          <td>)" + String(lat, 6) + R"(</td>
        </tr>
        <tr>
          <td>Longitude</td>
          <td>)" + String(lon, 6) + R"(</td>
        </tr>
        <tr>
          <td>Altitude</td>
          <td>)" + String(alt) + R"( m</td>
        </tr>
        <tr>
          <td>Speed</td>
          <td>)" + String(speed) + R"( km/h</td>
        </tr>
        <tr>
          <td>Satellites</td>
          <td>)" + String(satellites) + R"(</td>
        </tr>
        <tr>
          <td rowspan="2">DHT11</td>
          <td>Temperature</td>
          <td>)" + String(temp) + R"( °C</td>
        </tr>
        <tr>
          <td>Humidity</td>
          <td>)" + String(humidity) + R"( %</td>
        </tr>
        <tr>
          <td rowspan="4">MQ Sensors</td>
          <td>MQ2</td>
          <td>)" + String(mq2) + R"(</td>
        </tr>
        <tr>
          <td>MQ7</td>
          <td>)" + String(mq7) + R"(</td>
        </tr>
        <tr>
          <td>MQ8</td>
          <td>)" + String(mq8) + R"(</td>
        </tr>
        <tr>
          <td>MQ135</td>
          <td>)" + String(mq135) + R"(</td>
        </tr>
        <tr>
          <td>Barometric Pressure</td>
          <td>Pressure</td>
          <td>)" + String(pressure) + R"( hPa</td>
        </tr>
        <tr>
          <td>LDR</td>
          <td>State</td>
          <td><span class='status )" + (ldrState == "Dark" ? "dark" : "light") + R"('>)" + ldrState + R"(</span></td>
        </tr>
        <tr>
          <td>Rain Sensor</td>
          <td>State</td>
          <td><span class='status )" + (rainState == "Dry" ? "dry" : "wet") + R"('>)" + rainState + R"(</span></td>
        </tr>
      </table>
    </body>
    </html>
  )";

  // Send HTML response
  server.send(200, "text/html", html);
}

void readMPU6050(float &ax, float &ay, float &az) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true); // Read 6 registers total, each axis value is stored in 2 registers

  ax = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  ay = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  az = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
}

void readGPS(float &lat, float &lon, float &alt, float &speed, int &satellites) {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid()) {
    lat = gps.location.lat();
    lon = gps.location.lng();
    alt = gps.altitude.meters();
    speed = gps.speed.kmph();
    satellites = gps.satellites.value();
  } else {
    lat = lon = alt = speed = 0.0;
    satellites = 0;
  }
}

void readDHT(float &temp, float &humidity) {
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
}

void readMQ(int &mq2, int &mq7, int &mq8, int &mq135) {
  mq2 = analogRead(mq2Pin);
  mq7 = analogRead(mq7Pin);
  mq8 = analogRead(mq8Pin);
  mq135 = analogRead(mq135Pin);
}

void readBarometricPressure(float &pressure) {
  pressure = bmp.readPressure() / 100.0F; // Convert Pa to hPa
}

String readLDRState() {
  int ldrValue = digitalRead(LDR_PIN);
  return (ldrValue == HIGH) ? "Light" : "Dark";
}

String readRainState() {
  int rainValue = digitalRead(RAIN_PIN);
  return (rainValue == HIGH) ? "Dry" : "Wet";
}
