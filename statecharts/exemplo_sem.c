#include <Arduino.h>

const int buttonPin = 2;

const int ledPin = 13;
const int redCarPin = 3;
const int yellowCarPin = 4;
const int greenCarPin = 5;
const int redPedPin = 6;
const int greenPedPin = 7;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(redCarPin, OUTPUT);
  pinMode(greenCarPin, OUTPUT);
  pinMode(yellowCarPin, OUTPUT);
  pinMode(redPedPin, OUTPUT);
  pinMode(greenPedPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {    
    digitalWrite(ledPin, HIGH); // turn LED on
    digitalWrite(redCarPin, HIGH); // turn red car light off
    digitalWrite(greenCarPin, HIGH); // turn green car light on
    digitalWrite(yellowCarPin, HIGH); // turn yellow car light off
    digitalWrite(redPedPin, HIGH); // turn red pedestrian light off
    digitalWrite(greenPedPin, HIGH); // turn green pedestrian light on
  } else {
    digitalWrite(ledPin, LOW);  // turn LED off:
    digitalWrite(redCarPin, LOW); // turn red car light on
    digitalWrite(greenCarPin, LOW); // turn green car light off
    digitalWrite(yellowCarPin, LOW); // turn yellow car light on
    digitalWrite(redPedPin, LOW); // turn red pedestrian light on
    digitalWrite(greenPedPin, LOW); // turn green pedestrian light off
  }
}
