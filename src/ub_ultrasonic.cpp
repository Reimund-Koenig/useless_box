#include "ub_ultrasonic.hpp"
#include "Arduino.h"

void ub::Ultrasonic::addition(int a, int b) {
    m_lastResult = a + b;
}

int ub::Ultrasonic::lastResult() {
    return m_lastResult;
}

