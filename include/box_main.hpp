#ifndef BOX_MAIN_HPP_
#define BOX_MAIN_HPP_

#include "box_servomotor.hpp"

namespace box {

class Main {

  private:
    int m_lastResult = -1;
    box::Servomotor* lower_servo;
    box::Servomotor* upper_servo;

  public:
    Main();
    virtual ~Main();
    virtual void run();

};

} // namespace box

#endif // BOX_MAIN_HPP_
