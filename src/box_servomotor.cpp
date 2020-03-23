#include "box_servomotor.hpp"
#include "Servo.h"
#include <stdio.h>

box::Servomotor::Servomotor(int pin_pwm_servomotor, bool clockwise,
                            int min_peak_angle, int max_peak_angle) {
    box::Servomotor::min_peak_angle = min_peak_angle;
    box::Servomotor::max_peak_angle = max_peak_angle;
    box::Servomotor::clockwise = clockwise;
    set_angle(min_peak_angle);
    servo.write(box::Servomotor::angle);
    servo.attach(pin_pwm_servomotor);
}

box::Servomotor::~Servomotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Servomotor::move_to_percent(int percentage) {
    if(percentage > 100) { percentage = 100; }
    if(percentage < 0) { percentage = 0; }
    int angle = (int)(((box::Servomotor::max_peak_angle -  box::Servomotor::min_peak_angle) *
                        (percentage / 100.0)) +
                        box::Servomotor::min_peak_angle
                     );
    set_angle(angle);
    servo.write(box::Servomotor::angle);
}

void box::Servomotor::move_to_angle(int angle) {
    set_angle(angle);
    servo.write(box::Servomotor::angle);
}

int box::Servomotor::get_angle() {
    return box::Servomotor::angle;
}

int box::Servomotor::get_current_angle() {
    return servo.read();
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Servomotor::set_angle(int angle) {
    if(box::Servomotor::clockwise) {
        angle = min_peak_angle - angle + max_peak_angle;
    }
    if(angle < min_peak_angle) { box::Servomotor::angle = min_peak_angle; return; }
    if(angle > max_peak_angle) { box::Servomotor::angle = max_peak_angle; return; }
    box::Servomotor::angle = angle;
}
