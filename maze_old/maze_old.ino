#include <Servo.h>
#include <LiquidCrystal.h>
#include "pitches.h"


/***** SERVO MOTORS *****/

// Define servo motors
Servo myServoX;
Servo myServoY;

// Define pins for each axis
int ServoXPin = 3;
int ServoYPin = 4;

// Set home position for servos
int ServoXHomePos = 85;
int ServoYHomePos = 85;

int ServoXPos;
int ServoYPos;

int servoStep;

/***** JOYSTICK *****/

// Define joystick control pins for each axis
int SW_pin = 5;
int XAxlePin = A0;
int YAxlePin = A1;

// Set start values for joystick
int XAxleValue = 0; 
int YAxleValue = 0;
  
int range = 12;           // output range of joystick X or Y movement
int centre = range/2;     // resting position value
int threshold = range/4;  // resting threshold


/***** LCD *****/

// Initialise library with interface pin numbers
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

char *levels[] = {"Easy", "Hard"};
int difficulty = 0; // pick 'easy' as default level

bool lvl_selected;

unsigned int durations[] = {60000, 5000};  // Time limits for each level
unsigned int start_time;
unsigned int time_lim;


/***** GAME OVER PROTOCOL *****/

int finish = 51;      // finish 'line' (basically tilt ball sensor)
bool played = false;  // check if game has already been played


/***** BUZZER *****/

int buzzerPin = 12;
int fail_tone[] = {NOTE_AS3, NOTE_A3, NOTE_GS3};

int short_note = 500;
int long_note = 1500;


/***** SOUND SENSOR *****/

int soundAnalogPin = A2;
int soundDigiPin = 2;
int soundAnalogVal = 0;
int soundDigiVal;

#define RED 22
#define BLUE 23


void setup() {
  myServoX.attach(ServoXPin); 
  myServoY.attach(ServoYPin);

  // Update servo positions at start-up
  ServoXPos = ServoXHomePos;
  ServoYPos = ServoYHomePos;
  myServoX.write(ServoXPos);
  myServoY.write(ServoYPos);

  digitalWrite(SW_pin, HIGH); // Instantiate joystick button

  lcd.begin(16, 2); // Set up LCD

  // Set up finish protocols
  digitalWrite(finish, HIGH);
  lvl_selected = false;

  Serial.begin(9600);

  pinMode(soundDigiPin, INPUT);

  // LEDs for servo step indication
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
}

void loop() {
  /***** LEVEL SELECTION *****/
  while (lvl_selected == false) {  
    // Print level select message on LCD  
    lcd.setCursor(2, 0);
    lcd.print("Select Level");
    
    lcd.setCursor(1, 1);
    lcd.print("   > ");
    lcd.print(levels[difficulty]);
    lcd.print(" <   ");
  
    XAxleValue = readAxis(XAxlePin);
    YAxleValue = readAxis(YAxlePin);

    // Check if joystick is moved
    if (XAxleValue != 0 || YAxleValue != 0) {
      if (difficulty == 0) {
        difficulty = 1; // If current level is easy, set to hard
      } else {
        difficulty = 0; // If current level is hard, set to easy
      }
      
      delay(250);
    }

    // Check if the joystick is pressed down
    if (digitalRead(SW_pin) == 0) {
      lvl_selected = true;
      Serial.print("Level selected: ");
      Serial.println(levels[difficulty]);

      time_lim = durations[difficulty];
      Serial.print("Time limit: ");
      Serial.println(time_lim);
     
      lcd.clear();
      start_time = millis();  // begin timer
      Serial.print("Start time: ");
      Serial.println(start_time);
      Serial.print("\n");

      servoStep = 1;
      digitalWrite(BLUE, HIGH);
      Serial.print("Setting servoStep to: ");
      Serial.println(servoStep);
    }
  }

  // Poll sound sensor
  soundAnalogVal = analogRead(soundAnalogPin);
  soundDigiVal = digitalRead(soundDigiPin);

  // Change servoStep (and LEDs to match) if sound change detected
  if (soundDigiVal == 1) {
    if (servoStep == 1) {
      servoStep = 2; // LED purple
      digitalWrite(RED, HIGH);
      digitalWrite(BLUE, HIGH);
      Serial.print("Setting servoStep to: ");
      Serial.println(servoStep);
    } else if (servoStep == 2) {
      servoStep = 3;  // LED red
      digitalWrite(RED, HIGH);
      digitalWrite(BLUE, LOW);
      Serial.print("Setting servoStep to: ");
      Serial.println(servoStep);
    } else {
      servoStep = 1;  // LED blue
      digitalWrite(RED, LOW);
      digitalWrite(BLUE, HIGH);
      Serial.print("Setting servoStep to: ");
      Serial.println(servoStep);
    }
  }

  XAxleValue = readAxis(XAxlePin);
  YAxleValue = readAxis(YAxlePin);

  int servoDelay = round(100/servoStep);
  
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

  int angleLim = 24;  // limit servo to home position +/- angleLim
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

  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(levels[difficulty]);

  lcd.setCursor(0, 1);
  lcd.print("Time left:     ");
  lcd.setCursor(11, 1);

  unsigned int time_now = millis();
  signed long int time_left = start_time + time_lim - time_now;
  
//  Serial.print("Current time: ");
//  Serial.println(time_now);
//  Serial.print("Time left: ");
//  Serial.println(time_left);
//  Serial.print("\n");

  if (time_left > 200) {
    lcd.print(round(time_left/1000));
    delay(10);
  } else if (played == false) {
    // Loss protocol
    lcd.print("0");
    lcd.clear();

    delay(250);

    lcd.setCursor(4, 0);
    lcd.print("You lost");
    lcd.setCursor(7, 1);
    lcd.print(":(");

    Serial.println("Game over: defeat");
    played = true;

    // Sad trombone sound
    for (int thisNote = 0; thisNote < 3; thisNote++) {
      tone(buzzerPin, fail_tone[thisNote], short_note);
      delay(1000);
    }
    tone(buzzerPin, NOTE_G3, long_note);
    delay(long_note + 250);
  }

  // Win protocol
  int digitalVal = digitalRead(finish);

  if(digitalVal == LOW && played == false) {
    lcd.clear();
    delay(250);
    
    lcd.setCursor(4, 0);
    lcd.print("You won!");
    lcd.setCursor(7, 1);
    lcd.print(":)");

    Serial.println("Game over: victory");
    played = true;
    
    tone(buzzerPin, NOTE_AS4, 300);
    delay(400);
    
    for (int i=0; i<3; i++) {
      tone(buzzerPin, NOTE_AS4, 100);
      delay(150);
    }
    
    tone(buzzerPin, NOTE_C5, 1000);
    delay(1500);
  }

  // Final message
  while(played) {
    lcd.setCursor(3, 0);
    lcd.print("Thanks for");
    lcd.setCursor(4, 1);
    lcd.print("playing!");
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
