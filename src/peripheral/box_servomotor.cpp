#include "ardunio_namespace.h" // needed for arduino build
#include "peripheral/box_servomotor.hpp"
#include "Servo.h"
#include <stdio.h>

box::Servomotor::Servomotor(int pin_pwm_servomotor, bool clockwise,
                            int min_peak_angle, int max_peak_angle,
                            box::Wait* box_wait_servo_step_speed) {
    box::Servomotor::min_peak_angle = min_peak_angle;
    box::Servomotor::max_peak_angle = max_peak_angle;
    box::Servomotor::clockwise = clockwise;
    box::Servomotor::box_wait_servo_step_speed = box_wait_servo_step_speed;
    set_angle(box::Servomotor::min_peak_angle);
    if(box::Servomotor::clockwise) {
        servo.write(180 - angle);
    } else {
        servo.write(angle);
    }
    box::Servomotor::current_angle = angle;
    servo.attach(pin_pwm_servomotor);
    last_percentage = 0;
}

box::Servomotor::~Servomotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

int box::Servomotor::move_to_percent(int percentage, int speed) {
    if(percentage > 100) { percentage = 100;}
    if(percentage < 0) { percentage = 0; }
    box::Servomotor::speed_sleep_ms = speed_to_millseconds(speed);
    box::Servomotor::last_percentage = percentage;
    int angle = (int)(((box::Servomotor::max_peak_angle - box::Servomotor::min_peak_angle) *
                        (percentage / 100.0)) +
                        box::Servomotor::min_peak_angle
                     );
    set_angle(angle);
    return calculate_sleep_time_ms();
}


void box::Servomotor::move() {
    if(angle == current_angle) { return; }
    if(speed_sleep_ms > 0) {
        move_step();
    } else {
        current_angle = angle;
        write();
    }
}

bool box::Servomotor::current_angle_smaller_than_90_percent(){
    double calc_angle = (double) current_angle;
    double percent_90 = (((double)max_peak_angle/100.0)*90.0);
    return calc_angle < percent_90;
}

int box::Servomotor::get_last_percentage() {
    return box::Servomotor::last_percentage;
}

int box::Servomotor::get_angle() {
    return box::Servomotor::current_angle;
}

int box::Servomotor::get_current_angle() {
    if(box::Servomotor::clockwise) {
        return 180 - servo.read();
    }
    return servo.read();
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Servomotor::write() {
    if(box::Servomotor::clockwise) {
        servo.write(180 - current_angle);
    } else {
        servo.write(current_angle);
    }
}

void box::Servomotor::move_step() {
    if(!box_wait_servo_step_speed->is_expired()) { return; }
    if(current_angle > angle) {
        current_angle--;
    } else {
        current_angle++;
    }
    write();
    box_wait_servo_step_speed->milliseconds(speed_sleep_ms);
}

unsigned long box::Servomotor::speed_to_millseconds(int speed) {
    if(speed > 5) { return (unsigned long) 0; }
    if(speed > 4) { return (unsigned long) 1; }
    if(speed > 3) { return (unsigned long) 2; }
    if(speed > 2) { return (unsigned long) 5; }
    if(speed > 1) { return (unsigned long) 8; }
    return (unsigned long) 10;
}

int box::Servomotor::calculate_sleep_time_ms() {
    int steps = 0;
    if(current_angle == angle) { return 0; }
    if(current_angle > angle) {
        steps = current_angle - angle;
    } else {
        steps = angle - current_angle;
    }
    return (steps * speed_sleep_ms) + (steps * 5);
}

void box::Servomotor::set_angle(int angle) {
    if(angle < min_peak_angle) { box::Servomotor::angle = min_peak_angle; return; }
    if(angle > max_peak_angle) { box::Servomotor::angle = max_peak_angle; return; }
    box::Servomotor::angle = angle;
}
