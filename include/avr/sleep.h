#ifndef avr_sleep_h
#define avr_sleep_h

namespace arduino
{
    extern "C" {
        void set_sleep_mode(int d);
        void sleep_enable();
        void sleep_cpu();
        void sleep_mode();
        void sleep_disable();
    }
}
#endif //avr_sleep_h