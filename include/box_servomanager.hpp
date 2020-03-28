#ifndef BOX_SERVOMANAGER_HPP_
#define BOX_SERVOMANAGER_HPP_

#include "box_servomotor.hpp"

namespace box {

class Servomanager {
  private:
    box::Servomotor* box_lower_servo;
    box::Servomotor* box_upper_servo;
    bool move_vice_versa;

  public:
    Servomanager(box::Servomotor* box_lower_servo,
                 box::Servomotor* box_upper_servo);
    virtual ~Servomanager();
    void move_lower_servo_to_percent(int percentage);
    void move_upper_servo_to_percent(int percentage);
    void move_servos_to_percent(int percentage_lower,
                                int percentage_upper);
    bool is_user_action();
    void change_vise_versa_mode();
};
}

#endif // BOX_SERVOMANAGER_HPP_