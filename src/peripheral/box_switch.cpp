#include "ardunio_namespace.h" // needed for arduino build
#include "peripheral/box_switch.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;


box::Switch::Switch(int pin_switch) {
    box::Switch::pin_switch = pin_switch;
    box::Switch::box_wait_switch_debounce = new box::Wait();
    box::Switch::debounce_delay_ms = 10;
    box::Switch::last_debounce_time = 0;
    pinMode(pin_switch, INPUT_PULLUP);
    switch_state = digitalRead(box::Switch::pin_switch);
    last_switch_state = switch_state;
    m_has_changed = false;
}

box::Switch::~Switch() {
}

bool box::Switch::check() {
    bool reading = (bool) digitalRead(box::Switch::pin_switch);
    if (reading != last_switch_state) {
        last_debounce_time = millis();
    }
    if ((millis() - last_debounce_time) > debounce_delay_ms) {
        if (reading != switch_state) {
            switch_state = (bool) reading;
            m_has_changed = true;
        }
    }
    last_switch_state = (bool) reading;
    return switch_state;
}
/*************************************************************************************************
 * Public Methods
 *************************************************/

bool box::Switch::is_high() {
    return check();
}

bool box::Switch::has_changed() {
    check();
    if(m_has_changed) {
        m_has_changed = false;
        return true;
    }
    return false;
}
