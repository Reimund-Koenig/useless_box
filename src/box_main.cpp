#include "box_main.hpp"
#include <Arduino.h>
#include <stdio.h>

using namespace arduino;


#define POWER_SUPPLY_CONNECTED false

#define PIN_SWITCH 2
#define PIN_POWER_SERVOS 7
#define PIN_POWER_SONAR 8
#define PIN_LOWER_SERVO_PWM 9
#define PIN_UPPER_SERVO_PWM 10
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12

#define LOWER_SERVO_MAX 155
#define LOWER_SERVO_MIN 25
#define LOWER_SERVO_CLOCKWISE false
#define UPPER_SERVO_MIN 0
#define UPPER_SERVO_MAX 158
#define UPPER_SERVO_CLOCKWISE true

box::Main::Main() {
    box_wait_servo_speed_control = new box::Wait();
    box_wait_deepsleep = new box::Wait();
    box_wait_lower_servo = new box::Wait();
    box_wait_upper_servo = new box::Wait();
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO);
    box_switch = new box::Switch(PIN_SWITCH);
    box_lower_servo = new box::Servomotor(
                        PIN_LOWER_SERVO_PWM, LOWER_SERVO_CLOCKWISE,
                        LOWER_SERVO_MIN, LOWER_SERVO_MAX,
                        box_wait_lower_servo);
    box_upper_servo = new box::Servomotor(
                        PIN_UPPER_SERVO_PWM, UPPER_SERVO_CLOCKWISE,
                        UPPER_SERVO_MIN, UPPER_SERVO_MAX,
                        box_wait_upper_servo);
    box_servo_manager = new box::Servomanager(box_lower_servo, box_upper_servo, box_wait_servo_speed_control, box_switch);
    box_mode_manager = new box::ModeManager(box_servo_manager, box_switch);
    bool is_engery_safe_mode = !POWER_SUPPLY_CONNECTED;
    pinMode(PIN_POWER_SERVOS, OUTPUT);
    pinMode(PIN_POWER_SONAR, OUTPUT);
    digitalWrite(PIN_POWER_SERVOS, HIGH);
    digitalWrite(PIN_POWER_SONAR, HIGH);
    randomSeed(analogRead(0));
    box_controller = new box::Controller(   is_engery_safe_mode,
                                            box_switch,
                                            box_sonar,
                                            box_servo_manager,
                                            box_wait_servo_speed_control,
                                            box_wait_deepsleep,
                                            box_mode_manager);
}

box::Main::~Main() {
}
