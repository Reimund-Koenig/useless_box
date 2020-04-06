#include "ardunio_namespace.h" // needed for arduino build
#include "box_systemtest.hpp"
#include "box_servomotor.hpp"
#include "box_button.hpp"
#include "box_switch.hpp"
#include "box_potentiometer.hpp"
#include "box_sonar.hpp"
#include <Serial.h>
#include <Arduino.h>

using namespace arduino;

#define PIN_RANDOM A0 //A0
#define PIN_POTENTIOMETER A1 //A1
#define PIN_BUTTON 2
#define PIN_SWITCH 4
#define LOWER_SERVO_PWM 9
#define LOWER_SERVO_MIN 0
#define LOWER_SERVO_MAX 133
#define LOWER_SERVO_CLOCKWISE false
#define UPPER_SERVO_PWM 10
#define UPPER_SERVO_MIN 42
#define UPPER_SERVO_MAX 180
#define UPPER_SERVO_CLOCKWISE true
#define PIN_SONAR_TRIGGER 11
#define PIN_SONAR_ECHO 12

#define BOX_MIN_RANGE 0
#define BOX_MAX_RANGE 200
#define BOX_BUTTON_DEBOUNCE_DELAY 50

box::Systemtest::Systemtest() {
    box_wait_lower_servo = new box::Wait();
    box_wait_upper_servo = new box::Wait();
    box_switch = new box::Switch(PIN_SWITCH);
    box_sonar = new box::Sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO);
    box_potentiometer = new box::Potentiometer(PIN_POTENTIOMETER,
                                               BOX_MIN_RANGE,
                                               BOX_MAX_RANGE);
    box_lower_servomotor = new box::Servomotor(LOWER_SERVO_PWM,
                                            LOWER_SERVO_CLOCKWISE,
                                            LOWER_SERVO_MIN,
                                            LOWER_SERVO_MAX,
                                            box_wait_lower_servo);
    box_upper_servomotor = new box::Servomotor(UPPER_SERVO_PWM,
                                            UPPER_SERVO_CLOCKWISE,
                                           UPPER_SERVO_MIN,
                                           UPPER_SERVO_MAX,
                                           box_wait_upper_servo);
    box_button = new box::Button(PIN_BUTTON, BOX_BUTTON_DEBOUNCE_DELAY);
    systemtest_state = 1;
    number_of_functions = 5;
    last_switch_state = -1;
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    delay(1000);
    Serial.println("Systemtest Started");
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
            break;
        case 3: // Sonar
            test_sonar_avarage();
            break;
        case 4: // Lower Servomotor
            test_lower_servomotor();
            break;
        case 5:  // Upper Servomotor
            test_upper_servomotor();
            break;
        default:
            break;
    }
}
void box::Systemtest::println(const char* str, int val=-1) {
    Serial.print(str);
    if(val != -1) {
        Serial.print(val);
    }
    Serial.println(".");
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
    bool switch_state = box_switch->is_high();
    // TODO: SERIAL PRINT LINE switch_state
    if(switch_state) {
        if(last_switch_state != HIGH) {
            println("Switch State is  ", HIGH);
            last_switch_state = HIGH;
        }
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        if(last_switch_state != LOW) {
            println("Switch State is  ", LOW);
            last_switch_state = LOW;
        }
    }
}

void box::Systemtest::test_sonar() {
    int distance_cm = box_sonar->get_distance_cm();
    println("Distance is  ", distance_cm);
    delay(10);
    return;
}

void box::Systemtest::test_sonar_avarage() {
    int distance_cm = box_sonar->get_average_distance_cm();
    println("Avarage Distance is  ", distance_cm);
    delay(10);
    return;
}

void box::Systemtest::test_lower_servomotor() {
    servomotor_angle = box_potentiometer->get_value();
    box_lower_servomotor->move_to_angle(servomotor_angle);
    if ((millis() - last_debounce_time) > 1000) {
        println("Motorangle is  ", servomotor_angle);
        last_debounce_time = millis();
    }
}

void box::Systemtest::test_upper_servomotor() {
    servomotor_angle = box_potentiometer->get_value();
    box_upper_servomotor->move_to_angle(servomotor_angle);
    if ((millis() - last_debounce_time) > 1000) {
        println("Motorangle is  ", servomotor_angle);
        last_debounce_time = millis();
    }
}