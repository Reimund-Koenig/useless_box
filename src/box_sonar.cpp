#include "box_sonar.hpp"
#include "Arduino.h"

void box::Sonar::addition(int a, int b) {
    m_lastResult = a + b;
}

int box::Sonar::lastResult() {
    return m_lastResult;
}

