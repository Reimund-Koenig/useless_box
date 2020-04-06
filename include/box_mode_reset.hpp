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
    bool switch_start_state_is_high;
    void run_mode_reset();
    void do_move(int percent_pilot, int percent_copilot);

  public:
    ModeReset(box::Servomanager* box_servomanager, box::Wait* box_wait);
    virtual ~ModeReset();
    virtual bool run();

};

} // namespace box

#endif // BOX_RESET_HPP_
