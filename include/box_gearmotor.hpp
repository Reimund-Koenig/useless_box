#ifndef BOX_GEARMOTOR_HPP_
#define BOX_GEARMOTOR_HPP_

#define GEARMOTOR_DIRECTION_FORWARD 0
#define GEARMOTOR_DIRECTION_BACKWARD 1
#define GEARMOTOR_DIRECTION_STOP 2

#define GEARMOTOR_DEFAULT_SPEED 100
#define GEARMOTOR_DEFAULT_DIRECTION GEARMOTOR_DIRECTION_STOP

namespace box {

class Gearmotor {

  private:
    int direction;
    int speed;
    int pin_pwm_speed = 10;
    int pin_forward = 9;
    int pin_backward = 8;
    virtual void move_forward();
    virtual void move_backward();
    virtual void move_stop();
    virtual void set_direction(int);
    virtual void set_speed(int);

  protected:
    Gearmotor(int pin_pwm_speed, int pin_forward, int pin_backward);

  public:
    virtual ~Gearmotor();
    virtual void move(int direction, int speed);
    virtual int get_direction();
    virtual int get_speed();
};

} // namespace box

#endif // BOX_GEARMOTOR_HPP_
