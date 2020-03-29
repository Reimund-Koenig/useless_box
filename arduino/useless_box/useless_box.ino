#include "ardunio_namespace.h"
#include "box_main.hpp"
#include "box_switch.hpp"
#include "box_sonar.hpp"
#include "box_servomotor.hpp"
#include "box_wait.hpp"
#include "box_mode_normal.hpp"
#include "box_mode_awareness.hpp"
#include "box_mode_reset.hpp"


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


box::Main* box_main;
box::Switch* box_switch;
box::Sonar* box_sonar;
box::Servomanager* box_servomanager;
box::Servomotor* box_lower_servo;
box::Servomotor* box_upper_servo;

box::Wait* box_wait;
box::ModeAwareness box_mode_awareness;
box::ModeReset box_mode_reset;
box::ModeNormal box_mode_normal;

void setup() {
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
    box_mode_awareness = new box::ModeAwareness(box_servomanager, box_wait);
    box_mode_reset = new box::ModeReset(box_servomanager, box_wait);
    box_mode_normal = new box::ModeNormal(box_servomanager, box_wait);

    box_main = new box::Main(box_switch, box_sonar,
                             box_servomanager);
}

void loop() {
    box_main->run();
}