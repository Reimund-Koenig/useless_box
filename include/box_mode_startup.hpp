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
    box::Wait* box_wait;
    box::Switch* box_switch;
    int box_mode_state;
    bool switch_start_state_is_high;
    void run_mode_reset();

  public:
    ModeStartup(box::Servomanager* box_servomanager,
                  box::Wait* box_wait, box::Switch* box_switch);
    virtual ~ModeStartup();
    virtual bool run();

};

} // namespace box

#endif // BOX_STARTUP_HPP_
