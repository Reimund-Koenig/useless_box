#ifndef BOX_STARTSEQUENCE_HPP_
#define BOX_STARTSEQUENCE_HPP_

#include "box_mode_function_jitter.hpp"
#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeStartSequence {

  private:
    box::Servomanager* box_servomanager;
    box::ModeFunctionJitter* box_mode_function_jitter;
    box::Wait* box_wait_controller;
    box::Switch* box_switch;
    int box_mode_state;
    bool switch_start_state_is_high;

  public:
    ModeStartSequence(box::Servomanager* box_servomanager,
                box::ModeFunctionJitter* box_mode_function_jitter,
                box::Wait* box_wait_controller,
                box::Switch* box_switch);
    virtual ~ModeStartSequence();
    virtual bool run();

};

} // namespace box

#endif // BOX_STARTSEQUENCE_HPP_
