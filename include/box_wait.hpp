#ifndef BOX_WAIT_HPP_
#define BOX_WAIT_HPP_

#include "ardunio_namespace.h" // needed for arduino build
#include <Arduino.h>
#include <stdio.h>

namespace box {

class Wait {
  private:
    unsigned long last_time;
    unsigned long wait_delay;
  public:
    Wait();
    virtual ~Wait();
    virtual void milliseconds(unsigned long milliseconds);
    virtual bool is_free();
};

} // namespace box

#endif // BOX_CONTROLLER_HPP_