#include "pitches.h"

int buzzerPin = 5;

void setup() {
 
}
 
void loop() {
  tone(buzzerPin, NOTE_AS4, 300);
  delay(400);
  
  for (int i=0; i<3; i++) {
    tone(buzzerPin, NOTE_AS4, 100);
    delay(150);
  }

  tone(buzzerPin, NOTE_C5, 1000);

  delay(5000);
}
