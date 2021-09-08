#ifndef BOX_AWARENESS_HPP_
#define BOX_AWARENESS_HPP_

#include "modes/box_submode_function_jitter.hpp"
#include "manager/box_servo_manager.hpp"
#include "peripheral/box_sonar.hpp"
#include "peripheral/box_switch.hpp"


namespace box {

class ModeAwareness {

  private:
    box::Servomanager* box_servo_manager;
    box::SubModeFunctionJitter* box_submode_function_jitter;
    int last_distance;
    int box_percentage;
    int box_speed;
    bool run_submode_jitter;
    void decide_for_jitter(int distance);
    bool run_awareness(int distance);

  public:
    ModeAwareness(box::Servomanager* box_servo_manager,
                  box::SubModeFunctionJitter* box_submode_function_jitter);
    virtual ~ModeAwareness();
    virtual bool run(int distance);

};

} // namespace box

#endif // BOX_AWARENESS_HPP_
