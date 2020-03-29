#ifndef BOX_MAIN_HPP_
#define BOX_MAIN_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"
#include "box_mode_awareness.hpp"
#include "box_mode_normal.hpp"
#include "box_mode_reset.hpp"

namespace box {

class Main {

  private:
    int box_mode;
    bool is_reset_finished;
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomanager* box_servomanager;
    box::Wait* box_wait;
    box::ModeAwareness* box_mode_awareness;
    box::ModeNormal* box_mode_normal;
    box::ModeReset* box_mode_reset;
    void select_new_box_mode();

  public:
    Main(box::Switch* box_switch,
         box::Sonar* box_sonar,
         box::Servomanager* box_servomanager,
         box::Wait* box_wait,
         box::ModeReset* box_mode_reset,
         box::ModeNormal* box_mode_normal,
         box::ModeAwareness* box_mode_awareness);
    virtual ~Main();
    virtual void run();

};

} // namespace box

#endif // BOX_MAIN_HPP_
