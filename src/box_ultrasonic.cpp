#include "box_ultrasonic.hpp"
#include "Arduino.h"

void box::Ultrasonic::addition(int a, int b) {
    m_lastResult = a + b;
}

int box::Ultrasonic::lastResult() {
    return m_lastResult;
}

