#include <Arduino.h>

const int trig_pin = 2;
const int echo_pin = 32;

long duration;
int distance;

void setup()
{
  Serial.begin(115200);

  // activates correlating pins on arduino board as input/output
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loop() {
  digitalWrite(trig_pin, LOW);        // Resets the trig_pin
  delayMicroseconds(2);               // Waits 2 microsenconds
  digitalWrite(trig_pin, HIGH);       // Sets the trig_pin state to HIGH
  delayMicroseconds(10);              // Waits 10 microseconds
  digitalWrite(trig_pin, LOW);        // Sets the trig_pin state to LOW
  duration = pulseIn(echo_pin, HIGH); // Reads the echoPin and assign it to the variable duration
  distance = duration*0.034/2;        // Calculation of the distance in cm
  
  Serial.println(distance);

  delay(100);
}