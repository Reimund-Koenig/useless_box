#ifndef Serial_h
#define Serial_h

#include <stddef.h>

class Serial {
  public:
    Serial();
    int begin(int baut);
    size_t write(const char* str);
    size_t print(int i);
    size_t print(const char c);
    size_t print(const char* str);
    size_t println(const char* str);
};

#endif // Serial_h
