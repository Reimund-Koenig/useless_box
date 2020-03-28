#ifndef BOX_AWARENESS_HPP_
#define BOX_AWARENESS_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeAwareness {

  private:
    int box_mode;
    int run_mode_reset_step;
    box::Servomanager* box_servomanager;
    box::Wait* box_wait;
    void run_mode_reset();
    void run_mode_awareness(int distance);
    void run_mode_normal();
    unsigned long last_time;
    unsigned long wait_delay;

  public:
    ModeAwareness(box::Servomanager* box_servomanager,
                  box::Wait* box_wait);
    virtual ~ModeAwareness();
    virtual void run(int distance);

};

} // namespace box

#endif // BOX_AWARENESS_HPP_
