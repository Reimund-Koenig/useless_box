#ifndef BOX_NORMAL_HPP_
#define BOX_NORMAL_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeNormal {

  private:
    box::Servomanager* box_servomanager;
    box::Wait* box_wait;
    int run_mode_normal_step;
    void run_mode_reset();

  public:
    ModeNormal(box::Servomanager* box_servomanager,
                  box::Wait* box_wait);
    virtual ~ModeNormal();
    virtual void run();

};

} // namespace box

#endif // BOX_NORMAL_HPP_
