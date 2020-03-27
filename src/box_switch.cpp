#include "ardunio_namespace.h" // needed for arduino build
#include "box_switch.hpp"
#include <Arduino.h>
#include <stdio.h>
using namespace arduino;


box::Switch::Switch(int pin_switch) {
    box::Switch::pin_switch = pin_switch;
    pinMode(pin_switch, INPUT_PULLUP);
}

box::Switch::~Switch() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

int box::Switch::is_high() {
    return digitalRead(box::Switch::pin_switch);
}
