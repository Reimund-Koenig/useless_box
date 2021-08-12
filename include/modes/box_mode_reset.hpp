#ifndef BOX_RESET_HPP_
#define BOX_RESET_HPP_

#include "manager/box_servo_manager.hpp"
#include "manager/box_wait_manager.hpp"
#include "peripheral/box_sonar.hpp"
#include "peripheral/box_switch.hpp"

namespace box {

class ModeReset {

  private:
    box::Servomanager* box_servo_manager;
    box::Wait* box_wait_controller;
    int run_mode_reset_step;
    bool switch_start_state_is_high;
    void run_mode_reset();

  public:
    ModeReset(box::Servomanager* box_servo_manager,
              box::Wait* box_wait_controller);
    virtual ~ModeReset();
    virtual bool run();

};

} // namespace box

#endif // BOX_RESET_HPP_
