#include "pitches.h"

int buzzerPin = 5;

int fail_tone[] = {NOTE_AS3, NOTE_A3, NOTE_GS3};

int short_note = 500;
int long_note = 1500;
 
void setup() {
 
}
 
void loop() {
  for (int thisNote = 0; thisNote < 3; thisNote++) {
    tone(buzzerPin, fail_tone[thisNote], short_note);
    delay(1000);
  }

  tone(buzzerPin, NOTE_G3, long_note);
   
  delay(5000);
}
