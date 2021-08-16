#include "ardunio_namespace.h" // needed for arduino build
#include "peripheral/box_servomotor.hpp"
#include "Servo.h"
#include <stdio.h>

box::Servomotor::Servomotor(int pin_pwm_servomotor, bool clockwise,
                            int min_peak_angle, int max_peak_angle,
                            box::Wait* box_wait_till_servo_finished_moving) {
    box::Servomotor::min_peak_angle = min_peak_angle;
    box::Servomotor::max_peak_angle = max_peak_angle;
    box::Servomotor::clockwise = clockwise;
    box::Servomotor::box_wait_servo_speed = box_wait_till_servo_finished_moving;
    set_angle(box::Servomotor::min_peak_angle);
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

int box::Servomotor::move_to_percent(int percentage, int speed) {
    if(percentage > 100) { percentage = 100;}
    if(percentage < 0) { percentage = 0; }
    box::Servomotor::speed_sleep_ms = speed_to_millseconds(speed);
    last_percentage = percentage;
    int angle = (int)(((box::Servomotor::max_peak_angle - box::Servomotor::min_peak_angle) *
                        (percentage / 100.0)) +
                        box::Servomotor::min_peak_angle
                     );
    set_angle(angle);
    return calculate_sleep_time_ms();
}

void box::Servomotor::move_step() {
    if(angle == current_angle) { return; }
    if(!box_wait_servo_speed->is_expired()) { return; }
    if(current_angle > angle) {
        current_angle--;
    } else {
        current_angle++;
    }
    if(box::Servomotor::clockwise) {
        servo.write(180 - current_angle);
    } else {
        servo.write(current_angle);
    }
    box_wait_servo_speed->milliseconds(speed_sleep_ms);
}

bool box::Servomotor::current_angle_smaller_than_95_percent(){
    double calc_angle = (double) current_angle;
    double percent_95 = (((double)max_peak_angle/100.0)*95.0);
    return calc_angle < percent_95;
}

int box::Servomotor::move_to_angle(int angle, int speed) {
    box::Servomotor::speed_sleep_ms = speed_to_millseconds(speed);
    set_angle(angle);
    return calculate_sleep_time_ms();
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
    return (steps * speed_sleep_ms) + (steps * 4);
}

void box::Servomotor::set_angle(int angle) {
    if(angle < min_peak_angle) { box::Servomotor::angle = min_peak_angle; return; }
    if(angle > max_peak_angle) { box::Servomotor::angle = max_peak_angle; return; }
    box::Servomotor::angle = angle;
}
