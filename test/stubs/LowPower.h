#ifndef LowPower_h
#define LowPower_h

#include <gmock/gmock.h>

#define __AVR__
#define __AVR_ATmega328P__

enum period_t {	SLEEP_15MS,	SLEEP_FOREVER };

enum bod_t { BOD_OFF, BOD_ON };
enum adc_t { ADC_OFF, ADC_ON };

class LowPower_ {

  public:
    void powerDown(period_t, adc_t, bod_t);

};
extern LowPower_ LowPower;

#endif // LowPower_h
