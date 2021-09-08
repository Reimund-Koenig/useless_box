#include "ardunio_namespace.h" // needed for arduino build
#include "peripheral/box_sonar.hpp"
#include <Arduino.h>
#include <stdio.h>

using namespace arduino;

box::Sonar::Sonar(int pin_trigger, int pin_echo, box::Wait* box_wait_till_next_distance_measurement) {
    box::Sonar::pin_trigger = pin_trigger;
    box::Sonar::pin_echo = pin_echo;
    box::Sonar::average_not_the_first_iteration = false;
    box::Sonar::median_iter = 0;
    box::Sonar::average_iter = 0;
    box::Sonar::average_summary = 0;
    box::Sonar::sleep_time_till_next_measurement = (int)1000/MEASURES_PER_SECOND;
    box::Sonar::box_wait_till_next_distance_measurement = box_wait_till_next_distance_measurement;
    pinMode(box::Sonar::pin_trigger, OUTPUT);
    digitalWrite(box::Sonar::pin_trigger, LOW);
    pinMode(box::Sonar::pin_echo, INPUT);
}

box::Sonar::~Sonar(){
}

/*************************************************************************************************
 * Public Methods
 *************************************************/

unsigned int box::Sonar::get_median_distance_cm(){
    if(!box_wait_till_next_distance_measurement->is_expired()) {
        return box::Sonar::get_median();
    }
    box_wait_till_next_distance_measurement->milliseconds(sleep_time_till_next_measurement);
    box::Sonar::get_distance_cm();
    box::Sonar::calculate_median();
    return box::Sonar::get_median();
}

unsigned int box::Sonar::get_average_distance_cm(){
    if(!box_wait_till_next_distance_measurement->is_expired()) {
        return average_distance_cm;
    }
    box_wait_till_next_distance_measurement->milliseconds(sleep_time_till_next_measurement);
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
    box::Sonar::add_to_median_array(distance_in_cm);
    return distance_in_cm;
}

/*************************************************************************************************
 * Private Methods
 *************************************************/

void box::Sonar::add_to_median_array(int distance_in_cm){
    if(median_iter >= NUMBER_OF_MEDIAN_VALUES) {
        median_iter = 0;
    }
    median_array[median_iter] = distance_in_cm;
    median_iter++;
}

void box::Sonar::bubble_sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

unsigned int  box::Sonar::get_median () {
    int mid_index = (int) (NUMBER_OF_MEDIAN_VALUES/2.0);
    return sort_median_array[mid_index];
}

void box::Sonar::calculate_median () {
    for(int i=0; i<(NUMBER_OF_MEDIAN_VALUES-1); i++) {
        sort_median_array[i] = median_array[i];
    }
    bubble_sort(sort_median_array, NUMBER_OF_MEDIAN_VALUES);
}

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
