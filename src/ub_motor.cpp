#include "ub_motor.hpp"

void ub::Motor::addition(int a, int b) {
    m_lastResult = a + b;
}

int ub::Motor::lastResult() {
    return m_lastResult;
}

