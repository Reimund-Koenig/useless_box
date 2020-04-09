#ifndef BOX_STARTUP_HPP_
#define BOX_STARTUP_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeStartup {

  private:
    box::Servomanager* box_servomanager;
    box::Wait* box_wait_controller;
    box::Switch* box_switch;
    int box_mode_state;
    int box_mode_jitter_state;
    int jitter_speed;
    bool switch_start_state_is_high;
    bool run_jitter();

  public:
    ModeStartup(box::Servomanager* box_servomanager,
                  box::Wait* box_wait_controller,
                  box::Switch* box_switch);
    virtual ~ModeStartup();
    virtual bool run();

};

} // namespace box

#endif // BOX_STARTUP_HPP_
