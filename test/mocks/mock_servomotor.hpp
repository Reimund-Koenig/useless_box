#ifndef MOCK_SERVOMOTORMOTOR_HPP_
#define MOCK_SERVOMOTORMOTOR_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <Servo.h>

class ServomotorMock : public Servo {
  public:
    MOCK_METHOD1(write, void(int));
    MOCK_METHOD0(read, int());
};

/**
 * Use this global pointer in your test, like so:
 * servomotor_mock = new NiceMock<ServomotorMock>;
 * ...
 * delete servomotor_mock;
 */
::testing::NiceMock<ServomotorMock>* servomotor_mock;

Servo::Servo(){}
uint8_t Servo::attach(int pin) { return 0; }
void Servo::write(int value) {
  return servomotor_mock->write(value);
}
int Servo::read() {
  return servomotor_mock->read();
}

void Servo::writeMicroseconds(int value){}
int Servo::readMicroseconds() { return 0; }
bool Servo::attached(){  return false ; }

#endif // MOCK_SERVOMOTORMOTOR_HPP_