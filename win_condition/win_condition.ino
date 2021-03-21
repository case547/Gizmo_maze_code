#include "pitches.h"

int ledPin = 13;
int finish = 51;

int buzzerPin = 5;
bool played;

void setup() { 
  pinMode(ledPin, OUTPUT);
  pinMode(finish, INPUT);
  digitalWrite(finish, HIGH);

  played = false;
} 

void loop() {  
  int digitalVal = digitalRead(finish);
  
  if(digitalVal == LOW && played == false) {
    tone(buzzerPin, NOTE_AS4, 300);
    delay(400);

    for (int i=0; i<3; i++) {
      tone(buzzerPin, NOTE_AS4, 100);
      delay(150);
    }
    
    tone(buzzerPin, NOTE_C5, 1000);
    delay(10);

    played = true;
  }
}
