// Copyright 2014 http://switchdevice.com

#include "Serial.h"

static SerialMock* gSerialMock = NULL;
SerialMock* serialMockInstance() {
  if(!gSerialMock) {
    gSerialMock = new SerialMock();
  }
  return gSerialMock;
}

void releaseSerialMock() {
  if(gSerialMock) {
    delete gSerialMock;
    gSerialMock = NULL;
  }
}

// void printDouble(double num, int digits) {}
// template<typename T> void printBase(T num, int base) {}
// bool Serial_::printToCout = false;
// void Serial_::setPrintToCout(bool flag) {}
// size_t Serial_::print(const char *s) {}
// size_t Serial_::print(char c) {}
// size_t Serial_::print(unsigned char c, int base) {}
// size_t Serial_::print(int num, int base) {}
// size_t Serial_::print(unsigned int num, int base) {}
// size_t Serial_::print(long num, int base) {}
// size_t Serial_::print(unsigned long num, int base) {}
// size_t Serial_::print(double num, int digits) {}
// size_t Serial_::println(const char *s) {}
// size_t Serial_::println(char c) {}
// size_t Serial_::println(unsigned char c, int base) {}
// size_t Serial_::println(int num, int base) {}
// size_t Serial_::println(unsigned int num, int base) {}
// size_t Serial_::println(long num, int base) {}
// size_t Serial_::println(unsigned long num, int base) {}
// size_t Serial_::println(double num, int digits) {}
// size_t Serial_::println(void) {}
// size_t Serial_::write(int val) {}
// size_t Serial_::write(const char *str) {}
// size_t Serial_::write(const int *buffer, size_t size) {}
int Serial_::begin(int port) {}
// void Serial_::flush() {}
// int Serial_::available() {}
// int Serial_::read() {}
// Preinstantiate Objects
Serial_ Serial;
