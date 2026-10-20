#include <Servo.h>
#include <stdio.h>
#include <ctype.h>

// Servo calibration tool.
// Use the Serial Monitor to command each servo to a known angle and find
// the numeric angles that match the desired physical home position.

constexpr uint8_t SERVO_ONE_PIN = 9;
constexpr uint8_t SERVO_TWO_PIN = 10;
constexpr int STARTUP_ANGLE = 135;
constexpr int MAX_TEST_ANGLE = 270;
constexpr int SERVO_MIN_PULSE_US = 500;
constexpr int SERVO_MAX_PULSE_US = 2500;

Servo servoOne;
Servo servoTwo;

int servoOneAngle = STARTUP_ANGLE;
int servoTwoAngle = STARTUP_ANGLE;
bool servosAttached = false;

int clampAngle(int angle) {
  return constrain(angle, 0, MAX_TEST_ANGLE);
}

void writeAngle(Servo& servo, int angle) {
  const int pulseWidth = map(
    angle,
    0,
    MAX_TEST_ANGLE,
    SERVO_MIN_PULSE_US,
    SERVO_MAX_PULSE_US
  );

  servo.writeMicroseconds(pulseWidth);
}

void printState() {
  Serial.print(F("Servo 1 angle: "));
  Serial.println(servoOneAngle);
  Serial.print(F("Servo 2 angle: "));
  Serial.println(servoTwoAngle);
}

void printHelp() {
  Serial.println(F("Commands:"));
  Serial.println(F("  1 210   move servo 1 to 210 degrees"));
  Serial.println(F("  2 90    move servo 2 to 90 degrees"));
  Serial.println(F("  b 210 90  move both servos"));
  Serial.println(F("  d       detach both servos for manual movement"));
  Serial.println(F("  a       attach both servos at their last commanded angles"));
  Serial.println(F("  ?       print current commanded angles"));
  Serial.println(F("Use Newline or Both NL & CR as the line ending."));
}

void attachServos() {
  if (!servosAttached) {
    servoOne.attach(SERVO_ONE_PIN);
    servoTwo.attach(SERVO_TWO_PIN);
    servosAttached = true;
  }

  writeAngle(servoOne, servoOneAngle);
  writeAngle(servoTwo, servoTwoAngle);
}

void setServoOne(int angle) {
  servoOneAngle = clampAngle(angle);
  attachServos();
  writeAngle(servoOne, servoOneAngle);
  printState();
}

void setServoTwo(int angle) {
  servoTwoAngle = clampAngle(angle);
  attachServos();
  writeAngle(servoTwo, servoTwoAngle);
  printState();
}

void setBoth(int firstAngle, int secondAngle) {
  servoOneAngle = clampAngle(firstAngle);
  servoTwoAngle = clampAngle(secondAngle);
  attachServos();
  writeAngle(servoOne, servoOneAngle);
  writeAngle(servoTwo, servoTwoAngle);
  printState();
}

void handleCommand(String command) {
  command.trim();

  if (command.length() == 0) {
    return;
  }

  const char commandType = tolower(command.charAt(0));

  if (commandType == '1') {
    setServoOne(command.substring(1).toInt());
  } else if (commandType == '2') {
    setServoTwo(command.substring(1).toInt());
  } else if (commandType == 'b') {
    int firstAngle = 0;
    int secondAngle = 0;

    if (sscanf(command.c_str(), "b %d %d", &firstAngle, &secondAngle) == 2) {
      setBoth(firstAngle, secondAngle);
    } else {
      Serial.println(F("Use: b 120 45"));
    }
  } else if (commandType == 'd') {
    servoOne.detach();
    servoTwo.detach();
    servosAttached = false;
    Serial.println(F("Servos detached; move them manually if needed."));
  } else if (commandType == 'a') {
    attachServos();
    Serial.println(F("Servos attached at their last commanded angles."));
    printState();
  } else if (commandType == '?') {
    printState();
  } else {
    printHelp();
  }
}

void setup() {
  Serial.begin(9600);
  attachServos();

  Serial.println(F("Servo calibration tool ready."));
  Serial.println(F("Initial command is 135 degrees for both servos."));
  Serial.println(F("Test range is 0-270 degrees using 500-2500 microseconds."));
  printHelp();
  printState();
}

void loop() {
  if (Serial.available() > 0) {
    handleCommand(Serial.readStringUntil('\n'));
  }
}
