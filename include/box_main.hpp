#ifndef BOX_MAIN_HPP_
#define BOX_MAIN_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"
#include "box_mode_awareness.hpp"

namespace box {

class Main {

  private:
    int box_mode;
    int run_mode_reset_step;
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomanager* box_servomanager;
    box::Wait* box_wait;
    box::ModeAwareness* box_mode_awareness;
    void run_mode_reset();
    void run_mode_awareness(int distance);
    void run_mode_normal();

  public:
    Main(box::Switch* box_switch,
         box::Sonar* box_sonar,
         box::Servomanager* box_servomanager);
    virtual ~Main();
    virtual void run();

};

} // namespace box

#endif // BOX_MAIN_HPP_
