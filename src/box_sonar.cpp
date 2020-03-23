#include "box_sonar.hpp"
#include <Arduino.h>
#include <stdio.h>

box::Sonar::Sonar(int pin_trigger, int pin_echo) {
    box::Sonar::pin_trigger = pin_trigger;
    box::Sonar::pin_echo = pin_echo;
    box::Sonar::average_not_the_first_iteration = false;
    box::Sonar::average_iter = 0;
    box::Sonar::average_summary = 0;
    pinMode(box::Sonar::pin_trigger, OUTPUT);
    digitalWrite(box::Sonar::pin_trigger, LOW);
    pinMode(box::Sonar::pin_echo, INPUT);
}

box::Sonar::~Sonar(){
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

unsigned int box::Sonar::get_average_distance_cm(){
    box::Sonar::get_distance_cm();
    return average_distance_cm;
}

unsigned int box::Sonar::get_distance_cm(){
    long duration;
    digitalWrite(box::Sonar::pin_trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(box::Sonar::pin_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(box::Sonar::pin_trigger, LOW);
    duration = pulseIn(box::Sonar::pin_echo, HIGH);
    unsigned int distance_in_cm = (int)((duration/2.0)*0.0343);
    box::Sonar::calculate_average(distance_in_cm);
    return distance_in_cm;
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Sonar::calculate_average(int distance_in_cm){
    if(average_iter >= NUMBER_OF_AVERAGE_VALUES) {
        average_iter = 0;
        average_not_the_first_iteration = true;
    }
    average_summary += distance_in_cm;
    if(average_not_the_first_iteration) {
        average_summary -= average_array[average_iter];
        average_array[average_iter] = distance_in_cm;
        average_distance_cm = (int)(average_summary/NUMBER_OF_AVERAGE_VALUES);
    } else {
        average_array[average_iter] = distance_in_cm;
        average_distance_cm = (int)(average_summary/(average_iter+1));
    }
    average_iter++;
}