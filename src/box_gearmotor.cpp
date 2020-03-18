#include "box_gearmotor.hpp"
#include <Arduino.h>
#include <stdio.h>

box::Gearmotor::Gearmotor(int pin_pwm_speed, int pin_forward, int pin_backward) {
    box::Gearmotor::speed = GEARMOTOR_DEFAULT_SPEED;
    box::Gearmotor::direction = GEARMOTOR_DEFAULT_DIRECTION;
    box::Gearmotor::pin_pwm_speed = pin_pwm_speed;
    box::Gearmotor::pin_forward = pin_forward;
    box::Gearmotor::pin_backward = pin_backward;
    pinMode(pin_pwm_speed, OUTPUT);
    pinMode(pin_forward, OUTPUT);
    pinMode(pin_backward, OUTPUT);
}

box::Gearmotor::~Gearmotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Gearmotor::move(int direction, int speed) {
    set_direction(direction);
    set_speed(speed);
    if (box::Gearmotor::direction == GEARMOTOR_DIRECTION_STOP) {
        move_stop();
    } else if (box::Gearmotor::direction == GEARMOTOR_DIRECTION_FORWARD) {
        move_forward();
    } else if (box::Gearmotor::direction == GEARMOTOR_DIRECTION_BACKWARD) {
        move_backward();
    }
}

int box::Gearmotor::get_speed() {
    return box::Gearmotor::speed;
}

int box::Gearmotor::get_direction() {
    return box::Gearmotor::direction;
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Gearmotor::set_speed(int speed) {
    if(speed < 0) { box::Gearmotor::speed = 0; return; }
    if(speed > 255) { box::Gearmotor::speed = 255; return; }
    box::Gearmotor::speed = speed;
}

void box::Gearmotor::set_direction(int direction) {
    if(direction == GEARMOTOR_DIRECTION_FORWARD ||
              direction == GEARMOTOR_DIRECTION_BACKWARD) {
        box::Gearmotor::direction = direction;
        return;
    }
    box::Gearmotor::direction = GEARMOTOR_DIRECTION_STOP;
}

void box::Gearmotor::move_stop() {
    digitalWrite(box::Gearmotor::pin_forward, LOW);
    digitalWrite(box::Gearmotor::pin_backward, LOW);
    analogWrite(box::Gearmotor::pin_pwm_speed, box::Gearmotor::speed);
}

void box::Gearmotor::move_forward() {
    digitalWrite(box::Gearmotor::pin_forward, HIGH);
    digitalWrite(box::Gearmotor::pin_backward, LOW);
    analogWrite(box::Gearmotor::pin_pwm_speed, box::Gearmotor::speed);
}

void box::Gearmotor::move_backward() {
    digitalWrite(box::Gearmotor::pin_forward, LOW);
    digitalWrite(box::Gearmotor::pin_backward, HIGH);
    analogWrite(box::Gearmotor::pin_pwm_speed, box::Gearmotor::speed);
}
