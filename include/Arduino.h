#ifndef Arduino_h
#define Arduino_h

#define LED_BUILTIN 0
#define CHANGE 1
#define INPUT 0x0

extern "C" {
    #define OUTPUT 0
    #define LOW 0
    #define HIGH 1
    void pinMode(int, int);
    void digitalWrite(int, int);
    int digitalRead(int);
    void analogWrite(int, int);
    void delay(int);
    void delayMicroseconds(int);
    unsigned long micros();
    void attachInterrupt(int, void (*)(void), int mode);
}

#endif
