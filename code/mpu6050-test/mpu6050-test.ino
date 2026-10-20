#include <Wire.h>
#include <MPU6050.h>

// Standalone MPU6050 test sketch.
// This sketch does not attach, power, or move any servos.

MPU6050 sensor;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  delay(100);
  sensor.initialize();

  if (!sensor.testConnection()) {
    Serial.println(F("MPU6050 connection failed."));
    Serial.println(F("Check VCC, GND, SDA -> A4, and SCL -> A5."));

    while (true) {
      delay(1000);
    }
  }

  Serial.println(F("MPU6050 connected."));
  Serial.println(F("ax,ay,az,tilt_degrees"));
}

void loop() {
  int16_t accelerationX;
  int16_t accelerationY;
  int16_t accelerationZ;

  sensor.getAcceleration(
    &accelerationX,
    &accelerationY,
    &accelerationZ
  );

  const float tiltDegrees = atan2(
    static_cast<float>(accelerationX),
    static_cast<float>(accelerationZ)
  ) * 180.0 / PI;

  Serial.print(accelerationX);
  Serial.print(',');
  Serial.print(accelerationY);
  Serial.print(',');
  Serial.print(accelerationZ);
  Serial.print(',');
  Serial.println(tiltDegrees, 2);

  delay(200);
}
