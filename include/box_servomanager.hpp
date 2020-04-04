#ifndef BOX_SERVOMANAGER_HPP_
#define BOX_SERVOMANAGER_HPP_

#include "box_servomotor.hpp"
#include "box_switch.hpp"

namespace box {

class Servomanager {
  private:
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    box::Switch* box_switch;

  public:
    Servomanager(box::Servomotor* box_lower_servo,
                 box::Servomotor* box_upper_servo,
                 box::Switch* box_switch);
    virtual ~Servomanager();
    virtual void move_steps(int speed);
    virtual void move_pilot_servo_to_percent(int percentage);
    virtual void move_copilot_servo_to_percent(int percentage);
    virtual void move_servos_to_percent(int percentage_lower,
                                int percentage_upper);
    virtual bool is_no_box_action();
};
}

#endif // BOX_SERVOMANAGER_HPP_