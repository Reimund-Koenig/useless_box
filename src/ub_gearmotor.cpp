#include "ub_gearmotor.hpp"
#include <stdio.h>
#include "Arduino.h"

ub::Gearmotor::Gearmotor(int pin_pwm_speed, int pin_forward, int pin_backward) {
    ub::Gearmotor::speed = GEARMOTOR_DEFAULT_SPEED;
    ub::Gearmotor::direction = GEARMOTOR_DEFAULT_DIRECTION;
    ub::Gearmotor::pin_pwm_speed = pin_pwm_speed;
    ub::Gearmotor::pin_forward = pin_forward;
    ub::Gearmotor::pin_backward = pin_backward;
    pinMode(pin_pwm_speed, OUTPUT);
    pinMode(pin_forward, OUTPUT);
    pinMode(pin_backward, OUTPUT);
}

ub::Gearmotor::~Gearmotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void ub::Gearmotor::move(int direction, int speed) {
    set_direction(direction);
    set_speed(speed);
    if (ub::Gearmotor::direction == GEARMOTOR_DIRECTION_STOP) {
        move_stop();
    } else if (ub::Gearmotor::direction == GEARMOTOR_DIRECTION_FORWARD) {
        move_forward();
    } else if (ub::Gearmotor::direction == GEARMOTOR_DIRECTION_BACKWARD) {
        move_backward();
    }
}

int ub::Gearmotor::get_speed() {
    return ub::Gearmotor::speed;
}

int ub::Gearmotor::get_direction() {
    return ub::Gearmotor::direction;
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void ub::Gearmotor::set_speed(int speed) {
    if(speed < 0) { ub::Gearmotor::speed = 0; return; }
    if(speed > 255) { ub::Gearmotor::speed = 255; return; }
    ub::Gearmotor::speed = speed;
}

void ub::Gearmotor::set_direction(int direction) {
    if(direction == GEARMOTOR_DIRECTION_FORWARD ||
              direction == GEARMOTOR_DIRECTION_BACKWARD) {
        ub::Gearmotor::direction = direction;
        return;
    }
    ub::Gearmotor::direction = GEARMOTOR_DIRECTION_STOP;
}

void ub::Gearmotor::move_stop() {
    digitalWrite(ub::Gearmotor::pin_forward, LOW);
    digitalWrite(ub::Gearmotor::pin_backward, LOW);
    analogWrite(ub::Gearmotor::pin_pwm_speed, ub::Gearmotor::speed);
}

void ub::Gearmotor::move_forward() {
    digitalWrite(ub::Gearmotor::pin_forward, HIGH);
    digitalWrite(ub::Gearmotor::pin_backward, LOW);
    analogWrite(ub::Gearmotor::pin_pwm_speed, ub::Gearmotor::speed);
}

void ub::Gearmotor::move_backward() {
    digitalWrite(ub::Gearmotor::pin_forward, LOW);
    digitalWrite(ub::Gearmotor::pin_backward, HIGH);
    analogWrite(ub::Gearmotor::pin_pwm_speed, ub::Gearmotor::speed);
}
