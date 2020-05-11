#ifndef BOX_WAKEUP_HPP_
#define BOX_WAKEUP_HPP_

#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeWakeup {

  public:
    ModeWakeup();
    virtual ~ModeWakeup();
    virtual bool run(int distance);

};

} // namespace box

#endif // BOX_WAKEUP_HPP_
