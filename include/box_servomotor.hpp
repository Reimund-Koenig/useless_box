#ifndef BOX_SERVOMOTOR_HPP_
#define BOX_SERVOMOTOR_HPP_

#include "Servo.h"

namespace box {

class Servomotor {

  private:
    Servo servo;
    int angle;
    int min_peak_angle;
    int max_peak_angle;
    int speed_delay_ms;
    bool clockwise;
    virtual void set_angle(int);

  public:
    Servomotor(int pin_pwm_servomotor, bool clockwise,
               int min_peak_angle, int max_peak_angle);
    virtual ~Servomotor();
    virtual void move_to_percent(int percent);
    virtual void move_to_angle(int angle);
    virtual int get_current_angle();
    virtual int get_angle();
};

} // namespace box

#endif // BOX_SERVOMOTOR_HPP_
