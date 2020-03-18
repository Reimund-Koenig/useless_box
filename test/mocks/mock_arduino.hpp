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
    MOCK_METHOD0(millis, unsigned long());
    MOCK_METHOD3(attachInterrupt, void(int, void (*)(), int));
    MOCK_METHOD1(detachInterrupt, void(int));
    MOCK_METHOD3(pulseIn, unsigned long(int pin, int state, unsigned long timeout));
    MOCK_METHOD5(map, long(long, long, long, long, long));
};

/**
 * Use this global pointer in your test, like so:
 * arduinoMock = new NiceMock<ArduinoMock>;
 * ...
 * delete arduinoMock;
 */
::testing::NiceMock<ArduinoMock>* arduinoMock;

extern "C" {
  long map(long input, long min_input, long max_input, long min_output, long max_output) {
    return arduinoMock->map(input, min_input, max_input, min_output, max_output);
  }
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
  unsigned long millis() {
    return arduinoMock->millis();
  }
  void attachInterrupt(int pin, void (*cb) (), int mode) {
    return arduinoMock->attachInterrupt(pin, cb, mode);
  }
  void detachInterrupt(int pin) {
    return arduinoMock->detachInterrupt(pin);
  }
  unsigned long pulseIn(int pin, int state, unsigned long timeout) {
    return arduinoMock->pulseIn(pin, state, timeout);
  }
}

#endif // MOCK_ARDUINO_HPP_