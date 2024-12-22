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

ServoEasing bodyServo;

void blinkEye();

void setup() {
  ESP_LOGI(MAIN_TAG, "Setup!");

  ESP_LOGI(MAIN_TAG, "Setup Servo");
  bodyServo.setEasingType(EASE_QUADRATIC_IN);
  bodyServo.attach(PIN_SERVO1_TABLE, 90);

  ESP_LOGI(MAIN_TAG, "Setup Eye");
  ledcSetup(CH_EYE, 1000, 8);
  ledcAttachPin(PIN_EYE, CH_EYE);

  blinkEye();

  ESP_LOGI(MAIN_TAG, "Setup Legs");
  pinMode(PIN_MOTOR1, OUTPUT);
  pinMode(PIN_MOTOR2, OUTPUT);

  digitalWrite(PIN_MOTOR1, HIGH);
  digitalWrite(PIN_MOTOR2, LOW);


}

void loop() {
  bodyServo.easeTo(0, DEGREES_PER_SECOND);
  for(int i = 0; i < 2; i ++)
    blinkEye();

  bodyServo.easeTo(90, DEGREES_PER_SECOND);
  for(int i = 0; i < 3; i ++)
    blinkEye();

  bodyServo.easeTo(180, DEGREES_PER_SECOND);
  for(int i = 0; i < 2; i ++)
    blinkEye();

  bodyServo.easeTo(90, DEGREES_PER_SECOND);
  for(int i = 0; i < 3; i ++)
    blinkEye();
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