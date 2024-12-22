#include <Arduino.h>

#define MAIN_TAG "Main"

// Servo
#define MAX_EASING_SERVOS 1
#define DISABLE_MICROS_AS_DEGREE_PARAMETER

#include <ServoEasing.hpp>

#include "common.h"

#define EYE_NORMAL 23
#define EYE_BRIGHT 221

#define DEGREES_PER_SECOND 30

ServoEasing bodyServo;

void setup() {
  ESP_LOGI(MAIN_TAG, "Setup!");

  ESP_LOGI(MAIN_TAG, "Setup Servo");
  bodyServo.setEasingType(EASE_QUADRATIC_IN);
  bodyServo.attach(PIN_SERVO1_TABLE, 90);

  ESP_LOGI(MAIN_TAG, "Setup Eye");
  ledcSetup(CH_EYE, 1000, 8);
  ledcAttachPin(PIN_EYE, CH_EYE);
  ledcWrite(CH_EYE, EYE_NORMAL);

  ESP_LOGI(MAIN_TAG, "Setup Legs");
  pinMode(PIN_MOTOR1, OUTPUT);
  pinMode(PIN_MOTOR2, OUTPUT);

  digitalWrite(PIN_MOTOR1, HIGH);
  digitalWrite(PIN_MOTOR2, LOW);
}

void loop() {
  bodyServo.easeTo(0, DEGREES_PER_SECOND);
  delay(2000);

  bodyServo.easeTo(90, DEGREES_PER_SECOND);
  digitalWrite(PIN_MOTOR1, LOW);
  ledcWrite(CH_EYE, EYE_BRIGHT);
  delay(1000);
  ledcWrite(CH_EYE, EYE_NORMAL);
  digitalWrite(PIN_MOTOR1, HIGH);

  bodyServo.easeTo(180, DEGREES_PER_SECOND);
  delay(2000);

  bodyServo.easeTo(90, DEGREES_PER_SECOND);
  digitalWrite(PIN_MOTOR1, LOW);
  ledcWrite(CH_EYE, EYE_BRIGHT);
  delay(3000);
  ledcWrite(CH_EYE, EYE_NORMAL);
  digitalWrite(PIN_MOTOR1, HIGH);
}