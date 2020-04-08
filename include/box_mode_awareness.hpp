#ifndef BOX_AWARENESS_HPP_
#define BOX_AWARENESS_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeAwareness {

  private:
    box::Servomanager* box_servomanager;
    box::Wait* box_wait_controller;
    int last_distance;

  public:
    ModeAwareness(box::Servomanager* box_servomanager,
                  box::Wait* box_wait_controller);
    virtual ~ModeAwareness();
    virtual bool run(int distance);

};

} // namespace box

#endif // BOX_AWARENESS_HPP_
