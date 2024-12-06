#include <esp_now.h>
#include <WiFi.h>

// Right motor
int enableRightMotor = 22;
int rightMotorPin1 = 16;
int rightMotorPin2 = 17;
// Left motor
int enableLeftMotor = 23;
int leftMotorPin1 = 18;
int leftMotorPin2 = 19;

// Ultrasonic sensors
#define FRONT_TRIG 3
#define FRONT_ECHO 2
#define RIGHT_TRIG 5
#define RIGHT_ECHO 4
#define LEFT_TRIG 7
#define LEFT_ECHO 6

#define MAX_MOTOR_SPEED 200
#define OBSTACLE_DISTANCE 20 // مسافة العائق بالسم

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

unsigned long lastRecvTime = 0;

struct PacketData
{
  byte xAxisValue;
  byte yAxisValue;
  byte switchPressed;
  bool automaticMode;
};
PacketData receiverData;

bool automaticMode = false;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  if (len == 0)
  {
    return;
  }
  memcpy(&receiverData, incomingData, sizeof(receiverData));
  automaticMode = receiverData.automaticMode;

  if (!automaticMode)
  {
    simpleMovements();
  }

  lastRecvTime = millis();
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  setUpPinModes();

  // Set ultrasonic pins
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
}

void loop()
{
  if (automaticMode)
  {
    avoidObstacles();
  }
  else
  {
    unsigned long now = millis();
    if (now - lastRecvTime > SIGNAL_TIMEOUT)
    {
      rotateMotor(0, 0);
    }
  }
}

void avoidObstacles()
{
  int frontDistance = measureDistance(FRONT_TRIG, FRONT_ECHO);
  int rightDistance = measureDistance(RIGHT_TRIG, RIGHT_ECHO);
  int leftDistance = measureDistance(LEFT_TRIG, LEFT_ECHO);

  if (frontDistance < OBSTACLE_DISTANCE)
  {
    if (leftDistance > rightDistance)
    {
      rotateMotor(MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED); // Turn left
    }
    else
    {
      rotateMotor(-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED); // Turn right
    }
  }
  else
  {
    rotateMotor(MAX_MOTOR_SPEED, MAX_MOTOR_SPEED); // Move forward
  }
}

int measureDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  int duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}
