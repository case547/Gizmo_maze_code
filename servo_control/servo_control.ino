#include <Servo.h>

// Define servo motors
Servo myServoX;
Servo myServoY;

// Define pins for each axis
int ServoXPin = 3;
int ServoYPin = 4;

// Set home position for servos
int ServoXHomePos = 90;
int ServoYHomePos = 90;

int ServoXPos;
int ServoYPos;

// Define joystick control pins for each axis
int XAxlePin = A0;
int YAxlePin = A1;

// Set start values for joystick
int XAxleValue = 0; 
int YAxleValue = 0;
  
int range = 12;           // output range of X or Y movement
int centre = range/2;     // resting position value
int threshold = range/4;  // resting threshold

int servoStep = 1;
int servoDelay = 100;

void setup() {
  myServoX.attach(ServoXPin); 
  myServoY.attach(ServoYPin);

  // Update servo positions at start-up
  ServoXPos = ServoXHomePos;
  ServoYPos = ServoYHomePos;
  
  myServoX.write(ServoXPos);
  myServoY.write(ServoYPos);
  
  Serial.begin(9600);
}

void loop() {
  XAxleValue = readAxis(XAxlePin);
  YAxleValue = readAxis(YAxlePin);
  
  Serial.print(XAxleValue, DEC);
  Serial.print(" - ");
  Serial.println(YAxleValue, DEC);
  
  // Check joystick vals and move servos
  if (XAxleValue > 0) {
    ServoXPos += servoStep;
    myServoX.write(ServoXPos);
    delay(servoDelay);
  }
  if (XAxleValue < 0) {
    ServoXPos -= servoStep;
    myServoX.write(ServoXPos);
    delay(servoDelay);
  }
  if (YAxleValue > 0) {
    ServoYPos += servoStep;
    myServoY.write(ServoYPos);
    delay(servoDelay);
  }
  if (YAxleValue < 0) {
    ServoYPos -= servoStep;
    myServoY.write(ServoYPos);
    delay(servoDelay);
  }

  int angleLim = 30;
  if (ServoXPos > ServoXHomePos + angleLim) {
     ServoXPos = ServoXHomePos + angleLim;
  }
  if (ServoXPos < ServoXHomePos - angleLim) {
    ServoXPos = ServoXHomePos - angleLim;
  }
  if (ServoYPos > ServoYHomePos + angleLim) {
    ServoYPos = ServoYHomePos + angleLim;
  }
  if (ServoYPos < ServoYHomePos - angleLim) {
    ServoYPos = ServoYHomePos - angleLim;
  }
  
  delay(10);
}

int readAxis(int thisAxis) {
  // Read analog input
  int reading = analogRead(thisAxis); 
  
  // Map reading from analog input to output range
  reading = map(reading, 0, 1023, 0, range);
  
  // If output reading outside of rest position threshold, use it:
  int distance = reading - centre;
  if (abs(distance) < threshold) {
    distance = 0;
  }
  
  return distance;  // for specified axis
}
