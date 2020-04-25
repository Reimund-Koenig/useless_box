#ifndef MOCK_AVR_SLEEP_HPP_
#define MOCK_AVR_SLEEP_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <avr/sleep.h>

class SleepMock {
  public:
    MOCK_METHOD1(set_sleep_mode, void(int));
    MOCK_METHOD0(sleep_enable, void());
    MOCK_METHOD0(sleep_mode, void());
    MOCK_METHOD0(sleep_disable, void());
    MOCK_METHOD0(sleep_cpu, void());
};

/**
 * Use this global pointer in your test, like so:
 * arduino_mock = new NiceMock<SleepMock>;
 * ...
 * delete sleep_mock;
 */
::testing::NiceMock<SleepMock>* sleep_mock;

namespace arduino {

extern "C" {
  void set_sleep_mode(int d) {
    return sleep_mock->set_sleep_mode(d);
  }
  void sleep_cpu()  {
    return sleep_mock->sleep_cpu();
  }
  void sleep_enable()  {
    return sleep_mock->sleep_cpu();
  }
  void sleep_mode()  {
    return sleep_mock->sleep_cpu();
  }
  void sleep_disable()  {
    return sleep_mock->sleep_cpu();
  }
}
} // namespace arduino

#endif // MOCK_AVR_SLEEP_HPP_
