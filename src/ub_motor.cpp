#include "ub_motor.hpp"
#include <stdio.h>

ub::Motor::Motor() {
    ub::Motor::speed = MOTOR_DEFAULT_SPEED;
    ub::Motor::direction = MOTOR_DEFAULT_DIRECTION;
}

ub::Motor::~Motor() {
}

void ub::Motor::set_direction(int direction) {
    if(direction == MOTOR_DIRECTION_FORWARD ||
              direction == MOTOR_DIRECTION_BACKWARD) {
        ub::Motor::direction = direction;
        return;
    }
    ub::Motor::direction = MOTOR_DIRECTION_STOP;
}

int ub::Motor::get_direction() {
    return ub::Motor::direction;
}

void ub::Motor::set_speed(int speed) {
    if(speed < 0) { ub::Motor::speed = 0; return; }
    if(speed > 255) { ub::Motor::speed = 255; return; }
    ub::Motor::speed = speed;
}

int ub::Motor::get_speed() {
    return ub::Motor::speed;
}

void ub::Motor::move() {
    if (ub::Motor::direction == MOTOR_DIRECTION_STOP) return;
    if (ub::Motor::direction == MOTOR_DIRECTION_FORWARD) {
        move_forward();
    } else if (ub::Motor::direction == MOTOR_DIRECTION_BACKWARD) {
        move_backward();
    }
}

void ub::Motor::move_forward() {
    printf("Move Forward\n");
    // Move motor forward with ub::Motor::speed
}

void ub::Motor::move_backward() {
    printf("Move Backward\n");
    // Move motor backward with ub::Motor::speed
}