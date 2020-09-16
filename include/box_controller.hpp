#ifndef BOX_CONTROLLER_HPP_
#define BOX_CONTROLLER_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"
#include "box_mode_manager.hpp"

namespace box {

class Controller {

  private:
    int box_mode;
    int box_speed;
    int distance;
    bool is_mode_finished;
    bool sleep_mode_enabled;
    int time_till_sleep;
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomanager* box_servomanager;
    box::Wait* box_wait_controller;
    box::Wait* box_wait_deepsleep;
    box::ModeManager* box_mode_manager;
    void switch_box_mode();

  public:
    Controller(
        box::Switch* box_switch,
        box::Sonar* box_sonar,
        box::Servomanager* box_servomanager,
        box::Wait* box_wait_controller,
        box::Wait* box_wait_deepsleep,
        box::ModeManager* box_mode_manager);
    virtual ~Controller();
    virtual void run();

};

} // namespace box

#endif // BOX_CONTROLLER_HPP_
