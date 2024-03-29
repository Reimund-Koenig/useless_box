#include "box_main.hpp"
#include <Arduino.h>
#include <stdio.h>

using namespace arduino;

#define PIN_SWITCH 2
#define PIN_POWER_SERVOS 7
#define PIN_POWER_SONAR 8
#define PIN_LOWER_SERVO_PWM 9
#define PIN_UPPER_SERVO_PWM 10
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12

#define LOWER_SERVO_MAX 160
#define LOWER_SERVO_MIN 0
#define LOWER_SERVO_CLOCKWISE false
#define UPPER_SERVO_MIN 10
#define UPPER_SERVO_MAX 160
#define UPPER_SERVO_CLOCKWISE true

box::Main::Main() {
    box_wait_till_servomanager_finished_moving = new box::Wait();
    box_wait_deepsleep = new box::Wait();
    box_wait_till_next_distance_measurement = new box::Wait();
    box_wait_servo_step_speed_lower = new box::Wait();
    box_wait_servo_step_speed_upper = new box::Wait();
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO, box_wait_till_next_distance_measurement);
    box_switch = new box::Switch(PIN_SWITCH);
    box_lower_servo = new box::Servomotor(
                        PIN_LOWER_SERVO_PWM, LOWER_SERVO_CLOCKWISE,
                        LOWER_SERVO_MIN, LOWER_SERVO_MAX,
                        box_wait_servo_step_speed_lower);
    box_upper_servo = new box::Servomotor(
                        PIN_UPPER_SERVO_PWM, UPPER_SERVO_CLOCKWISE,
                        UPPER_SERVO_MIN, UPPER_SERVO_MAX,
                        box_wait_servo_step_speed_upper);
    box_servo_manager = new box::Servomanager(box_lower_servo,
                                              box_upper_servo,
                                              box_wait_till_servomanager_finished_moving,
                                              box_switch);
    box_mode_manager = new box::ModeManager(box_servo_manager);
    pinMode(PIN_POWER_SERVOS, OUTPUT);
    pinMode(PIN_POWER_SONAR, OUTPUT);
    digitalWrite(PIN_POWER_SERVOS, HIGH);
    digitalWrite(PIN_POWER_SONAR, HIGH);
    randomSeed(analogRead(0));
    box_controller = new box::Controller(
                                        box_switch,
                                        box_sonar,
                                        box_servo_manager,
                                        box_wait_deepsleep,
                                        box_mode_manager
                                        );
}

box::Main::~Main() {
}
