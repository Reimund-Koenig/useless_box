#include "box_switch.hpp"
#include "Arduino.h"
#include <stdio.h>


box::Switch::Switch(int pin_switch) {
    box::Switch::pin_switch = pin_switch;
    pinMode(pin_switch, INPUT_PULLUP);
}

box::Switch::~Switch() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

int box::Switch::get_state() {
    return digitalRead(box::Switch::pin_switch);
}
