#include "Servo.h"
#include "ub_servomotor.hpp"
#include <stdio.h>

ub::Servomotor::Servomotor(int pin_pwm_servomotor) {
    servo.attach(pin_pwm_servomotor);
    ub::Servomotor::angle = SERVOMOTOR_DEFAULT_ANGLE;
    ub::Servomotor::direction = SERVOMOTOR_DEFAULT_DIRECTION;
}

ub::Servomotor::~Servomotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void ub::Servomotor::move(int direction) {
    set_direction(direction);
    if (ub::Servomotor::direction == SERVOMOTOR_DIRECTION_STOP) {
        move_stop();
    } else if (ub::Servomotor::direction == SERVOMOTOR_DIRECTION_FORWARD) {
        move_forward();
    } else if (ub::Servomotor::direction == SERVOMOTOR_DIRECTION_BACKWARD) {
        move_backward();
    }
}

int ub::Servomotor::get_direction() {
    return ub::Servomotor::direction;
}

int ub::Servomotor::get_angle() {
    return ub::Servomotor::angle;
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void ub::Servomotor::set_angle(int angle) {
    if(angle < 0) { ub::Servomotor::angle = 0; return; }
    if(angle > 180) { ub::Servomotor::angle = 180; return; }
    ub::Servomotor::angle = angle;
}

void ub::Servomotor::set_direction(int direction) {
    if(direction == SERVOMOTOR_DIRECTION_FORWARD ||
              direction == SERVOMOTOR_DIRECTION_BACKWARD) {
        ub::Servomotor::direction = direction;
        return;
    }
    ub::Servomotor::direction = SERVOMOTOR_DIRECTION_STOP;
}

void ub::Servomotor::move_stop() {
}

void ub::Servomotor::move_forward() {
    set_angle(angle+1);
    servo.write(angle);
}

void ub::Servomotor::move_backward() {
    set_angle(angle-1);
    servo.write(angle);
}
