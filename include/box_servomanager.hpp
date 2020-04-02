#ifndef BOX_SERVOMANAGER_HPP_
#define BOX_SERVOMANAGER_HPP_

#include "box_servomotor.hpp"

namespace box {

class Servomanager {
  private:
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    bool move_vice_versa;
    bool should_change_to_vice_versa_mode;

  public:
    Servomanager(box::Servomotor* box_lower_servo,
                 box::Servomotor* box_upper_servo);
    virtual ~Servomanager();
    virtual void move_steps(int speed);
    virtual void move_lower_servo_to_percent(int percentage);
    virtual void move_upper_servo_to_percent(int percentage);
    virtual void move_servos_to_percent(int percentage_lower,
                                int percentage_upper);
    virtual bool is_no_box_action();
    virtual bool is_vice_versa_mode_activated();
    virtual bool change_vise_versa_if_required_and_return_is_changed();
    virtual void random_select_if_vice_versa_mode_should_be_changed();
};
}

#endif // BOX_SERVOMANAGER_HPP_