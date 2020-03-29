#ifndef BOX_STARTUP_HPP_
#define BOX_STARTUP_HPP_

#include "box_servomanager.hpp"
#include "box_sonar.hpp"
#include "box_switch.hpp"
#include "box_wait.hpp"

namespace box {

class ModeStartup {

  private:
    box::Servomanager* box_servomanager;
    box::Wait* box_wait;
    void run_mode_reset();

  public:
    ModeStartup(box::Servomanager* box_servomanager,
                  box::Wait* box_wait);
    virtual ~ModeStartup();
    virtual void run();

};

} // namespace box

#endif // BOX_STARTUP_HPP_
