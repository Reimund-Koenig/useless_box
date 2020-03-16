#ifndef UB_MAIN_HPP_
#define UB_MAIN_HPP_

#include "ub_servomotor.hpp"

namespace ub {

class Main {

  private:
    int m_lastResult = -1;
    ub::Servomotor* servo1;

    int pin_servomotor_pwm = 9;

  protected:
  public:
    Main();
    virtual ~Main();
    virtual void run();

};

} // namespace ub

#endif // UB_MAIN_HPP_
