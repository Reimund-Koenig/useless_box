#ifndef BOX_MAIN_HPP_
#define BOX_MAIN_HPP_

#include "box_servomotor.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"

namespace box {

class Main {

  private:
    int m_lastResult = -1;
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    box::Sonar* box_sonar;
    box::Switch* box_switch;

  public:
    Main();
    virtual ~Main();
    virtual void run();

};

} // namespace box

#endif // BOX_MAIN_HPP_
