#ifndef BOX_SERVOMOTOR_HPP_
#define BOX_SERVOMOTOR_HPP_

#include "Servo.h"
#include "manager/box_wait_manager.hpp"

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
    unsigned long speed_sleep_ms;
    bool clockwise;
    void set_angle(int);
    box::Wait* box_wait_servo_step_speed;
    unsigned long speed_to_millseconds(int speed);
    int calculate_sleep_time_ms();
    void move_step();
    void write();

  public:
    Servomotor(int pin_pwm_servomotor, bool clockwise,
               int min_peak_angle, int max_peak_angle,
               box::Wait* box_wait_servo_step_speed);
    virtual ~Servomotor();
    virtual void move();
    virtual bool current_angle_smaller_than_90_percent();
    virtual int move_to_percent(int percent, int speed);
    virtual int get_current_angle();
    virtual int get_angle();
    virtual int get_last_percentage();
};

} // namespace box

#endif // BOX_SERVOMOTOR_HPP_
