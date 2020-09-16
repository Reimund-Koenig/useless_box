#ifndef BOX_MODEMANAGER_HPP_
#define BOX_MODEMANAGER_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

#include "box_mode_function_jitter.hpp"
#include "box_mode_awareness.hpp"
#include "box_mode_reset.hpp"

namespace box {

class ModeManager {

  private:
    box::ModeFunctionJitter* box_mode_function_jitter;
    box::ModeAwareness* box_mode_awareness;
    box::ModeReset* box_mode_reset;

  public:
    ModeManager(box::Servomanager* box_servomanager,
                  box::Wait* box_wait_controller, box::Switch* box_switch);
    virtual ~ModeManager();
    virtual bool run_mode_awareness(int distance);
    virtual bool run_mode_reset();
};

} // namespace box

#endif // BOX_MODEMANAGER_HPP_
