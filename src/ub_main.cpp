#include "ub_main.hpp"

#include "Arduino.h"

ub::Main::Main() {
    // setup
    pinMode(LED_BUILTIN, OUTPUT);
}

ub::Main::~Main() {
}

void ub::Main::addition(int a, int b) {
    m_lastResult = a + b;
}

int ub::Main::lastResult() {
    return m_lastResult;
}

void ub::Main::blink() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}