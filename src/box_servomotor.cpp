#include "ardunio_namespace.h" // needed for arduino build
#include "box_servomotor.hpp"
#include "Servo.h"
#include <stdio.h>

box::Servomotor::Servomotor(int pin_pwm_servomotor, bool clockwise,
                            int min_peak_angle, int max_peak_angle,
                            box::Wait* box_wait) {
    box::Servomotor::min_peak_angle = min_peak_angle;
    box::Servomotor::max_peak_angle = max_peak_angle;
    box::Servomotor::clockwise = clockwise;
    box::Servomotor::box_wait = box_wait;
    set_angle(min_peak_angle);
    servo.write(box::Servomotor::angle);
    box::Servomotor::current_angle = box::Servomotor::angle;
    servo.attach(pin_pwm_servomotor);
    last_percentage = 0;
}

box::Servomotor::~Servomotor() {
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

void box::Servomotor::move_to_percent(int percentage) {
    if(percentage > 100) { percentage = 100;}
    if(percentage < 0) { percentage = 0; }
    last_percentage = percentage;
    int angle = (int)(((box::Servomotor::max_peak_angle -  box::Servomotor::min_peak_angle) *
                        (percentage / 100.0)) +
                        box::Servomotor::min_peak_angle
                     );
    set_angle(angle);
}

int box::Servomotor::set_speed_and_get_sleeptime(int speed) {
    box::Servomotor::speed_in_milliseconds = speed_to_millseconds(speed);
    return calculate_sleep_time(speed);
}

void box::Servomotor::move_step() {
    if(angle == current_angle) { return; }
    if(!box_wait->is_free()) { return; }
    if(current_angle > angle) {
        current_angle--;
    } else {
        current_angle++;
    }
    servo.write(current_angle);
    box_wait->milliseconds(speed_in_milliseconds);
}

void box::Servomotor::move_to_angle(int angle) {
    set_angle(angle);
}

int box::Servomotor::get_last_percentage() {
    return box::Servomotor::last_percentage;
}

int box::Servomotor::get_angle() {
    return box::Servomotor::current_angle;
}

int box::Servomotor::get_current_angle() {
    return servo.read();
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

unsigned long box::Servomotor::speed_to_millseconds(int speed) {
    if(speed > 5) { return (unsigned long) 0; }
    if(speed > 4) { return (unsigned long) 10; }
    if(speed > 3) { return (unsigned long) 15; }
    if(speed > 2) { return (unsigned long) 20; }
    if(speed > 1) { return (unsigned long) 25; }
    return (unsigned long) 30;
}

void box::Servomotor::set_angle(int angle) {
    if(box::Servomotor::clockwise) {
        angle = min_peak_angle - angle + max_peak_angle;
    }
    if(angle < min_peak_angle) { box::Servomotor::angle = min_peak_angle; return; }
    if(angle > max_peak_angle) { box::Servomotor::angle = max_peak_angle; return; }
    box::Servomotor::angle = angle;
}

int box::Servomotor::calculate_sleep_time(int speed) {
    if(speed > 5) { return 400; }
    if(speed > 4) { return 800; }
    if(speed > 3) { return 1000; }
    if(speed > 2) { return 1200; }
    if(speed > 1) { return 1400; }
    return 1600;
}