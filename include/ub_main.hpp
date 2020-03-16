#ifndef UB_MAIN_HPP_
#define UB_MAIN_HPP_

// #include "ub_servomotor.hpp"
// #include "Servo.h"

namespace ub {

class Main {

  private:
    int m_lastResult = -1;
    // ub::Servomotor* servo1;

    // Gearmotor 1 Pin Setup
    int pin_gearmotor_1_pwm_speed = 10;
    int pin_gearmotor_1_forward = 9;
    int pin_gearmotor_1_backward = 8;

    // Gearmotor 2 Pin Setup
    int pin_gearmotor_2_pwm_speed = 5;
    int pin_gearmotor_2_forward = 7;
    int pin_gearmotor_2_backward = 6;

  protected:
  public:
    Main();
    virtual ~Main();
    virtual void run();
    virtual void addition(int, int);
    virtual int lastResult();

};

} // namespace ub

#endif // UB_MAIN_HPP_
