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
    static size_t print(const char[]);
    static size_t print(int);
    static size_t println(const char[]);
    size_t write(const char *str);
    int begin(int);
    // static void flush();
    // int available();
    // int read();
};
extern Serial_ Serial;

#endif // SERIAL_H
