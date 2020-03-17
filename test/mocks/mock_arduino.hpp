#ifndef MOCK_ARDUINO_HPP_
#define MOCK_ARDUINO_HPP_

#include "../../include/Arduino.h"
#include"gmock/gmock.h"
#include "gtest/gtest.h"

class ArduinoMock {
  public:
    MOCK_METHOD2(pinMode, void(int, int));
    MOCK_METHOD2(digitalWrite, void(int, int));
    MOCK_METHOD1(digitalRead, int(int));
    MOCK_METHOD2(analogWrite, void(int, int));
    MOCK_METHOD1(analogRead, int(int));
    MOCK_METHOD1(delay, void(int));
    MOCK_METHOD1(delayMicroseconds, void(int));
    // MOCK_METHOD1(random, long(long));
    MOCK_METHOD1(randomSeed, void(unsigned int));
    MOCK_METHOD0(micros, unsigned long());
    MOCK_METHOD3(attachInterrupt, void(int, void (*)(), int));
};

/**
 * Use this global pointer in your test, like so:
 * arduinoMock = new NiceMock<ArduinoMock>;
 * ...
 * delete arduinoMock;
 */
::testing::NiceMock<ArduinoMock>* arduinoMock;

extern "C" {
  void pinMode(int pin, int mode)  {
    return arduinoMock->pinMode(pin, mode);
  }
  void digitalWrite(int pin, int state) {
    return arduinoMock->digitalWrite(pin, state);
  }
  int digitalRead(int pin) {
    return arduinoMock->digitalRead(pin);
  }
  void analogWrite(int pin, int state) {
    return arduinoMock->analogWrite(pin, state);
  }
  int analogRead(int pin) {
    return arduinoMock->analogRead(pin);
  }
  void delay(int time_miliseconds) {
    return arduinoMock->delay(time_miliseconds);
  }
  void delayMicroseconds(int time_microseconds) {
    return arduinoMock->delayMicroseconds(time_microseconds);
  }
  // long random(long range) {
  //   return arduinoMock->random(range);
  // }
  void randomSeed(unsigned int pin) {
    return arduinoMock->randomSeed(pin);
  }
  unsigned long micros() {
    return arduinoMock->micros();
  }
  void attachInterrupt(int pin, void (*cb) (), int mode) {
    return arduinoMock->attachInterrupt(pin, cb, mode);
  }
}

#endif // MOCK_ARDUINO_HPP_