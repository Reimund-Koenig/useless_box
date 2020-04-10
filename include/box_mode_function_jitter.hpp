#ifndef BOX_MODE_FUCTION_JITTER_HPP_
#define BOX_MODE_FUCTION_JITTER_HPP_

#include "box_servomanager.hpp"

namespace box {

class ModeFunctionJitter {

  private:
    box::Servomanager* box_servomanager;
    int box_mode_jitter_state;
    int jitter_speed;
    int jitter_count;

  public:
    ModeFunctionJitter(box::Servomanager* box_servomanager);
    virtual ~ModeFunctionJitter();
    virtual bool run(bool pilot, int count, int percent_1, int percent_2, int speed);

};

} // namespace box

#endif // BOX_MODE_FUCTION_JITTER_HPP_
