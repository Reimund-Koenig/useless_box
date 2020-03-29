#include "ardunio_namespace.h" // needed for arduino build
#include "box_switch.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;


box::Switch::Switch(int pin_switch) {
    box::Switch::pin_switch = pin_switch;
    pinMode(pin_switch, INPUT_PULLUP);
    last_state = digitalRead(box::Switch::pin_switch);
    m_has_changed = is_high();
}

box::Switch::~Switch() {
}

bool box::Switch::check() {
    bool current_state = digitalRead(box::Switch::pin_switch);
    if(last_state != current_state && !m_has_changed) {
        m_has_changed = true;
    }
    last_state = current_state;
    return current_state;
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
