#ifndef BOX_MODEMANAGER_HPP_
#define BOX_MODEMANAGER_HPP_

#include "peripheral/box_sonar.hpp"
#include "peripheral/box_switch.hpp"
#include "manager/box_servo_manager.hpp"
#include "manager/box_wait_manager.hpp"

#include "modes/box_submode_function_jitter.hpp"
#include "modes/box_mode_awareness.hpp"
#include "modes/box_mode_reset.hpp"

namespace box {

class ModeManager {

  private:
    box::SubModeFunctionJitter* box_submode_function_jitter;
    box::ModeAwareness* box_mode_awareness;
    box::ModeReset* box_mode_reset;

  public:
    ModeManager(box::Servomanager* box_servo_manager, box::Switch* box_switch);
    virtual ~ModeManager();
    virtual bool run_mode_awareness(int distance);
    virtual bool run_mode_reset();
};

} // namespace box

#endif // BOX_MODEMANAGER_HPP_
