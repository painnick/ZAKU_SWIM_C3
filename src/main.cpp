#include <Arduino.h>

#define MAIN_TAG "Main"

// Servo
#define MAX_EASING_SERVOS 1
#define DISABLE_MICROS_AS_DEGREE_PARAMETER

#include <ServoEasing.hpp>

#include "common.h"

#define EYE_MINIMUM 12
#define EYE_MAXIMUM 221

#define DEGREES_PER_SECOND 30

#define MOTOR_SPEED_NORMAL 128

ServoEasing bodyServo;

int eyeBright = EYE_MINIMUM;

uint32_t motorSpeed = MOTOR_SPEED_NORMAL;

void blinkEye();

void motorStart();

void motorStop();

void updateMotorSpeed(uint32_t newMotorSpeed);

[[noreturn]] void taskSensor(__attribute__((unused)) void *params);

void setup() {
  ESP_LOGI(MAIN_TAG, "Setup!");

  ESP_LOGI(MAIN_TAG, "Setup Servo");
  bodyServo.setEasingType(EASE_QUADRATIC_IN);
  bodyServo.attach(PIN_SERVO1_TABLE, 90);

  ESP_LOGI(MAIN_TAG, "Setup Eye");
  ledcSetup(CH_EYE, 5000, 8);
  ledcAttachPin(PIN_EYE, CH_EYE);

  blinkEye();

  ESP_LOGI(MAIN_TAG, "Setup Legs");
  ledcSetup(CH_MOTOR1, 1000, 8);
  ledcAttachPin(PIN_MOTOR1, CH_MOTOR1);
  pinMode(PIN_MOTOR2, OUTPUT);

  digitalWrite(PIN_MOTOR2, LOW);
  motorStart();

  xTaskCreate(
      taskSensor,
      "taskSensor",
      10000,
      nullptr,
      1,
      nullptr);
}

int rest = 0;

void loop() {
  rest = random(3);

  bodyServo.easeTo(30, DEGREES_PER_SECOND);
  delay(2000);

  bodyServo.easeTo(90, DEGREES_PER_SECOND);
  if (rest == 1) {
    motorStop();
    for (int i = 0; i < 3; i++)
      blinkEye();
    motorStart();
  }

  bodyServo.easeTo(150, DEGREES_PER_SECOND);
  delay(2000);

  bodyServo.easeTo(90, DEGREES_PER_SECOND);
  if (rest == 2) {
    motorStop();
    for (int i = 0; i < 3; i++)
      blinkEye();
    motorStart();
  }
}

bool isBlink = false;

void blinkEye() {
  isBlink = true;
  for (int i = EYE_MINIMUM; i < EYE_MAXIMUM; i += 36) {
    ledcWrite(CH_EYE, i);
    delay(100);
  }
  ledcWrite(CH_EYE, EYE_MAXIMUM);
  delay(600);
  for (int i = EYE_MAXIMUM; i > EYE_MINIMUM; i -= 48) {
    ledcWrite(CH_EYE, i);
    delay(100);
  }
  delay(300);
  ledcWrite(CH_EYE, eyeBright);
  isBlink = false;
}

void motorStart() {
  ledcWrite(CH_MOTOR1, 255);
  delay(200);
  ledcWrite(CH_MOTOR1, motorSpeed);
}

void motorStop() {
  ledcWrite(CH_MOTOR1, 0);
}

void updateMotorSpeed(uint32_t newMotorSpeed) {
  motorSpeed = newMotorSpeed;
  ledcWrite(CH_MOTOR1, motorSpeed);
}

void taskSensor(__attribute__((unused)) void *params) {
  while (true) {
    auto volts = analogRead(PIN_SENSOR1) * 0.0008056640625;
    auto cm = 29.988 * pow(volts, -1.173);

    if (cm < 15) {
      updateMotorSpeed(256);
      eyeBright = EYE_MAXIMUM - 50;
      if (!isBlink)
        ledcWrite(CH_EYE, eyeBright);
    } else if (cm < 30) {
      updateMotorSpeed(MOTOR_SPEED_NORMAL + 96);
      eyeBright = EYE_MAXIMUM - 100;
      if (!isBlink)
        ledcWrite(CH_EYE, eyeBright);
    } else if (cm < 50) {
      updateMotorSpeed(MOTOR_SPEED_NORMAL + 64);
      eyeBright = EYE_MAXIMUM - 150;
      if (!isBlink)
        ledcWrite(CH_EYE, eyeBright);
    } else if (cm < 80) {
      updateMotorSpeed(MOTOR_SPEED_NORMAL + 32);
      eyeBright = EYE_MAXIMUM - 190;
      if (!isBlink)
        ledcWrite(CH_EYE, eyeBright);
    } else {
      updateMotorSpeed(MOTOR_SPEED_NORMAL);
      eyeBright = EYE_MINIMUM;
      if (!isBlink)
        ledcWrite(CH_EYE, eyeBright);
    }

    delay(100);
  }
}