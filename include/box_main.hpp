#ifndef BOX_MAIN_HPP_
#define BOX_MAIN_HPP_

#include "box_controller.hpp"
#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"
#include "box_mode_manager.hpp"

namespace box {

class Main {
  public:
    Main();
    virtual ~Main();
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    box::Servomanager* box_servomanager;
    box::Wait* box_wait;
    box::Wait* box_wait_lower_servo;
    box::Wait* box_wait_upper_servo;
    box::ModeManager* box_mode_manager;
    box::Controller* box_controller;
};

} // namespace box

#endif // BOX_MAIN_HPP_
