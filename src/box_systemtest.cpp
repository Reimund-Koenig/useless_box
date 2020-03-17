
#include "Servo.h"
#include "box_servomotor.hpp"
#include "box_button.hpp"
#include "box_switch.hpp"
#include "box_sonar.hpp"
#include "box_systemtest.hpp"
#include "Arduino.h"

#define PIN_SERVO_1 9
#define PIN_SERVO_2 10

box::Systemtest::Systemtest() {
    box_servo1 = new box::Servomotor(PIN_SERVO_1, 0, 200);
    box_servo2 = new box::Servomotor(PIN_SERVO_2, 0, 200);
    box_button = new box::Button(2, 50);
    box_switch = new box::Switch(3);
    // box_sonar = new box::Sonar();
    systemtest_state = 0;
    number_of_functions = 3;
    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(analogRead(0));
}

box::Systemtest::~Systemtest() {
}

void box::Systemtest::run() {
    update_systemtest_state();
    switch (systemtest_state) {
        case 0: // Servomotor 1
            test_servomotor_1();
            break;
        case 1:  // Servomotor 2
            test_servomotor_2();
            break;
        case 2:  // Switch
            test_switch();
            break;
        case 3: // Sonar
            test_sonar();
        default:
            break;
    }
}

void box::Systemtest::update_systemtest_state() {
    if(box_button->pressed()) {
        systemtest_state++;
        if(systemtest_state >= number_of_functions) {
            systemtest_state = 0;
        }
    }
}

void box::Systemtest::test_servomotor_1() {
    // Potentiometer needed
    // while(servo1->get_angle() < 180 ) {
    //     servo1->move(SERVOMOTOR_DIRECTION_FORWARD);
    // }
    return;
}

void box::Systemtest::test_servomotor_2() {
    // Potentiometer needed
    // while(servo1->get_angle() < 180 ) {
    //     servo1->move(SERVOMOTOR_DIRECTION_FORWARD);
    // }
    return;
}

void box::Systemtest::test_switch() {
    if(box_switch->get_state()) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void box::Systemtest::test_sonar() {
    return;
}