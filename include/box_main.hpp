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
    box::Controller* box_controller;
  private:
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    box::Servomanager* box_servo_manager;
    box::Wait* box_wait_till_next_distance_measurement;
    box::Wait* box_wait_deepsleep;
    box::Wait* box_wait_servo_step_speed_lower;
    box::Wait* box_wait_servo_step_speed_upper;
    box::Wait* box_wait_till_servomanager_finished_moving;
    box::ModeManager* box_mode_manager;
};

} // namespace box

#endif // BOX_MAIN_HPP_
