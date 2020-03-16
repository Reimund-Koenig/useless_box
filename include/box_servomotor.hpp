#ifndef BOX_SERVOMOTOR_HPP_
#define BOX_SERVOMOTOR_HPP_

#include "Servo.h"

#define SERVOMOTOR_DIRECTION_FORWARD 0
#define SERVOMOTOR_DIRECTION_BACKWARD 1
#define SERVOMOTOR_DIRECTION_STOP 2

#define SERVOMOTOR_DEFAULT_DIRECTION SERVOMOTOR_DIRECTION_STOP

namespace box {

class Servomotor {

  private:
    Servo servo;
    int direction;
    int angle;
    int min_peak_angle;
    int max_peak_angle;
    int speed_delay_ms;
    virtual void move_forward();
    virtual void move_backward();
    virtual void move_stop();
    virtual void set_direction(int);
    virtual void set_angle(int);

  // protected:

  public:
    Servomotor(int pin_pwm_servomotor, int min_peak_angle, int max_peak_angle);
    virtual ~Servomotor();
    virtual void move_to_percent(int percent);
    virtual void move(int direction);
    virtual int get_current_angle();
    virtual int get_direction();
    virtual int get_angle();
};

} // namespace box

#endif // BOX_SERVOMOTOR_HPP_
