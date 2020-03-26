#ifndef MOCK_ARDUINO_HPP_
#define MOCK_ARDUINO_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <Arduino.h>

class ArduinoMock {
  public:
    MOCK_METHOD1(random, long(long));
    MOCK_METHOD2(digitalWrite, void(int, int));
    MOCK_METHOD1(digitalRead, int(int));
    MOCK_METHOD2(analogWrite, void(int, int));
    MOCK_METHOD1(analogRead, int(int));
    MOCK_METHOD1(delay, void(int));
    MOCK_METHOD1(delayMicroseconds, void(int));
    MOCK_METHOD1(randomSeed, void(unsigned int));
    MOCK_METHOD0(micros, unsigned long());
    MOCK_METHOD0(millis, unsigned long());
    MOCK_METHOD3(attachInterrupt, void(int, void (*)(), int));
    MOCK_METHOD1(detachInterrupt, void(int));
    MOCK_METHOD3(pulseIn, unsigned long(int pin, int state, unsigned long timeout));
    MOCK_METHOD5(map, long(long, long, long, long, long));
    MOCK_METHOD2(pinMode, void(int, int));
};

/**
 * Use this global pointer in your test, like so:
 * arduino_mock = new NiceMock<ArduinoMock>;
 * ...
 * delete arduino_mock;
 */
::testing::NiceMock<ArduinoMock>* arduino_mock;

extern "C" {
  // long random(long range) {
  //   return arduino_mock->random(range);
  // }
  long map(long input, long min_input, long max_input, long min_output, long max_output) {
    return arduino_mock->map(input, min_input, max_input, min_output, max_output);
  }
  void pinMode(int pin, int mode)  {
    return arduino_mock->pinMode(pin, mode);
  }
  void digitalWrite(int pin, int state) {
    return arduino_mock->digitalWrite(pin, state);
  }
  int digitalRead(int pin) {
    return arduino_mock->digitalRead(pin);
  }
  void analogWrite(int pin, int state) {
    return arduino_mock->analogWrite(pin, state);
  }
  int analogRead(int pin) {
    return arduino_mock->analogRead(pin);
  }
  void delay(int time_miliseconds) {
    return arduino_mock->delay(time_miliseconds);
  }
  void delayMicroseconds(int time_microseconds) {
    return arduino_mock->delayMicroseconds(time_microseconds);
  }
  void randomSeed(unsigned int pin) {
    return arduino_mock->randomSeed(pin);
  }
  unsigned long micros() {
    return arduino_mock->micros();
  }
  unsigned long millis() {
    return arduino_mock->millis();
  }
  void attachInterrupt(int pin, void (*cb) (), int mode) {
    return arduino_mock->attachInterrupt(pin, cb, mode);
  }
  void detachInterrupt(int pin) {
    return arduino_mock->detachInterrupt(pin);
  }
  unsigned long pulseIn(int pin, int state, unsigned long timeout) {
    return arduino_mock->pulseIn(pin, state, timeout);
  }
}

#endif // MOCK_ARDUINO_HPP_