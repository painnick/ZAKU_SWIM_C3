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

#define MOTOR_SPEED 128

ServoEasing bodyServo;

void blinkEye();

void motorStart(uint32_t speed = MOTOR_SPEED);

void motorStop();

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

void blinkEye() {
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
}

void motorStart(uint32_t speed) {
  ledcWrite(CH_MOTOR1, 255);
  delay(200);
  ledcWrite(CH_MOTOR1, speed);
}

void motorStop() {
  ledcWrite(CH_MOTOR1, 0);
}
