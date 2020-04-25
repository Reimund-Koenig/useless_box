#ifndef MOCK_AVR_SLEEP_HPP_
#define MOCK_AVR_SLEEP_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <avr/sleep.h>

class AvrSleepMock {
  public:
    MOCK_METHOD1(set_sleep_mode, void(int));
    MOCK_METHOD0(sleep_enable, void());
    MOCK_METHOD0(sleep_mode, void());
    MOCK_METHOD0(sleep_disable, void());
    MOCK_METHOD0(sleep_cpu, void());
};

/**
 * Use this global pointer in your test, like so:
 * arduino_mock = new NiceMock<AvrSleepMock>;
 * ...
 * delete avr_sleep_mock;
 */
::testing::NiceMock<AvrSleepMock>* avr_sleep_mock;

namespace arduino {

extern "C" {
  void set_sleep_mode(int d) {
    return avr_sleep_mock->set_sleep_mode(d);
  }
  void sleep_cpu()  {
    return avr_sleep_mock->sleep_cpu();
  }
  void sleep_enable()  {
    return avr_sleep_mock->sleep_enable();
  }
  void sleep_mode()  {
    return avr_sleep_mock->sleep_mode();
  }
  void sleep_disable()  {
    return avr_sleep_mock->sleep_disable();
  }
}
} // namespace arduino

#endif // MOCK_AVR_SLEEP_HPP_
