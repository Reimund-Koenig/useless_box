#ifndef MOCK_SERIAL_HPP_
#define MOCK_SERIAL_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Serial.h"

class SerialMock : public Serial_ {
  public:
    MOCK_METHOD1(print, size_t(int));
    MOCK_METHOD1(print, size_t(const char));
    MOCK_METHOD1(print, size_t(const char*));
    MOCK_METHOD1(println, size_t(const char*));
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

// size_t Serial_::print(int i) { return serial_mock->print(i); }
// size_t Serial_::print(const char c) { return serial_mock->print(c); }
// size_t Serial_::print(const char* str) { return serial_mock->write(str); }
// size_t Serial_::println(const char* str) { return serial_mock->println(str); }
// size_t Serial_::write(const char* str) { return serial_mock->write(str); }
int Serial_::begin(int baut) { return serial_mock->begin(baut); }

// Preinstantiate Objects
Serial_ Serial;

#endif // MOCK_SERIAL_HPP_