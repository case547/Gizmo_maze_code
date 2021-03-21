int soundAnalogPin = A2;  // Select the Arduino input pin to accept the Sound Sensor's analog output
int soundDigiPin = 2;     // Select the Arduino input pin to accept the Sound Sensor's digital output
int soundAnalogVal = 0;   // Define variable to store the analog value coming from the Sound Sensor
int soundDigiVal;         // Define variable to store the digital value coming from the Sound Sensor

int servoStep = 1;

// Define LED pins
#define RED 22
#define BLUE 23

void setup() {
  Serial.begin(9600);           // The IDE settings for Serial Monitor/Plotter (preferred) must match this speed
  pinMode(soundDigiPin, INPUT); // Define pin as an input port, to accept digital input

  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(BLUE, HIGH);
}

void loop() {
  soundAnalogVal = analogRead(soundAnalogPin);  // Read the value of the analog interface assigned to digitalValue
  soundDigiVal = digitalRead(soundDigiPin);     // Read the value of the digital interface assigned to digitalValue
  Serial.print(soundAnalogVal);
  Serial.print(" - ");
  Serial.println(soundDigiVal);

  if (soundDigiVal == 1) {
    if (servoStep == 1) {
      servoStep = 2;
      digitalWrite(RED, HIGH);
      digitalWrite(BLUE, HIGH);
    } else if (servoStep == 2) {
      servoStep = 3;
      digitalWrite(RED, HIGH);
      digitalWrite(BLUE, LOW);
    } else {
      servoStep = 1;
      digitalWrite(RED, LOW);
      digitalWrite(BLUE, HIGH);
    }
  }

  delay(250);
}
