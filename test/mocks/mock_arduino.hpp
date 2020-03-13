#ifndef MOCK_ARDUINO_HPP_
#define MOCK_ARDUINO_HPP_

#include "../../include/Arduino.h"
#include"gmock/gmock.h"
#include "gtest/gtest.h"

class ArduinoMock {
  public:
    MOCK_METHOD2(analogWrite, void(int, int));
    MOCK_METHOD2(digitalWrite, void(int, int));
    MOCK_METHOD2(pinMode, void(int, int));
};

/**
 * Use this global pointer in your test, like so:
 * arduinoMock = new NiceMock<ArduinoMock>;
 * ...
 * delete arduinoMock;
 */
::testing::NiceMock<ArduinoMock>* arduinoMock;

extern "C" {
  void analogWrite(int pin, int state) {
    return arduinoMock->analogWrite(pin, state);
  }
  void digitalWrite(int pin, int state) {
    return arduinoMock->digitalWrite(pin, state);
  }
  void pinMode(int pin, int mode)  {
    return arduinoMock->pinMode(pin, mode);
  }
}
#endif // MOCK_ARDUINO_HPP_