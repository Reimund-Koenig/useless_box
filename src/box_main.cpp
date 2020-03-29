#include "box_main.hpp"
using namespace arduino;

#define LOWER_SERVO_PWM 9
#define LOWER_SERVO_MIN 0
#define LOWER_SERVO_MAX 138
#define LOWER_SERVO_CLOCKWISE false
#define UPPER_SERVO_PWM 10
#define UPPER_SERVO_MIN 42
#define UPPER_SERVO_MAX 180
#define UPPER_SERVO_CLOCKWISE true
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12
#define PIN_SWITCH 4

box::Main::Main() {
    box_lower_servo = new box::Servomotor(
                        LOWER_SERVO_PWM, LOWER_SERVO_CLOCKWISE,
                        LOWER_SERVO_MIN, LOWER_SERVO_MAX);
    box_upper_servo = new box::Servomotor(
                        UPPER_SERVO_PWM, UPPER_SERVO_CLOCKWISE,
                        UPPER_SERVO_MIN, UPPER_SERVO_MAX);
    box_servomanager = new box::Servomanager(box_lower_servo, box_upper_servo);
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO);
    box_switch = new box::Switch(PIN_SWITCH);

    box_wait = new box::Wait();
    box_mode_manager = new box::ModeManager(box_servomanager, box_wait);

    box_controller = new box::Controller(box_switch,
                                            box_sonar,
                                            box_servomanager,
                                            box_wait,
                                            box_mode_manager);
    randomSeed(analogRead(0));
}

box::Main::~Main() {
}