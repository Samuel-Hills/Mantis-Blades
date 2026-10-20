#include <Servo.h>

// Static dual-servo motion test.
// No MPU6050 is used. Both servos move through fixed, limited ranges.

namespace Config {
  constexpr uint8_t SERVO_ONE_PIN = 9;
  constexpr uint8_t SERVO_TWO_PIN = 10;

  // Approximate positions from the previous live test.
  constexpr int SERVO_ONE_START = 153;
  constexpr int SERVO_TWO_START = 30;

  // Servo 1 travels exactly 60 degrees: 153 -> 93.
  constexpr int SERVO_ONE_END = 93;

  // Servo 2 travels exactly 90 degrees: 30 -> 120.
  constexpr int SERVO_TWO_END = 120;

  constexpr unsigned long SWEEP_TIME_MS = 10000;
  constexpr unsigned long UPDATE_INTERVAL_MS = 20;
}

Servo servoOne;
Servo servoTwo;

unsigned long sweepStartTime = 0;
bool movingForward = true;

void writeInterpolatedPosition(float progress) {
  const int servoOneAngle = round(
    Config::SERVO_ONE_START +
    progress * (Config::SERVO_ONE_END - Config::SERVO_ONE_START)
  );

  const int servoTwoAngle = round(
    Config::SERVO_TWO_START +
    progress * (Config::SERVO_TWO_END - Config::SERVO_TWO_START)
  );

  servoOne.write(constrain(servoOneAngle, 0, 180));
  servoTwo.write(constrain(servoTwoAngle, 0, 180));
}

void setup() {
  Serial.begin(9600);

  servoOne.attach(Config::SERVO_ONE_PIN);
  servoTwo.attach(Config::SERVO_TWO_PIN);

  // Move to the known starting positions before beginning the slow sweep.
  writeInterpolatedPosition(0.0f);
  delay(500);

  sweepStartTime = millis();
  Serial.println(F("Static servo test started."));
}

void loop() {
  const unsigned long elapsed = millis() - sweepStartTime;
  const float rawProgress = min(
    1.0f,
    static_cast<float>(elapsed) / Config::SWEEP_TIME_MS
  );

  const float progress = movingForward
    ? rawProgress
    : 1.0f - rawProgress;

  writeInterpolatedPosition(progress);

  if (elapsed >= Config::SWEEP_TIME_MS) {
    movingForward = !movingForward;
    sweepStartTime = millis();
  }

  delay(Config::UPDATE_INTERVAL_MS);
}
