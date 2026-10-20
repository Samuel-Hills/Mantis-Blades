#include <Servo.h>

// Static dual-servo motion test with a manual five-second home phase.
// No MPU6050 is used.
//
// During the first five seconds, the servo signal outputs are detached so the
// mechanisms can be moved by hand. At the end, place them at the configured
// home positions. The sketch then sweeps between fixed limits.

namespace Config {
  constexpr uint8_t SERVO_ONE_PIN = 9;
  constexpr uint8_t SERVO_TWO_PIN = 10;

  // Physical home positions to set during the five-second phase.
  constexpr int SERVO_ONE_HOME = 153;
  constexpr int SERVO_TWO_HOME = 30;

  // Servo 1 travels exactly 60 degrees: 153 -> 93.
  constexpr int SERVO_ONE_END = 93;

  // Servo 2 travels exactly 90 degrees: 30 -> 120.
  constexpr int SERVO_TWO_END = 120;

  constexpr unsigned long PASSIVE_TIME_MS = 5000;
  constexpr unsigned long SWEEP_TIME_MS = 10000;
  constexpr unsigned long UPDATE_INTERVAL_MS = 20;
}

Servo servoOne;
Servo servoTwo;

unsigned long startupTime = 0;
bool motionStarted = false;
bool movingForward = true;
float progress = 0.0f;
int lastCountdownSecond = -1;

void writeInterpolatedPosition(float newProgress) {
  const int servoOneAngle = round(
    Config::SERVO_ONE_HOME +
    newProgress * (Config::SERVO_ONE_END - Config::SERVO_ONE_HOME)
  );

  const int servoTwoAngle = round(
    Config::SERVO_TWO_HOME +
    newProgress * (Config::SERVO_TWO_END - Config::SERVO_TWO_HOME)
  );

  servoOne.write(constrain(servoOneAngle, 0, 180));
  servoTwo.write(constrain(servoTwoAngle, 0, 180));
}

void startMotion() {
  servoOne.attach(Config::SERVO_ONE_PIN);
  servoTwo.attach(Config::SERVO_TWO_PIN);

  // The user should have manually placed both mechanisms at these positions.
  writeInterpolatedPosition(0.0f);

  progress = 0.0f;
  movingForward = true;
  motionStarted = true;

  Serial.println(F("Home phase complete. Motion started."));
}

void setup() {
  Serial.begin(9600);

  // Keep the outputs detached during the manual positioning phase.
  servoOne.detach();
  servoTwo.detach();

  startupTime = millis();
  Serial.println(F("5-second passive home phase started."));
  Serial.println(F("Move both mechanisms to their home positions now."));
}

void loop() {
  if (!motionStarted) {
    const unsigned long elapsed = millis() - startupTime;

    if (elapsed < Config::PASSIVE_TIME_MS) {
      const int remainingSecond = static_cast<int>(
        (Config::PASSIVE_TIME_MS - elapsed + 999) / 1000
      );

      if (remainingSecond != lastCountdownSecond) {
        Serial.print(F("Home phase: "));
        Serial.print(remainingSecond);
        Serial.println(F(" seconds remaining"));
        lastCountdownSecond = remainingSecond;
      }

      delay(Config::UPDATE_INTERVAL_MS);
      return;
    }

    startMotion();
    return;
  }

  writeInterpolatedPosition(progress);

  const float progressStep =
    static_cast<float>(Config::UPDATE_INTERVAL_MS) /
    static_cast<float>(Config::SWEEP_TIME_MS);

  progress += movingForward ? progressStep : -progressStep;

  if (progress >= 1.0f) {
    progress = 1.0f;
    movingForward = false;
  } else if (progress <= 0.0f) {
    progress = 0.0f;
    movingForward = true;
  }

  delay(Config::UPDATE_INTERVAL_MS);
}
