/**
 * Arduino Serial mock
 */
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <gmock/gmock.h>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Serial_ {

  private:
    static bool printToCout;

  public:
    static void setPrintToCout(bool flag);

  public:
    // static size_t print(const char[]);
    // static size_t print(char);
    // static size_t print(unsigned char, int = DEC);
    // static size_t print(int, int = DEC);
    // static size_t print(unsigned int, int = DEC);
    // static size_t print(long, int = DEC);
    // static size_t print(unsigned long, int = DEC);
    // static size_t print(double, int = 2);

    // static size_t println(const char[]);
    // static size_t println(char);
    // static size_t println(unsigned char, int = DEC);
    // static size_t println(int, int = DEC);
    // static size_t println(unsigned int, int = DEC);
    // static size_t println(long, int = DEC);
    // static size_t println(unsigned long, int = DEC);
    // static size_t println(double, int = 2);
    // static size_t println(void);

    // size_t write(int);
    // size_t write(const char *str);
    // size_t write(const int *buffer, size_t size);

    int begin(int);

    // int available();
    // int read();

    // static void flush();

    /*
    TODO: Not implemented yet.
    int getWriteError();
    void clearWriteError();
    static size_t print(const __FlashStringHelper *);
    static size_t print(const String &);
    static size_t print(const Printable&);
    static size_t println(const __FlashStringHelper *);
    static size_t println(const String &s);
    static size_t println(const Printable&);
    */
};
extern Serial_ Serial;

// SerialMock* serialMockInstance();
#endif // SERIAL_H