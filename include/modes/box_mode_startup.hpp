#ifndef BOX_STARTUP_HPP_
#define BOX_STARTUP_HPP_

#include "modes/box_mode_function_jitter.hpp"
#include "manager/box_servo_manager.hpp"
#include "manager/box_wait_manager.hpp"
#include "peripheral/box_sonar.hpp"
#include "peripheral/box_switch.hpp"

namespace box {

class ModeStartup {

  private:
    box::Servomanager* box_servo_manager;

  public:
    ModeStartup(box::Servomanager* box_servo_manager);
    virtual ~ModeStartup();
    virtual bool run();

};

} // namespace box

#endif // BOX_STARTUP_HPP_
