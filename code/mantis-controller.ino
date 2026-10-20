#include <Wire.h>
#include <Servo.h>
#include <MPU6050.h>

// Tilt-controlled dual-servo controller for the Mantis-Blades project.
// This sketch focuses on tunability, clearer behavior, and maintainable structure.

namespace Config {
  constexpr uint8_t PRIMARY_SERVO_PIN = 9;
  constexpr uint8_t SECONDARY_SERVO_PIN = 10;

  constexpr int PRIMARY_START_ANGLE = 170;
  constexpr int SECONDARY_START_ANGLE = 30;

  constexpr int PRIMARY_MIN_ANGLE = 60;
  constexpr int PRIMARY_MAX_ANGLE = 170;

  constexpr int SAFE_MIN_ANGLE = 30;
  constexpr int SAFE_MAX_ANGLE = 170;

  // Ignore small changes to reduce unnecessary movement and servo jitter.
  constexpr int MOVEMENT_DEADBAND = 3;

  // Controls accelerometer smoothing. Higher values produce smoother movement.
  constexpr float FILTER_WEIGHT = 0.90f;

  // Increases how strongly the servos respond to changes in tilt.
  constexpr float TILT_GAIN = 2.50f;

  constexpr float MIN_TILT = -90.0f;
  constexpr float MAX_TILT = 90.0f;

  constexpr unsigned long SAMPLE_INTERVAL_MS = 20;
}

class TiltServoController {
 public:
  void begin() {
    Serial.begin(9600);
    Wire.begin();

    initializeSensor();
    initializeServos();
  }

  void update() {
    const unsigned long currentTime = millis();

    // Maintain a 20 ms update rate without blocking other Arduino tasks.
    if (currentTime - lastSampleTime_ < Config::SAMPLE_INTERVAL_MS) {
      return;
    }

    lastSampleTime_ = currentTime;

    const float measuredTilt = readTiltDegrees();
    filteredTilt_ = applyLowPassFilter(measuredTilt);

    const float adjustedTilt = constrain(
      filteredTilt_ * Config::TILT_GAIN,
      Config::MIN_TILT,
      Config::MAX_TILT
    );

    const int primaryTarget = calculatePrimaryAngle(adjustedTilt);
    const int secondaryTarget = calculateMirroredAngle(primaryTarget);

    updateServo(
      primaryServo_,
      primaryAngle_,
      primaryTarget,
      F("Primary servo")
    );

    updateServo(
      secondaryServo_,
      secondaryAngle_,
      secondaryTarget,
      F("Secondary servo")
    );
  }

 private:
  MPU6050 sensor_;

  Servo primaryServo_;
  Servo secondaryServo_;

  float filteredTilt_ = 0.0f;

  int primaryAngle_ = Config::PRIMARY_START_ANGLE;
  int secondaryAngle_ = Config::SECONDARY_START_ANGLE;

  unsigned long lastSampleTime_ = 0;

  void initializeSensor() {
    sensor_.initialize();

    if (!sensor_.testConnection()) {
      Serial.println(F("Error: MPU6050 connection failed."));

      while (true) {
        delay(1000);
      }
    }

    Serial.println(F("MPU6050 initialized successfully."));
  }

  void initializeServos() {
    primaryServo_.attach(Config::PRIMARY_SERVO_PIN);
    secondaryServo_.attach(Config::SECONDARY_SERVO_PIN);

    primaryServo_.write(primaryAngle_);
    secondaryServo_.write(secondaryAngle_);
  }

  float readTiltDegrees() {
    int16_t accelerationX;
    int16_t accelerationY;
    int16_t accelerationZ;

    sensor_.getAcceleration(
      &accelerationX,
      &accelerationY,
      &accelerationZ
    );

    // Calculate the sensor's tilt around the X-axis.
    return atan2f(
      static_cast<float>(accelerationX),
      static_cast<float>(accelerationZ)
    ) * RAD_TO_DEG;
  }

  float applyLowPassFilter(float measurement) const {
    // Smooth accelerometer noise to prevent sudden servo movement.
    return
      Config::FILTER_WEIGHT * filteredTilt_ +
      (1.0f - Config::FILTER_WEIGHT) * measurement;
  }

  int calculatePrimaryAngle(float tiltDegrees) const {
    const float normalizedTilt =
      (tiltDegrees - Config::MIN_TILT) /
      (Config::MAX_TILT - Config::MIN_TILT);

    const float angle =
      Config::PRIMARY_MIN_ANGLE +
      normalizedTilt *
      (Config::PRIMARY_MAX_ANGLE - Config::PRIMARY_MIN_ANGLE);

    return constrain(
      static_cast<int>(roundf(angle)),
      Config::SAFE_MIN_ANGLE,
      Config::SAFE_MAX_ANGLE
    );
  }

  int calculateMirroredAngle(int primaryAngle) const {
    // Move the second servo in the opposite direction.
    return constrain(
      180 - primaryAngle,
      Config::SAFE_MIN_ANGLE,
      Config::SAFE_MAX_ANGLE
    );
  }

  void updateServo(
    Servo& servo,
    int& currentAngle,
    int targetAngle,
    const __FlashStringHelper* servoName
  ) {
    // Ignore insignificant changes to reduce servo jitter.
    if (abs(targetAngle - currentAngle) <= Config::MOVEMENT_DEADBAND) {
      return;
    }

    currentAngle = targetAngle;
    servo.write(currentAngle);

    Serial.print(servoName);
    Serial.print(F(" angle: "));
    Serial.println(currentAngle);
  }
};

TiltServoController controller;

void setup() {
  controller.begin();
}

void loop() {
  controller.update();
}
