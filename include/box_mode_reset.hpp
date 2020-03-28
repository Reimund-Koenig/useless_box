#ifndef BOX_RESET_HPP_
#define BOX_RESET_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeReset {

  private:
    box::Servomanager* box_servomanager;
    box::Wait* box_wait;
    int run_mode_reset_step;
    void run_mode_reset();

  public:
    ModeReset(box::Servomanager* box_servomanager,
                  box::Wait* box_wait);
    virtual ~ModeReset();
    virtual void run();

};

} // namespace box

#endif // BOX_RESET_HPP_
