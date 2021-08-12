#ifndef BOX_MAIN_HPP_
#define BOX_MAIN_HPP_

#include "box_controller.hpp"
#include "manager/box_servo_manager.hpp"
#include "manager/box_mode_manager.hpp"
#include "manager/box_wait_manager.hpp"
#include "peripheral/box_sonar.hpp"
#include "peripheral/box_switch.hpp"

namespace box {

class Main {
  public:
    Main();
    virtual ~Main();
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    box::Servomanager* box_servo_manager;
    box::Wait* box_wait_servo_speed_control;
    box::Wait* box_wait_deepsleep;
    box::Wait* box_wait_lower_servo;
    box::Wait* box_wait_upper_servo;
    box::ModeManager* box_mode_manager;
    box::Controller* box_controller;
};

} // namespace box

#endif // BOX_MAIN_HPP_
