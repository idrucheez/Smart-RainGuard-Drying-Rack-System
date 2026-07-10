/*
  Smart Rain Cover System

  Hardware:
  - Arduino Uno
  - YL-83 rain sensor module using digital output
  - BTS7960 / IBT-2 motor driver
  - JGB37-3530 12 V, 111 RPM DC gear motor
  - LM2596 regulated 5 V logic supply
  - 3S 18650 battery pack for motor power

  Pin mapping:
  RPWM     -> Arduino D5
  LPWM     -> Arduino D6
  Rain DO  -> Arduino D7

  BTS7960:
  VCC  -> regulated 5 V
  GND  -> common ground
  R_EN -> regulated 5 V
  L_EN -> regulated 5 V
  R_IS and L_IS -> not connected

  IMPORTANT:
  This version uses time-based travel. Install physical limit switches
  before using the mechanism unattended.
*/

const uint8_t RPWM_PIN = 5;
const uint8_t LPWM_PIN = 6;
const uint8_t RAIN_PIN = 7;

// Most YL-83 modules output LOW when wet.
// Change this to HIGH if your module behaves oppositely.
const uint8_t RAIN_DETECTED_STATE = LOW;

// Adjust after testing.
// This is the constant-speed period after the soft-start ramp.
const unsigned long RUN_TIME_MS = 5000;

// PWM settings: Arduino Uno PWM range is 0-255.
const uint8_t START_PWM = 80;
const uint8_t DRIVE_PWM = 200;
const uint8_t PWM_STEP = 5;
const unsigned long RAMP_STEP_DELAY_MS = 25;

// Sensor must stay in its new state for this duration before activation.
const unsigned long SENSOR_DEBOUNCE_MS = 500;

// Assumes the rack starts fully extended when the Arduino powers on.
// Change to true only when it physically starts retracted.
bool isRetracted = false;

uint8_t lastRawRainState = HIGH;
uint8_t stableRainState = HIGH;
unsigned long lastSensorChangeTime = 0;

void setup() {
  pinMode(RPWM_PIN, OUTPUT);
  pinMode(LPWM_PIN, OUTPUT);
  pinMode(RAIN_PIN, INPUT);

  stopMotor();

  Serial.begin(9600);
  Serial.println(F("Smart Rain Cover System started."));
  Serial.println(F("WARNING: Time-based positioning is active."));
}

void loop() {
  updateRainSensor();

  const bool rainDetected = (stableRainState == RAIN_DETECTED_STATE);

  if (rainDetected && !isRetracted) {
    Serial.println(F("Rain detected: retracting cover."));

    retractCover();
    isRetracted = true;

    Serial.println(F("Retraction cycle completed."));
  }
  else if (!rainDetected && isRetracted) {
    Serial.println(F("Sensor dry: extending cover."));

    extendCover();
    isRetracted = false;

    Serial.println(F("Extension cycle completed."));
  }
}

void updateRainSensor() {
  const uint8_t rawState = digitalRead(RAIN_PIN);

  if (rawState != lastRawRainState) {
    lastRawRainState = rawState;
    lastSensorChangeTime = millis();
  }

  if ((millis() - lastSensorChangeTime) >= SENSOR_DEBOUNCE_MS) {
    stableRainState = rawState;
  }
}

// Direction mapping selected from the tested hardware:
// Retraction uses LPWM and extension uses RPWM.
// If the mechanism moves in the wrong direction, swap these functions
// or swap the two motor wires at M+ and M-.

void retractCover() {
  softStartMotor(LPWM_PIN, RPWM_PIN);
  delay(RUN_TIME_MS);
  stopMotor();
}

void extendCover() {
  softStartMotor(RPWM_PIN, LPWM_PIN);
  delay(RUN_TIME_MS);
  stopMotor();
}

void softStartMotor(uint8_t activePwmPin, uint8_t inactivePwmPin) {
  analogWrite(inactivePwmPin, 0);

  // Starting above zero can help a loaded geared motor overcome static friction.
  for (int pwm = START_PWM; pwm <= DRIVE_PWM; pwm += PWM_STEP) {
    analogWrite(activePwmPin, pwm);
    delay(RAMP_STEP_DELAY_MS);
  }

  analogWrite(activePwmPin, DRIVE_PWM);
}

void stopMotor() {
  analogWrite(RPWM_PIN, 0);
  analogWrite(LPWM_PIN, 0);
}
