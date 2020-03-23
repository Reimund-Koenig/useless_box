#ifndef BOX_SYSTEMTEST_HPP_
#define BOX_SYSTEMTEST_HPP_

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
    void test_lower_servomotor();
    void test_upper_servomotor();
    void test_switch();
    void test_sonar();
    void test_sonar_avarage();
    void println(const char* str, int val);
    box::Potentiometer* box_potentiometer;
    box::Servomotor* box_lower_servomotor;
    box::Servomotor* box_upper_servomotor;
    box::Button* box_button;
    box::Switch* box_switch;
    box::Sonar* box_sonar;
    int last_switch_state;
    unsigned long last_debounce_time;

  public:
    Systemtest();
    virtual ~Systemtest();
    virtual void run();

};

} // namespace box

#endif // BOX_SYSTEMTEST_HPP_
