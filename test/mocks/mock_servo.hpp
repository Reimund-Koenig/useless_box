#ifndef MOCK_ARDUINO_HPP_
#define MOCK_ARDUINO_HPP_

#include "Servo.h"
#include"gmock/gmock.h"
#include "gtest/gtest.h"

class ServoMock : public Servo {
  public:
    MOCK_METHOD1(write, void(int));
};

/**
 * Use this global pointer in your test, like so:
 * servoMock = new NiceMock<ServoMock>;
 * ...
 * delete servoMock;
 */
::testing::NiceMock<ServoMock>* servoMock;

Servo::Servo(){}
uint8_t Servo::attach(int pin) { return 0; }
void Servo::write(int value) {
  return servoMock->write(value);
}
void Servo::writeMicroseconds(int value){}
int Servo::read() {  return  0; }
int Servo::readMicroseconds() { return 0; }
bool Servo::attached(){  return false ; }

#endif // MOCK_ARDUINO_HPP_