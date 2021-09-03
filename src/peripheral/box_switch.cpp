#include "ardunio_namespace.h" // needed for arduino build
#include "peripheral/box_switch.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;


box::Switch::Switch(int pin_switch) {
    box::Switch::pin_switch = pin_switch;
    box::Switch::box_wait_switch_debounce = new box::Wait();
    box::Switch::debounce_delay_ms = 15;
    box::Switch::last_debounce_time = 0;
    pinMode(pin_switch, INPUT_PULLUP);
    switch_state = (bool) digitalRead(box::Switch::pin_switch);
    last_switch_state = switch_state;
    switch_state_has_changed = false;
}

box::Switch::~Switch() {
}

bool box::Switch::check() {
    bool current_switch_state = (bool) digitalRead(box::Switch::pin_switch);
    if (current_switch_state != last_switch_state) {
        last_debounce_time = millis();
    }
    if ((millis() - last_debounce_time) > debounce_delay_ms) {
        if (current_switch_state != switch_state) {
            switch_state = current_switch_state;
            switch_state_has_changed = true;
        }
    }
    last_switch_state = current_switch_state;
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
    if(switch_state_has_changed) {
        switch_state_has_changed = false;
        return true;
    }
    return false;
}
