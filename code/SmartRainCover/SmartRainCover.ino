/*
  Project Title: Smart RainGuard Drying Rack System

  Description:
  This project automatically protects clothes from rain by using a YL-83
  rain sensor to detect water. When rain is detected, an Arduino Uno sends
  PWM control signals to a BTS7960 motor driver, which drives a JGB37-3530
  12 V DC geared motor to retract the rain cover.

  When the rain sensor becomes dry, the motor rotates in the opposite
  direction to extend the cover back to its original position.

  The motor uses a soft-start PWM routine to reduce sudden current surges,
  mechanical shock, and voltage drops during startup. The cover position is
  estimated using motor runtime and a software state variable.

  Hardware Used:
  - Arduino Uno
  - YL-83 rain sensor module
  - BTS7960 motor driver
  - JGB37-3530 DC geared motor, 12 V, 111 RPM
  - LM2596 DC-DC buck converter
  - 3 × 18650 rechargeable battery cells in series
  - String-and-spool rain-cover mechanism

  Pin Connections:
  - BTS7960 RPWM -> Arduino pin 5
  - BTS7960 LPWM -> Arduino pin 6
  - YL-83 DO     -> Arduino pin 7
  - BTS7960 VCC  -> Regulated 5 V
  - BTS7960 GND  -> Common ground
  - BTS7960 R_EN -> 5 V
  - BTS7960 L_EN -> 5 V

  Important Notes:
  - The motor is powered directly from the 3S battery pack through the
    BTS7960 motor driver.
  - The Arduino, rain sensor, and BTS7960 logic side are powered using
    regulated 5 V from the LM2596 buck converter.
  - All circuit grounds must be connected together.
  - The current version uses time-based positioning. Limit switches are
    recommended for improved safety and position accuracy.

  Author: Idriss Salim
  Platform: Arduino IDE
*/

const int RPWM = 5;
const int LPWM = 6;
const int rainPin = 7;

const int runTime = 7000; // Motor running time in milliseconds

bool isRetracted = false; // Tracks the assumed rack position

void setup() {
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(rainPin, INPUT);

  stopMotor();

  Serial.begin(9600);
  Serial.println("Smart Rain Cover System Started");
}

void loop() {
  int rainState = digitalRead(rainPin);

  // Rain detected and rack is currently extended
  if (rainState == LOW && !isRetracted) {
    Serial.println("Rain detected - Retracting");

    retractCover();
    delay(runTime);
    stopMotor();

    isRetracted = true;
  }

  // No rain and rack is currently retracted
  else if (rainState == HIGH && isRetracted) {
    Serial.println("No rain - Extending");

    extendCover();
    delay(runTime);
    stopMotor();

    isRetracted = false;
  }
}

// Retracts the cover using LPWM
void retractCover() {
  analogWrite(RPWM, 0);

  for (int speed = 0; speed <= 200; speed += 5) {
    analogWrite(LPWM, speed);
    delay(30);
  }
}

// Extends the cover using RPWM
void extendCover() {
  analogWrite(LPWM, 0);

  for (int speed = 0; speed <= 200; speed += 5) {
    analogWrite(RPWM, speed);
    delay(30);
  }
}

// Stops the DC motor
void stopMotor() {
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
}
