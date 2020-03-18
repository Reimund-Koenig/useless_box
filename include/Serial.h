#ifndef Serial_h
#define Serial_h

#include <stddef.h>

class Serial {
  public:
    Serial();
    int begin(int baut);
    size_t write(const char* str);
};

#endif // Serial_h
