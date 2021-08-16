#ifndef BOX_SERVOMANAGER_HPP_
#define BOX_SERVOMANAGER_HPP_

#include "peripheral/box_servomotor.hpp"
#include "peripheral/box_switch.hpp"

namespace box {

class Servomanager {
  private:
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    box::Switch* box_switch;
    box::Wait* box_wait_till_servo_finished_moving;

  public:
    Servomanager(box::Servomotor* box_lower_servo,
                 box::Servomotor* box_upper_servo,
                 box::Wait* box_wait_till_servo_finished_moving,
                 box::Switch* box_switch);
    virtual ~Servomanager();
    virtual void move_steps();
    virtual void move_pilot_servo_to_percent(int percentage, int speed);
    virtual void move_copilot_servo_to_percent(int percentage, int speed);
    virtual bool box_servos_not_reached_switch();
};
}

#endif // BOX_SERVOMANAGER_HPP_
