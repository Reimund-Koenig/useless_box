#ifndef MOCK_AVR_SLEEP_H_
#define MOCK_AVR_SLEEP_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "avr/sleep.h"

class AvrSleepMock{
  public:
    MOCK_METHOD1(set_sleep_mode, void(int));
    MOCK_METHOD0(sleep_enable, void());
    MOCK_METHOD0(sleep_cpu, void());
    MOCK_METHOD0(sleep_disable, void());
};

/**
 * Use this global pointer in your test, like so:
 * arduino_mock = new NiceMock<AvrSleepMock>;
 * ...
 * delete mock_avr_sleep;
 */
::testing::NiceMock<AvrSleepMock>* mock_avr_sleep;

namespace arduino {
  extern "C" {
    void set_sleep_mode(int x) {
      return mock_avr_sleep->set_sleep_mode(x);
    }

    void sleep_enable() {
      return mock_avr_sleep->sleep_enable();
    }

    void sleep_cpu() {
      return mock_avr_sleep->sleep_cpu();
    }

    void sleep_disable() {
      return mock_avr_sleep->sleep_disable();
    }
  } // extern "C"
} // namespace arduino

#endif // MOCK_AVR_SLEEP_H_
