#include "box_systemtest.hpp"
#include "box_servomotor.hpp"
#include "box_button.hpp"
#include "box_switch.hpp"
#include "box_potentiometer.hpp"
#include "box_sonar.hpp"
#include <Arduino.h>
#include "Serial.h"
#include <stdio.h>

#define PIN_RANDOM 0 //A0
#define PIN_POTENTIOMETER 1 //A1
#define PIN_BUTTON 2
#define PIN_SWITCH 3
#define PIN_SERVOMOTOR_1 9
#define PIN_SERVOMOTOR_2 10
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12

#define BOX_MIN_RANGE 0
#define BOX_MAX_RANGE 200
#define BOX_BUTTON_DEBOUNCE_DELAY 50

box::Systemtest::Systemtest() {
    box_potentiometer = new box::Potentiometer(PIN_POTENTIOMETER,
                                               BOX_MIN_RANGE,
                                               BOX_MAX_RANGE);
    box_servomotor_1 = new box::Servomotor(PIN_SERVOMOTOR_1,
                                           BOX_MIN_RANGE,
                                           BOX_MAX_RANGE);
    box_servomotor_2 = new box::Servomotor(PIN_SERVOMOTOR_2,
                                           BOX_MIN_RANGE,
                                           BOX_MAX_RANGE);
    box_button = new box::Button(PIN_BUTTON, BOX_BUTTON_DEBOUNCE_DELAY);
    box_switch = new box::Switch(PIN_SWITCH);
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO);
    systemtest_state = 1;
    number_of_functions = 4;
    pinMode(LED_BUILTIN, OUTPUT);
    serial.begin(9600);
    delay(1000);
    serial.print("Systemtest Started");
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
    delay(1000);
}
void box::Systemtest::println(const char* str, int val=-1) {
    serial.print(str);
    if(val != -1) {
        serial.print(val);
    }
    serial.println(".");
}

void box::Systemtest::update_systemtest_state() {
    if(box_button->pressed()) {
        println("Button pressed");
        systemtest_state++;
        if(systemtest_state > number_of_functions) {
            systemtest_state = 1;
        }
        println("Switched to Systemtest ", systemtest_state);
    }
}

void box::Systemtest::test_switch() {
    bool switch_state = box_switch->get_state();
    // TODO: SERIAL PRINT LINE switch_state
    if(switch_state) {
        digitalWrite(LED_BUILTIN, HIGH);
        println("Switch State is  ", HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        println("Switch State is  ", LOW);
    }
}

void box::Systemtest::test_sonar() {
    int distance_cm = box_sonar->get_distance_cm();
    println("Distance is  ", distance_cm);
    return;
}

void box::Systemtest::test_servomotor_1() {
    servomotor_angle = box_potentiometer->get_value();
    box_servomotor_1->move_angle(servomotor_angle);
    println("Motorangle is  ", servomotor_angle);
}

void box::Systemtest::test_servomotor_2() {
    servomotor_angle = box_potentiometer->get_value();
    box_servomotor_2->move_angle(servomotor_angle);
    println("Motorangle is  ", servomotor_angle);
}