#ifndef MOCK_LOW_POWER_
#define MOCK_LOW_POWER_
#define __AVR__
#define __AVR_ATmega328P__

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "LowPower.h"

class LowPowerMock : public LowPower_ {
  public:
    MOCK_METHOD3(powerDown, void(period_t, adc_t, bod_t));
};

/**
 * Use this global pointer in your test, like so:
 * arduino_mock = new NiceMock<LowPowerMock>;
 * ...
 * delete low_power_mock;
 */
::testing::NiceMock<LowPowerMock>* low_power_mock;

void LowPower_::powerDown(period_t period, adc_t adc, bod_t bod) {
  return low_power_mock->powerDown(period, adc, bod);
}

LowPower_ LowPower;

#endif // MOCK_LOW_POWER_
