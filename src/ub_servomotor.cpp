#include "ub_servomotor.hpp"
#include <stdio.h>

ub::Servomotor::Servomotor(int pin_pwm_servomotor) {
    servo.attach(pin_pwm_servomotor);
    ub::Servomotor::angle = SERVOMOTOR_DEFAULT_ANGLE;
    ub::Servomotor::direction = SERVOMOTOR_DEFAULT_DIRECTION;
    ub::Servomotor::speed_delay_ms = SERVOMOTOR_DEFAULT_SPEED;
}

ub::Servomotor::~Servomotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void ub::Servomotor::move(int direction, int speed_delay_ms) {
    set_direction(direction);
    set_speed(speed_delay_ms);
    if (ub::Servomotor::direction == SERVOMOTOR_DIRECTION_STOP) {
        move_stop();
    } else if (ub::Servomotor::direction == SERVOMOTOR_DIRECTION_FORWARD) {
        move_forward();
    } else if (ub::Servomotor::direction == SERVOMOTOR_DIRECTION_BACKWARD) {
        move_backward();
    }
}

int ub::Servomotor::get_speed() {
    return ub::Servomotor::speed_delay_ms;
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

void ub::Servomotor::set_speed(int speed_delay_ms) {
    if(speed_delay_ms < 0) { ub::Servomotor::speed_delay_ms = 0; return; }
    if(speed_delay_ms > 255) { ub::Servomotor::speed_delay_ms = 255; return; }
    ub::Servomotor::speed_delay_ms = speed_delay_ms;
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
    // if(sleep time not reached -- speed) { return; }
    set_angle(angle+1);
    servo.write(angle);
}

void ub::Servomotor::move_backward() {
    // if(sleep time not reached -- speed) { return; }
    set_angle(angle-1);
    servo.write(angle);
}
