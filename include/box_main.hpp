#ifndef BOX_MAIN_HPP_
#define BOX_MAIN_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"

namespace box {

class Main {

  private:
    int box_mode;
    int run_mode_reset_step;
    box::Sonar* box_sonar;
    box::Switch* box_switch;
    box::Servomanager* box_servomanager;
    void wait_ms(int wait_ms);
    void run_mode_reset();
    void run_mode_awareness(int distance);
    void run_mode_normal();
    unsigned long last_time;
    unsigned long wait_delay;

  public:
    Main(box::Switch* box_switch,
         box::Sonar* box_sonar,
         box::Servomanager* box_servomanager);
    virtual ~Main();
    virtual void run();

};

} // namespace box

#endif // BOX_MAIN_HPP_
