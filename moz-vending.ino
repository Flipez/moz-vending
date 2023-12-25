#include <AccelStepper.h>

// ULN2003 Motor Driver Pins
const int IN1 = 5; // D1
const int IN2 = 4; // D2
const int IN3 = 14; // D5
const int IN4 = 12; // D6

// Pushbutton Pin
const int BUTTON = 13; // D7

// Motor Settings
const int MAXSPEED     = 1000;
const int ACCELERATION = 600;

// State Definitions
const int IDLE      = 1;
const int MOVING    = 2;
const int WAITING   = 3;
const int RETURNING = 4;

// Definition for homing
const int HOMEROTATION = 0;

// Steps to rotate per chamber (with 12 chambers)
const int chamberSize = 342;

// Define variables for rotation target and state
int rotationTarget = 0;
int state = IDLE;

// initialize the stepper library
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(BUTTON, HIGH); //activate arduino internal pull up

  // initialize the serial port
  Serial.begin(115200);
  
  // set the speed and acceleration
  stepper.setMaxSpeed(MAXSPEED);
  stepper.setAcceleration(ACCELERATION);
}

void loop() {
  switch(state) {
    case WAITING:
      delay(5000);
      Serial.println("Switching to RETURNING");
      state = RETURNING;
      stepper.moveTo(HOMEROTATION);
      break;
    case RETURNING:
      if (stepper.distanceToGo() == 0) {
        Serial.println("Switching to IDLE");
        state = IDLE;
      }
      stepper.run();
      break;
    case MOVING:
      if (stepper.distanceToGo() == 0){
        Serial.println("Switching to WAITING");
        state = WAITING;
      }
      stepper.run();
      break;
    case IDLE:
      if (digitalRead(BUTTON) == LOW ){
        Serial.println("Switching to MOVING");
        state = MOVING;
        rotationTarget =  chamberSize * random(1, 11);
        stepper.moveTo(rotationTarget);
      }
      break;
  }
}