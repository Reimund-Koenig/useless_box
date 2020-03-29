#ifndef BOX_CONTROLLER_HPP_
#define BOX_CONTROLLER_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"
#include "box_mode_awareness.hpp"
#include "box_mode_normal.hpp"
#include "box_mode_reset.hpp"
#include "box_mode_startup.hpp"

namespace box {

class Controller {

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
    box::ModeStartup* box_mode_startup;
    void select_new_box_mode();

  public:
    Controller(box::Switch* box_switch,
         box::Sonar* box_sonar,
         box::Servomanager* box_servomanager,
         box::Wait* box_wait,
         box::ModeReset* box_mode_reset,
         box::ModeNormal* box_mode_normal,
         box::ModeAwareness* box_mode_awareness,
         box::ModeStartup* box_mode_startup);
    virtual ~Controller();
    virtual void run();

};

} // namespace box

#endif // BOX_CONTROLLER_HPP_
