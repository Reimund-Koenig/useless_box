#ifndef Arduino_h
#define Arduino_h

#define LED_BUILTIN 0
#define CHANGE 1
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define LOW 0
#define HIGH 1

extern "C" {
    void pinMode(int, int);
    void digitalWrite(int, int);
    int digitalRead(int);
    void analogWrite(int, int);
    int analogRead(int);
    void delay(int);
    void delayMicroseconds(int);
    unsigned long micros();
    void attachInterrupt(int, void (*)(void), int mode);
    // long random(long);
    void randomSeed(unsigned int);
}

#endif
