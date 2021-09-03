#ifndef BOX_RESET_HPP_
#define BOX_RESET_HPP_

#include "manager/box_servo_manager.hpp"

namespace box {

class ModeReset {

  private:
    box::Servomanager* box_servo_manager;
    int box_speed;
    int run_mode_reset_step;
    void run_mode_reset();

  public:
    ModeReset(box::Servomanager* box_servo_manager);
    virtual ~ModeReset();
    virtual bool run();

};

} // namespace box

#endif // BOX_RESET_HPP_
