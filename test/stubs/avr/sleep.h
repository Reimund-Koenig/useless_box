#ifndef _AVR_SLEEP_H_
#define _AVR_SLEEP_H_ 1

namespace arduino {
    extern "C" {
        void set_sleep_mode(int);
        void sleep_enable();
        void sleep_cpu();
        void sleep_disable();
    } // extern "C"
} // namespace arduino
#endif /* _AVR_SLEEP_H_ */
