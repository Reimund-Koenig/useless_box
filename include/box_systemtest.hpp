#ifndef BOX_SYSTEMTEST_HPP_
#define BOX_SYSTEMTEST_HPP_

#include "box_button.hpp"
#include "box_servomotor.hpp"
#include "box_switch.hpp"
#include "box_sonar.hpp"

namespace box {

class Systemtest {

  private:
    int systemtest_state;
    int number_of_functions;
    int m_lastResult = -1;
    void update_systemtest_state();
    void test_servomotor_1();
    void test_servomotor_2();
    void test_switch();
    void test_sonar();
    box::Servomotor* box_servo1;
    box::Servomotor* box_servo2;
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
