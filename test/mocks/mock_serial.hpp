#ifndef MOCK_SERIAL_HPP_
#define MOCK_SERIAL_HPP_

#include "Serial.h"
// #include "../../include/Serial.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class SerialMock : public Serial {
  public:
    MOCK_METHOD1(write, size_t(const char*));
    MOCK_METHOD1(begin, int(int));
};

/**
 * Use this global pointer in your test, like so:
 * Serial_mock = new NiceMock<SerialMock>;
 * ...
 * delete serial_mock;
 */
::testing::NiceMock<SerialMock>* serial_mock;

Serial::Serial(){}
size_t Serial::write(const char* str) {
  return serial_mock->write(str);
}

int Serial::begin(int baut) {
  return serial_mock->begin(baut);
}

#endif // MOCK_SERIAL_HPP_