#ifndef BOX_CONTROLLER_HPP_
#define BOX_CONTROLLER_HPP_

#include "manager/box_servo_manager.hpp"
#include "manager/box_wait_manager.hpp"
#include "manager/box_mode_manager.hpp"
#include "peripheral/box_sonar.hpp"
#include "peripheral/box_switch.hpp"

namespace box {

class Controller {

  private:
    int box_mode;
    int box_speed;
    int distance;
    bool is_mode_finished;
    int time_till_sleep;
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomanager* box_servo_manager;
    box::Wait* box_wait_servo_speed_control;
    box::Wait* box_wait_deepsleep;
    box::ModeManager* box_mode_manager;
    void reset_servos_blocking();
    void switch_box_mode();
    void switch_to_reset_mode();
    void deep_sleep_till_switch_is_toggled();

  public:
    Controller(
        bool is_engery_safe_mode,
        box::Switch* box_switch,
        box::Sonar* box_sonar,
        box::Servomanager* box_servo_manager,
        box::Wait* box_wait_servo_speed_control,
        box::Wait* box_wait_deepsleep,
        box::ModeManager* box_mode_manager);
    virtual ~Controller();
    virtual void run();

};

} // namespace box

#endif // BOX_CONTROLLER_HPP_
