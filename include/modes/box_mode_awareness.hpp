#ifndef BOX_AWARENESS_HPP_
#define BOX_AWARENESS_HPP_

#include "modes/box_mode_function_jitter.hpp"
#include "manager/box_wait_manager.hpp"
#include "manager/box_servo_manager.hpp"
#include "peripheral/box_sonar.hpp"
#include "peripheral/box_switch.hpp"


namespace box {

class ModeAwareness {

  private:
    box::Servomanager* box_servo_manager;
    box::ModeFunctionJitter* box_mode_function_jitter;
    box::Wait* box_wait_controller;
    int last_distance;
    bool random_jitter;
    int jitter_percentage_1;
    int jitter_percentage_2;
    int jitter_count;
    int jitter_speed;
    bool run_awareness(int distance);
    bool run_jitter();
    void init_jitter(int box_percentage);

  public:
    ModeAwareness(box::Servomanager* box_servo_manager,
                  box::ModeFunctionJitter* box_mode_function_jitter,
                  box::Wait* box_wait_controller);
    virtual ~ModeAwareness();
    virtual bool run(int distance);

};

} // namespace box

#endif // BOX_AWARENESS_HPP_
