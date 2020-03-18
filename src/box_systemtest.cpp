
#include "Servo.h"
#include "box_servomotor.hpp"
#include "box_button.hpp"
#include "box_switch.hpp"
#include "box_sonar.hpp"
#include "box_systemtest.hpp"
#include "Arduino.h"

#define PIN_RANDOM 0
#define PIN_BUTTON 2
#define PIN_SWITCH 3
#define PIN_SERVOMOTOR_1 9
#define PIN_SERVOMOTOR_2 10
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12

box::Systemtest::Systemtest() {
    box_servo1 = new box::Servomotor(PIN_SERVOMOTOR_1, 0, 200);
    box_servo2 = new box::Servomotor(PIN_SERVOMOTOR_2, 0, 200);
    box_button = new box::Button(PIN_BUTTON, 50);
    box_switch = new box::Switch(PIN_SWITCH);
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO);
    systemtest_state = 1;
    number_of_functions = 4;
    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(analogRead(PIN_RANDOM));
}

box::Systemtest::~Systemtest() {
}

void box::Systemtest::run() {
    update_systemtest_state();
    switch (systemtest_state) {
        case 1:  // Switch
            test_switch();
            break;
        case 2: // Sonar
            test_sonar();
            break;
        case 3: // Servomotor 1
            test_servomotor_1();
            break;
        case 4:  // Servomotor 2
            test_servomotor_2();
            break;
        default:
            break;
    }
}

void box::Systemtest::update_systemtest_state() {
    if(box_button->pressed()) {
        systemtest_state++;
        if(systemtest_state > number_of_functions) {
            systemtest_state = 1;
        }
    }
}

void box::Systemtest::test_switch() {
    if(box_switch->get_state()) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void box::Systemtest::test_sonar() {
    box_sonar->get_distance_cm();
    delay(1000);
    return;
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