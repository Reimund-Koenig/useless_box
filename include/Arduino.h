#ifndef Arduino_h
#define Arduino_h

extern "C" {
    #define OUTPUT 0
    #define LOW 0
    #define HIGH 1
    void pinMode(int, int);
    void digitalWrite(int, int);
    void analogWrite(int, int);
}

#endif
