#ifndef BOX_SERVOMOTOR_HPP_
#define BOX_SERVOMOTOR_HPP_

#include "Servo.h"
#include "box_wait.hpp"

namespace box {

class Servomotor {

  private:
    Servo servo;
    int angle;
    int current_angle;
    int last_percentage;
    int min_peak_angle;
    int max_peak_angle;
    bool hit_switch;
    unsigned long speed_in_milliseconds;
    bool clockwise;
    void set_angle(int);
    box::Wait* box_wait;
    unsigned long speed_to_millseconds(int speed);
    int calculate_sleep_time(int speed);

  public:
    Servomotor(int pin_pwm_servomotor, bool clockwise,
               int min_peak_angle, int max_peak_angle,
               box::Wait* box_wait);
    virtual ~Servomotor();
    virtual void move_step();
    virtual int set_speed_and_get_sleeptime(int speed);
    virtual void move_to_percent(int percent);
    virtual void move_to_angle(int angle);
    virtual int get_current_angle();
    virtual int get_angle();
    virtual int get_last_percentage();
};

} // namespace box

#endif // BOX_SERVOMOTOR_HPP_
