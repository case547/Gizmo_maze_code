#include <LiquidCrystal.h>

// Initialise library with interface pin numbers
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

char *levels[] = {"Easy", "Hard"};
int difficulty = 0; // pick 'easy' as default level

bool lvl_selected = false;

unsigned int durations[] = {60000, 30000};
int start_time;

// Define joystick control pins for each axis
int SW_pin = 2;
int XAxlePin = A0;
int YAxlePin = A1;
// Set start values for joystick
int XAxleValue = 0; 
int YAxleValue = 0;

int range = 12;           // output range of X or Y movement
int centre = range/2;     // resting position value
int threshold = range/4;  // resting threshold

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  digitalWrite(SW_pin, HIGH);
}

void loop() {
  while (lvl_selected == false) {
    lcd.setCursor(0, 0);
    lcd.print("Select Level:");
    
    lcd.setCursor(6, 1);
    lcd.print(levels[difficulty]);
  
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
      lcd.clear();
      start_time = millis();  // begin timer
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(levels[difficulty]);

  lcd.setCursor(0, 1);
  lcd.print("Time left:     ");
  lcd.setCursor(11, 1);
  
  long int time_left = start_time + durations[difficulty] - millis();
  
  if (time_left > 0) {
    lcd.print(round(time_left/1000));
    delay(10);
  } else {
    lcd.print("0");
    lcd.clear();
    delay(1000);
    lvl_selected = false;
  }
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
