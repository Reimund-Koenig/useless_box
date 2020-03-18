#include "box_potentiometer.hpp"
#include <Arduino.h>
#include <stdio.h>


box::Potentiometer::Potentiometer(int pin_potentiometer, int min_peak, int max_peak) {
    box::Potentiometer::pin_potentiometer = pin_potentiometer;
    box::Potentiometer::min_peak = min_peak;
    box::Potentiometer::max_peak = max_peak;
}

box::Potentiometer::~Potentiometer() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

int box::Potentiometer::get_value() {
    // reads the value of the potentiometer (value between 0 and 1023)
    int value = analogRead(box::Potentiometer::pin_potentiometer);
    // scale it to use it with the servo (value between 0 and 180)
    return map(value, 0, 1023, min_peak, max_peak);
}
