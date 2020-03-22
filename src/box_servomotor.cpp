#include "box_servomotor.hpp"
#include "Servo.h"
#include <stdio.h>

box::Servomotor::Servomotor(int pin_pwm_servomotor, bool clockwise, int start, int min_peak_angle, int max_peak_angle) {
    servo.attach(pin_pwm_servomotor);
    box::Servomotor::min_peak_angle = min_peak_angle;
    box::Servomotor::max_peak_angle = max_peak_angle;
    box::Servomotor::angle = min_peak_angle;
    box::Servomotor::clockwise = clockwise;
    box::Servomotor::direction = SERVOMOTOR_DEFAULT_DIRECTION;
}

box::Servomotor::~Servomotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Servomotor::move_to_percent(int percentage) {
    int angle = (int)(((box::Servomotor::max_peak_angle -  box::Servomotor::min_peak_angle) *
                        (percentage / 100.0)) +
                        box::Servomotor::min_peak_angle
                     );
    set_angle(angle);
    servo.write(box::Servomotor::angle);
}

void box::Servomotor::move_angle(int angle) {
    set_angle(angle);
    servo.write(box::Servomotor::angle);
}

void box::Servomotor::move_direction(int direction) {
    set_direction(direction);
    if (box::Servomotor::direction == SERVOMOTOR_DIRECTION_STOP) {
        move_stop();
    } else if (box::Servomotor::direction == SERVOMOTOR_DIRECTION_FORWARD) {
        move_forward();
    } else if (box::Servomotor::direction == SERVOMOTOR_DIRECTION_BACKWARD) {
        move_backward();
    }
}

int box::Servomotor::get_direction() {
    return box::Servomotor::direction;
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
    if(angle < min_peak_angle) { box::Servomotor::angle = min_peak_angle; return; }
    if(angle > max_peak_angle) { box::Servomotor::angle = max_peak_angle; return; }
    box::Servomotor::angle = angle;
}

void box::Servomotor::set_direction(int direction) {
    if(direction == SERVOMOTOR_DIRECTION_FORWARD ||
              direction == SERVOMOTOR_DIRECTION_BACKWARD) {
        box::Servomotor::direction = direction;
        return;
    }
    box::Servomotor::direction = SERVOMOTOR_DIRECTION_STOP;
}

void box::Servomotor::move_stop() {
}

void box::Servomotor::move_forward() {
    set_angle(angle+1);
    servo.write(box::Servomotor::angle);
}

void box::Servomotor::move_backward() {
    set_angle(angle-1);
    servo.write(box::Servomotor::angle);
}
