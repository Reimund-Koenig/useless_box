#ifndef BOX_SYSTEMTEST_HPP_
#define BOX_SYSTEMTEST_HPP_

#include "Arduino.h"
#include "Servo.h"
#include "box_button.hpp"
#include "box_switch.hpp"
#include "box_potentiometer.hpp"
#include "box_sonar.hpp"
#include "box_servomotor.hpp"

namespace box {

class Systemtest {

  private:
    int systemtest_state;
    int number_of_functions;
    int servomotor_angle = -1;
    void update_systemtest_state();
    void test_servomotor_1();
    void test_servomotor_2();
    void test_switch();
    void test_sonar();
    box::Potentiometer* box_potentiometer;
    box::Servomotor* box_servomotor_1;
    box::Servomotor* box_servomotor_2;
    box::Button* box_button;
    box::Switch* box_switch;
    box::Sonar* box_sonar;

  protected:
    Systemtest();

  public:
    virtual ~Systemtest();
    virtual void run();

};

} // namespace box

#endif // BOX_SYSTEMTEST_HPP_
