#ifndef BOX_SUBMODE_FUCTION_JITTER_HPP_
#define BOX_SUBMODE_FUCTION_JITTER_HPP_

#include "manager/box_servo_manager.hpp"

namespace box {

class SubModeFunctionJitter {

  private:
    box::Servomanager* box_servo_manager;
    int box_mode_jitter_state;
    int jitter_speed;
    int jitter_count;
    int jitter_percentage_start;
    int jitter_percentage_end;

  public:
    SubModeFunctionJitter(box::Servomanager* box_servo_manager);
    virtual ~SubModeFunctionJitter();
    virtual void init(int box_percentage);
    virtual bool run();

};

} // namespace box

#endif // BOX_SUBMODE_FUCTION_JITTER_HPP_
