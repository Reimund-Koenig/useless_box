#include "Servo.h"
#include "box_servomotor.hpp"
#include <stdio.h>

box::Servomotor::Servomotor(int pin_pwm_servomotor) {
    servo.attach(pin_pwm_servomotor);
    box::Servomotor::angle = SERVOMOTOR_DEFAULT_ANGLE;
    box::Servomotor::direction = SERVOMOTOR_DEFAULT_DIRECTION;
}

box::Servomotor::~Servomotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Servomotor::move(int direction) {
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

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Servomotor::set_angle(int angle) {
    if(angle < 0) { box::Servomotor::angle = 0; return; }
    if(angle > 180) { box::Servomotor::angle = 180; return; }
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
    servo.write(angle);
}

void box::Servomotor::move_backward() {
    set_angle(angle-1);
    servo.write(angle);
}
